#include "heye/hv/hypervisor.hpp"
#include "heye/hv/vcpu.hpp"
#include "heye/hv/vmx.hpp"

#include "heye/arch/arch.hpp"

#include "heye/shared/asserts.hpp"
#include "heye/shared/trace.hpp"
#include "heye/shared/cpu.hpp"

namespace
{
uint64_t vmentry_control_mask()
{
    if (read<msr::vmx_basic>().true_controls)
        return read<msr::vmx_true_entry_controls>().flags;
    return read<msr::vmx_entry_controls>().flags;
}

uint64_t vmexit_control_mask()
{
    if (read<msr::vmx_basic>().true_controls)
        return read<msr::vmx_true_exit_controls>().flags;
    return read<msr::vmx_exit_controls>().flags;
}

uint64_t pinbased_control_mask()
{
    if (read<msr::vmx_basic>().true_controls)
        return read<msr::vmx_true_pinbased_controls>().flags;
    return read<msr::vmx_pinbased_controls>().flags;
}

uint64_t procbased_control_mask()
{
    if (read<msr::vmx_basic>().true_controls)
        return read<msr::vmx_true_procbased_controls>().flags;
    return read<msr::vmx_procbased_controls>().flags;
}

uint64_t procbased_control2_mask()
{
    return read<msr::vmx_procbased_controls2>().flags;
}

uint64_t adjust_controls(uint64_t value, uint64_t mask)
{
    value &= mask >> 32;
    value |= mask & 0xffffffff;
    return value;
}
};

vcpu_t::vcpu_t() : state(state_t::off)
{
    vmcs        = new vmx::vmcs_t;
    vmxon       = new vmx::vmcs_t;
    stack       = new uint8_t[KERNEL_STACK_SIZE];
    msr_bitmap  = new uint8_t[PAGE_SIZE];
    io_bitmap_a = new uint8_t[PAGE_SIZE];
    io_bitmap_b = new uint8_t[PAGE_SIZE];
}

vcpu_t::~vcpu_t()
{
    if (is_on())
        leave();

    delete vmcs;
    delete vmxon;
    delete stack;
    delete msr_bitmap;
    delete io_bitmap_a;
    delete io_bitmap_b;
}

bool vcpu_t::enter()
{
    if (!is_off())
        return false;

    auto cr0 = read<cr0_t>();
    auto cr4 = read<cr4_t>();
    // Enable vmx.
    //
    cr4.vmxe = true;
    // Adjust CR0 and CR4 registers.
    //
    cr0.flags |= read<msr::vmx_cr0_fixed0>().flags;
    cr0.flags &= read<msr::vmx_cr0_fixed1>().flags;

    cr4.flags |= read<msr::vmx_cr4_fixed0>().flags;
    cr4.flags &= read<msr::vmx_cr4_fixed1>().flags;

    write<cr0_t>(cr0);
    write<cr4_t>(cr4);
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

    vmx::invvpid_all_contexts();
    vmx::invept_all_contexts();

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

    if (vmx::launch())
    {
        logger::info("Failed to launch with code: 0x%lx", vmx::read(vmx::vmcs::vm_instruction_error));
    }
    else
    {
        logger::info("running in vmx non-root");
        state = state_t::on;
    }
    return is_on();
}

void vcpu_t::leave()
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
    __stosb(reinterpret_cast<unsigned char*>(vmxon),       0, sizeof(*vmxon));
    __stosb(reinterpret_cast<unsigned char*>(vmcs),        0, sizeof(*vmcs));
    __stosb(reinterpret_cast<unsigned char*>(msr_bitmap),  0, sizeof(msr_bitmap));
    __stosb(reinterpret_cast<unsigned char*>(stack),       0, sizeof(stack));
    __stosb(reinterpret_cast<unsigned char*>(io_bitmap_a), 0, sizeof(io_bitmap_a));
    __stosb(reinterpret_cast<unsigned char*>(io_bitmap_b), 0, sizeof(io_bitmap_b));
    // Mark state as `off`.
    //
    state = state_t::off;
}

bool vcpu_t::setup_host()
{
    uint8_t err{};

    err |= vmx::write(vmx::vmcs::host_es_selector, read<segment_t<es_t>>().selector.flags & 0xf8);
	err |= vmx::write(vmx::vmcs::host_cs_selector, read<segment_t<cs_t>>().selector.flags & 0xf8);
	err |= vmx::write(vmx::vmcs::host_ss_selector, read<segment_t<ss_t>>().selector.flags & 0xf8);
	err |= vmx::write(vmx::vmcs::host_ds_selector, read<segment_t<ds_t>>().selector.flags & 0xf8);
	err |= vmx::write(vmx::vmcs::host_fs_selector, read<segment_t<fs_t>>().selector.flags & 0xf8);
	err |= vmx::write(vmx::vmcs::host_gs_selector, read<segment_t<gs_t>>().selector.flags & 0xf8);
	err |= vmx::write(vmx::vmcs::host_tr_selector, read<segment_t<tr_t>>().selector.flags & 0xf8);
    err |= vmx::write(vmx::vmcs::host_sysenter_cs, read<msr::sysenter_cs>().flags);
    // Host cr3 is taken from the hypervisor intance (initialized on driver load).
    //
    err |= vmx::write(vmx::vmcs::host_cr3, hypervisor::get().system_process_pagetable().flags);
    err |= vmx::write(vmx::vmcs::host_cr4, read<cr4_t>().flags);
    err |= vmx::write(vmx::vmcs::host_cr0, read<cr0_t>().flags);

    err |= vmx::write(vmx::vmcs::host_fs_base,   read<segment_t<fs_t>>().base);
    err |= vmx::write(vmx::vmcs::host_gs_base,   read<segment_t<gs_t>>().base);
    err |= vmx::write(vmx::vmcs::host_tr_base,   read<segment_t<tr_t>>().base);
    err |= vmx::write(vmx::vmcs::host_idtr_base, read<idtr_t>().base);
    err |= vmx::write(vmx::vmcs::host_gdtr_base, read<gdtr_t>().base);

    err |= vmx::write(vmx::vmcs::host_sysenter_esp, read<msr::sysenter_esp>().flags);
    err |= vmx::write(vmx::vmcs::host_sysenter_eip, read<msr::sysenter_eip>().flags);
    // Host rip points to vmexit stub.
    //
    err |= vmx::write(vmx::vmcs::host_rip, reinterpret_cast<uintptr_t>(vmexit_stub));
    // Host rsp points to the top of allocated stack.
    //
    err |= vmx::write(vmx::vmcs::host_rsp, reinterpret_cast<uintptr_t>(stack + KERNEL_STACK_SIZE - sizeof(void*)));

    return err == 0;
}

bool vcpu_t::setup_controls()
{
    uint8_t err{};

    err |= vmx::write(vmx::vmcs::virtual_processor_id, 1);

    err |= vmx::write(vmx::vmcs::pin_based_vm_exec_control, adjust_controls(0, pinbased_control_mask()));

    msr::vmx_procbased_controls procbased_controls
    {
        .use_msr_bitmaps        = true,
        .use_secondary_controls = true,
    };
    err |= vmx::write(vmx::vmcs::cpu_based_vm_exec_control, adjust_controls(procbased_controls.flags, procbased_control_mask()));

    msr::vmx_procbased_controls2 procbased_controls2
    {
        .enable_ept     = true,
        .enable_rdtcp   = true,
        .enable_vpid    = true,
        .enable_invpcid = true,
        .enable_xsaves  = true,
    };
    err |= vmx::write(vmx::vmcs::secondary_vm_exec_control, adjust_controls(procbased_controls2.flags, procbased_control2_mask()));

    msr::vmx_exit_controls exit_controls
    {
        .host_address_space_size = true
    };
    err |= vmx::write(vmx::vmcs::vm_exit_controls, adjust_controls(exit_controls.flags, vmexit_control_mask()));

    msr::vmx_entry_controls entry_controls
    {
        .ia32_mode_guest = true
    };
    err |= vmx::write(vmx::vmcs::vm_entry_controls, adjust_controls(entry_controls.flags, vmentry_control_mask()));

    err |= vmx::write(vmx::vmcs::msr_bitmap,  pa_from_va(msr_bitmap));
    err |= vmx::write(vmx::vmcs::ept_pointer, ept_t::get().ept_pointer().flags);
    err |= vmx::write(vmx::vmcs::vmcs_link_pointer, ~0ull);

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

    uint8_t err{};

    err |= vmx::write(vmx::vmcs::guest_es_selector,   es.selector.flags);
	err |= vmx::write(vmx::vmcs::guest_cs_selector,   cs.selector.flags);
	err |= vmx::write(vmx::vmcs::guest_ss_selector,   ss.selector.flags);
	err |= vmx::write(vmx::vmcs::guest_ds_selector,   ds.selector.flags);
	err |= vmx::write(vmx::vmcs::guest_fs_selector,   fs.selector.flags);
	err |= vmx::write(vmx::vmcs::guest_gs_selector,   gs.selector.flags);
	err |= vmx::write(vmx::vmcs::guest_tr_selector,   tr.selector.flags);
    err |= vmx::write(vmx::vmcs::guest_ldtr_selector, ldtr.selector.flags);
    err |= vmx::write(vmx::vmcs::guest_ia32_debugctl, read<msr::debugctl>().flags);

    err |= vmx::write(vmx::vmcs::guest_es_limit,   es.limit);
    err |= vmx::write(vmx::vmcs::guest_cs_limit,   cs.limit);
    err |= vmx::write(vmx::vmcs::guest_ss_limit,   ss.limit);
    err |= vmx::write(vmx::vmcs::guest_ds_limit,   ds.limit);
    err |= vmx::write(vmx::vmcs::guest_fs_limit,   fs.limit);
    err |= vmx::write(vmx::vmcs::guest_gs_limit,   gs.limit);
    err |= vmx::write(vmx::vmcs::guest_tr_limit,   tr.limit);
    err |= vmx::write(vmx::vmcs::guest_ldtr_limit, ldtr.limit);
    err |= vmx::write(vmx::vmcs::guest_gdtr_limit, gdtr.limit);
    err |= vmx::write(vmx::vmcs::guest_idtr_limit, idtr.limit);

    err |= vmx::write(vmx::vmcs::guest_es_ar_bytes,   es.rights.flags);
    err |= vmx::write(vmx::vmcs::guest_cs_ar_bytes,   cs.rights.flags);
    err |= vmx::write(vmx::vmcs::guest_ss_ar_bytes,   ss.rights.flags);
    err |= vmx::write(vmx::vmcs::guest_ds_ar_bytes,   ds.rights.flags);
    err |= vmx::write(vmx::vmcs::guest_fs_ar_bytes,   fs.rights.flags);
    err |= vmx::write(vmx::vmcs::guest_gs_ar_bytes,   gs.rights.flags);
    err |= vmx::write(vmx::vmcs::guest_tr_ar_bytes,   tr.rights.flags);
    err |= vmx::write(vmx::vmcs::guest_ldtr_ar_bytes, ldtr.rights.flags);

    err |= vmx::write(vmx::vmcs::guest_sysenter_cs, read<msr::sysenter_cs>().flags);

    err |= vmx::write(vmx::vmcs::cr0_guest_host_mask, cr0_mask.flags);
    err |= vmx::write(vmx::vmcs::cr4_guest_host_mask, cr4_mask.flags);

    err |= vmx::write(vmx::vmcs::cr0_read_shadow, read<cr0_t>().flags);
    err |= vmx::write(vmx::vmcs::cr4_read_shadow, read<cr4_t>().flags);

    err |= vmx::write(vmx::vmcs::guest_cr0, read<cr0_t>().flags);
    err |= vmx::write(vmx::vmcs::guest_cr3, read<cr3_t>().flags);
    err |= vmx::write(vmx::vmcs::guest_cr4, read<cr4_t>().flags);

    err |= vmx::write(vmx::vmcs::guest_es_base,   es.base);
    err |= vmx::write(vmx::vmcs::guest_cs_base,   cs.base);
    err |= vmx::write(vmx::vmcs::guest_ss_base,   ss.base);
    err |= vmx::write(vmx::vmcs::guest_ds_base,   ds.base);
    err |= vmx::write(vmx::vmcs::guest_fs_base,   fs.base);
    err |= vmx::write(vmx::vmcs::guest_gs_base,   gs.base);
    err |= vmx::write(vmx::vmcs::guest_tr_base,   tr.base);
    err |= vmx::write(vmx::vmcs::guest_ldtr_base, ldtr.base);
    err |= vmx::write(vmx::vmcs::guest_gdtr_base, gdtr.base);
    err |= vmx::write(vmx::vmcs::guest_idtr_base, idtr.base);

    err |= vmx::write(vmx::vmcs::guest_dr7, read<dr7_t>().flags);

    err |= vmx::write(vmx::vmcs::guest_sysenter_esp, read<msr::sysenter_esp>().flags);
    err |= vmx::write(vmx::vmcs::guest_sysenter_eip, read<msr::sysenter_eip>().flags);

    return err == 0;
}
