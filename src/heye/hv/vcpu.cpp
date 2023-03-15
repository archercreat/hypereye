#include "hypervisor.hpp"
#include "vcpu.hpp"
#include "vmx.hpp"
#include "vmexit.hpp"
#include "heye/shared/trace.hpp"

namespace heye
{
vcpu_t::vcpu_t(hv_t* owner, setup_cb_t setup_cb, teardown_cb_t teardown_cb, vmexit_cb_t vmexit_cb)
    : hv(owner), state(state_t::off), setup_cb(setup_cb), teardown_cb(teardown_cb), vmexit_cb(vmexit_cb)
{
    vmcs       = new vmx::vmcs_t;
    vmxon      = new vmx::vmcs_t;
    io_bitmap  = new vmx::io_bitmap_t;
    msr_bitmap = new vmx::msr_bitmap_t;
    stack      = new stack_t;

    __stosb(reinterpret_cast<unsigned char*>(vmcs),       0, sizeof(vmx::vmcs_t));
    __stosb(reinterpret_cast<unsigned char*>(vmxon),      0, sizeof(vmx::vmcs_t));
    __stosb(reinterpret_cast<unsigned char*>(io_bitmap),  0, sizeof(vmx::io_bitmap_t));
    __stosb(reinterpret_cast<unsigned char*>(msr_bitmap), 0, sizeof(vmx::msr_bitmap_t));
    __stosb(reinterpret_cast<unsigned char*>(stack),      0, sizeof(stack_t));
}

vcpu_t::~vcpu_t()
{
    if (!is_off())
        stop();

    delete   vmcs;
    delete   vmxon;
    delete   io_bitmap;
    delete   msr_bitmap;
    delete[] stack;
}

bool vcpu_t::start()
{
    if (!is_off())
        return false;

    auto cr0 = read<cr0_t>();
    auto cr4 = read<cr4_t>();
    // Make sure CR0 meets CR0 fixed bits in VMX operation.
    //
    auto cr0_fixed0 = read<msr::vmx_cr0_fixed0>();
    auto cr0_fixed1 = read<msr::vmx_cr0_fixed1>();
    if ((~cr0.flags & cr0_fixed0.flags) || (~cr0.flags & cr0_fixed1.flags))
    {
        logger::info("Host CR0 is not allowed in VMX operation");
        return false;
    }
    // Check VMX support.
    //
    auto controls = read<msr::feature_control>();
    if (controls.lock)
    {
        if (!controls.vmxon)
        {
            logger::info("VMX disabled in BIOS");
            return false;
        }
    }
    else
    {
        controls.lock  = 1;
        controls.vmxon = 1;
        write<msr::feature_control>(controls);
    }
    // Enable vmx.
    //
    cr4.vmxe = true;
    // Adjust CR0 and CR4 registers.
    //
    write<cr0_t>(vmx::adjust(cr0));
    write<cr4_t>(vmx::adjust(cr4));
    // Setup vmxon/vmcs.
    //
    vmxon->revision_id = read<msr::vmx_basic>().vmcs_id;
    vmcs->revision_id  = read<msr::vmx_basic>().vmcs_id;
    // Enter vmx root opration.
    //
    if (vmx::on(pa_from_va(vmxon)))
    {
        logger::info("__vmxon failed");
        return false;
    }
    // Mark as `init`.
    //
    state = state_t::init;

    invvpid(vmx::invvpid_t::all_contexts);
    invept(vmx::invept_t::all_contexts);

    if (vmx::clear(pa_from_va(vmcs)) || vmx::vmptrld(pa_from_va(vmcs)))
    {
        logger::info("__vmx_clear || __vmx_vmptrld failed");
        return false;
    }

    if (!setup_guest() || !setup_host() || !setup_controls())
    {
        logger::info("Failed to setup vmcs");
        return false;
    }

    // Pass control to the user defined callback.
    //
    setup_cb(this);

    if (vmx::launch())
    {
        logger::info("Failed to launch with code: 0x%lx", read<vmx::vmcs::vm_instruction_error>());
    }
    else
    {
        logger::info("running in vmx non-root");
        state = state_t::on;
    }
    return is_on();
}

void vcpu_t::stop()
{
    logger::info("Leaving vmx root operation");

    if (is_on())
    {
        vmx::vmcall(vmcall_reason::vmxoff);
    }
    else if (is_init())
    {
        vmx::off();
    }
    // Disable vmx.
    //
    auto cr4 = read<cr4_t>();
    cr4.vmxe = false;
    write<cr4_t>(cr4);
    // Zero out per-processor structs since we might re-enter vm later.
    //
    __stosb(reinterpret_cast<unsigned char*>(vmcs),       0, sizeof(vmx::vmcs_t));
    __stosb(reinterpret_cast<unsigned char*>(vmxon),      0, sizeof(vmx::vmcs_t));
    __stosb(reinterpret_cast<unsigned char*>(io_bitmap),  0, sizeof(vmx::io_bitmap_t));
    __stosb(reinterpret_cast<unsigned char*>(msr_bitmap), 0, sizeof(vmx::msr_bitmap_t));
    __stosb(reinterpret_cast<unsigned char*>(stack),      0, sizeof(stack_t));
    // Mark state as `off`.
    //
    state = state_t::off;
    /// Pass control to the user defined callback.
    ///
    teardown_cb(this);
}

bool vcpu_t::setup_host()
{
    uint64_t err{};

    err |= write<vmx::vmcs::host_es_selector>(read<segment_t<es_t>>().selector.flags & 0xf8);
	err |= write<vmx::vmcs::host_cs_selector>(read<segment_t<cs_t>>().selector.flags & 0xf8);
	err |= write<vmx::vmcs::host_ss_selector>(read<segment_t<ss_t>>().selector.flags & 0xf8);
	err |= write<vmx::vmcs::host_ds_selector>(read<segment_t<ds_t>>().selector.flags & 0xf8);
	err |= write<vmx::vmcs::host_fs_selector>(read<segment_t<fs_t>>().selector.flags & 0xf8);
	err |= write<vmx::vmcs::host_gs_selector>(read<segment_t<gs_t>>().selector.flags & 0xf8);
	err |= write<vmx::vmcs::host_tr_selector>(read<segment_t<tr_t>>().selector.flags & 0xf8);
    err |= write<vmx::vmcs::host_sysenter_cs>(read<msr::sysenter_cs>().flags);
    // Host cr3 is taken from the hypervisor intance (hypervisor should be initialized on driver load).
    //
    err |= write<vmx::vmcs::host_cr3>(hv->system_process_pagetable().flags);
    err |= write<vmx::vmcs::host_cr4>(read<cr4_t>().flags);
    err |= write<vmx::vmcs::host_cr0>(read<cr0_t>().flags);

    err |= write<vmx::vmcs::host_fs_base>(  read<segment_t<fs_t>>().base);
    err |= write<vmx::vmcs::host_gs_base>(  read<segment_t<gs_t>>().base);
    err |= write<vmx::vmcs::host_tr_base>(  read<segment_t<tr_t>>().base);
    err |= write<vmx::vmcs::host_idtr_base>(read<idtr_t>().base);
    err |= write<vmx::vmcs::host_gdtr_base>(read<gdtr_t>().base);

    err |= write<vmx::vmcs::host_sysenter_esp>(read<msr::sysenter_esp>().flags);
    err |= write<vmx::vmcs::host_sysenter_eip>(read<msr::sysenter_eip>().flags);
    // Host rip points to vmexit stub.
    //
    err |= write<vmx::vmcs::host_rip>(reinterpret_cast<uintptr_t>(vmexit_stub));
    // Host rsp points to the top of allocated stack.
    //
    // (Low)               |
    // +-------------------+ <- 0x1000 (stack base)
    // |                   |
    // +-------------------+
    // |    cpu regs       |
    // +-------------------+
    // |    trap frame     |
    // +-------------------+ <- host rsp
    // |  vmexit callback  |
    // +-------------------+
    // |       vcpu*       |
    // +-------------------+ <- 0x2000 (stack base + stack size)
    // (High)              |
    //
    stack->vmexit_handler = vmexit_cb;
    stack->vcpu = this;
    err |= write<vmx::vmcs::host_rip>(reinterpret_cast<uint64_t>(vmexit_stub));
    err |= write<vmx::vmcs::host_rsp>(reinterpret_cast<uint64_t>(&stack->vmexit_handler));

    return err == 0;
}

bool vcpu_t::setup_controls()
{
    uint64_t err{};

    err |= write<vmx::vmcs::virtual_processor_id>(1);

    err |= write<vmx::vmcs::pin_based_vm_exec_control>(vmx::adjust(msr::vmx_pinbased_controls{}).flags);

    msr::vmx_procbased_controls procbased_controls
    {
        .use_msr_bitmaps        = true,
        .use_secondary_controls = true,
    };
    err |= write<vmx::vmcs::cpu_based_vm_exec_control>(vmx::adjust(procbased_controls).flags);

    msr::vmx_procbased_controls2 procbased_controls2
    {
        .enable_ept     = true,
        .enable_rdtcp   = true,
        .enable_vpid    = true,
        .enable_invpcid = true,
        .enable_xsaves  = true,
    };
    err |= write<vmx::vmcs::secondary_vm_exec_control>(vmx::adjust(procbased_controls2).flags);

    msr::vmx_exit_controls exit_controls
    {
        .host_address_space_size = true
    };
    err |= write<vmx::vmcs::vm_exit_controls>(vmx::adjust(exit_controls).flags);

    msr::vmx_entry_controls entry_controls
    {
        .ia32_mode_guest = true
    };
    err |= write<vmx::vmcs::vm_entry_controls>(vmx::adjust(entry_controls).flags);

    err |= write<vmx::vmcs::msr_bitmap>( pa_from_va(msr_bitmap));
    err |= write<vmx::vmcs::ept_pointer>(hv->ept->ept_pointer().flags);
    err |= write<vmx::vmcs::vmcs_link_pointer>(~0ull);

    return err == 0;
}

bool vcpu_t::setup_guest()
{
    const auto gdtr = read<gdtr_t>();
    const auto idtr = read<idtr_t>();

    const auto es   = read<segment_t<es_t>>();
    const auto cs   = read<segment_t<cs_t>>();
    const auto ss   = read<segment_t<ss_t>>();
    const auto ds   = read<segment_t<ds_t>>();
    const auto fs   = read<segment_t<fs_t>>();
    const auto gs   = read<segment_t<gs_t>>();
    const auto tr   = read<segment_t<tr_t>>();
    const auto ldtr = read<segment_t<ldtr_t>>();

    cr0_t cr0_mask{};
    cr3_t cr3_mask{};
    cr4_t cr4_mask{};

    uint64_t err{};

    err |= write<vmx::vmcs::guest_es_selector>(  es.selector.flags);
	err |= write<vmx::vmcs::guest_cs_selector>(  cs.selector.flags);
	err |= write<vmx::vmcs::guest_ss_selector>(  ss.selector.flags);
	err |= write<vmx::vmcs::guest_ds_selector>(  ds.selector.flags);
	err |= write<vmx::vmcs::guest_fs_selector>(  fs.selector.flags);
	err |= write<vmx::vmcs::guest_gs_selector>(  gs.selector.flags);
	err |= write<vmx::vmcs::guest_tr_selector>(  tr.selector.flags);
    err |= write<vmx::vmcs::guest_ldtr_selector>(ldtr.selector.flags);
    err |= write<vmx::vmcs::guest_ia32_debugctl>(read<msr::debugctl>().flags);

    err |= write<vmx::vmcs::guest_es_limit>(  es.limit);
    err |= write<vmx::vmcs::guest_cs_limit>(  cs.limit);
    err |= write<vmx::vmcs::guest_ss_limit>(  ss.limit);
    err |= write<vmx::vmcs::guest_ds_limit>(  ds.limit);
    err |= write<vmx::vmcs::guest_fs_limit>(  fs.limit);
    err |= write<vmx::vmcs::guest_gs_limit>(  gs.limit);
    err |= write<vmx::vmcs::guest_tr_limit>(  tr.limit);
    err |= write<vmx::vmcs::guest_ldtr_limit>(ldtr.limit);
    err |= write<vmx::vmcs::guest_gdtr_limit>(gdtr.limit);
    err |= write<vmx::vmcs::guest_idtr_limit>(idtr.limit);

    err |= write<vmx::vmcs::guest_es_ar_bytes>(  es.rights.flags);
    err |= write<vmx::vmcs::guest_cs_ar_bytes>(  cs.rights.flags);
    err |= write<vmx::vmcs::guest_ss_ar_bytes>(  ss.rights.flags);
    err |= write<vmx::vmcs::guest_ds_ar_bytes>(  ds.rights.flags);
    err |= write<vmx::vmcs::guest_fs_ar_bytes>(  fs.rights.flags);
    err |= write<vmx::vmcs::guest_gs_ar_bytes>(  gs.rights.flags);
    err |= write<vmx::vmcs::guest_tr_ar_bytes>(  tr.rights.flags);
    err |= write<vmx::vmcs::guest_ldtr_ar_bytes>(ldtr.rights.flags);

    err |= write<vmx::vmcs::guest_sysenter_cs>(read<msr::sysenter_cs>().flags);

    err |= write<vmx::vmcs::cr0_guest_host_mask>(cr0_mask.flags);
    err |= write<vmx::vmcs::cr4_guest_host_mask>(cr4_mask.flags);

    err |= write<vmx::vmcs::cr0_read_shadow>(read<cr0_t>().flags);
    err |= write<vmx::vmcs::cr4_read_shadow>(read<cr4_t>().flags);

    err |= write<vmx::vmcs::guest_cr0>(read<cr0_t>().flags);
    err |= write<vmx::vmcs::guest_cr3>(read<cr3_t>().flags);
    err |= write<vmx::vmcs::guest_cr4>(read<cr4_t>().flags);

    err |= write<vmx::vmcs::guest_es_base>(  es.base);
    err |= write<vmx::vmcs::guest_cs_base>(  cs.base);
    err |= write<vmx::vmcs::guest_ss_base>(  ss.base);
    err |= write<vmx::vmcs::guest_ds_base>(  ds.base);
    err |= write<vmx::vmcs::guest_fs_base>(  fs.base);
    err |= write<vmx::vmcs::guest_gs_base>(  gs.base);
    err |= write<vmx::vmcs::guest_tr_base>(  tr.base);
    err |= write<vmx::vmcs::guest_ldtr_base>(ldtr.base);
    err |= write<vmx::vmcs::guest_gdtr_base>(gdtr.base);
    err |= write<vmx::vmcs::guest_idtr_base>(idtr.base);

    err |= write<vmx::vmcs::guest_dr7>(read<dr7_t>().flags);

    err |= write<vmx::vmcs::guest_sysenter_esp>(read<msr::sysenter_esp>().flags);
    err |= write<vmx::vmcs::guest_sysenter_eip>(read<msr::sysenter_eip>().flags);

    return err == 0;
}

void vcpu_t::skip_instruction()
{
    write<vmx::vmcs::guest_rip>(regs().rip + read<vmx::vmcs::vm_exit_instruction_len>());
}

uint64_t vcpu_t::id() const
{
    return read<vmx::vmcs::virtual_processor_id>();
}

cpu::regs_t& vcpu_t::regs()
{
    return stack->regs;
}

vmx::exit_reason vcpu_t::exit_reason() const
{
    return static_cast<vmx::exit_reason>(read<vmx::vmcs::vm_exit_reason>() & 0xffff);
}

vmx::exit_qualification_t vcpu_t::exit_qualification() const
{
    vmx::exit_qualification_t qualification{ read<vmx::vmcs::exit_qualification>() };
    return qualification;
}

vmx::vm_interrupt_info_t vcpu_t::exit_interrupt_info() const
{
    vmx::vm_interrupt_info_t intr{ read<vmx::vmcs::vm_exit_intr_info>() & 0xffffffff };
    return intr;
}

vmx::vm_interrupt_info_t vcpu_t::entry_interrupt_info() const
{
    vmx::vm_interrupt_info_t intr{ read<vmx::vmcs::vm_entry_intr_info>() & 0xffffffff };
    return intr;
}
};
