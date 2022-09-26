#include "hypervisor.hpp"
#include "vcpu.hpp"
#include "vmx.hpp"

#include "arch/arch.hpp"

#include "shared/asserts.hpp"
#include "shared/trace.hpp"
#include "shared/cpu.hpp"

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

bool virtualize(vcpu_t* vcpu, const cpu::context_t* ctx)
{
    return vcpu->enter(ctx);
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
    if (is_off())
    {
        if (vminit_stub(virtualize, this))
        {
            // We are running in guest mode. Mark state as `on`.
            //
            state = state_t::on;
        }
    }
    return is_on();
}

bool vcpu_t::enter(const cpu::context_t* ctx)
{
    if (!is_off())
        return false;

    auto cr0 = read<cr0_t>();
    auto cr4 = read<cr4_t>();
    // Enable vmx.
    //
    cr4.vmxe = true;
    write<cr4_t>(cr4);
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
    if (vmx::on(vmxon->pa()))
    {
        logger::info("__vmxon failed");
        return false;
    }
    // Mark as `init`.
    //
    state = state_t::init;

    vmx::invvpid_all_contexts();

    vmx::invept_all_contexts();

    if (vmx::clear(vmcs->pa()) || vmx::vmptrld(vmcs->pa()))
    {
        logger::info("__vmx_clear || __vmx_vmptrld failed");
        return false;
    }

    if (!setup_vmcs(ctx))
    {
        logger::info("Failed to setup vmcs");
        return false;
    }

    vmx::launch();
    // Should never be here if all goes well.
    //
    logger::info("Failed to launch with the code: 0x%lx", vmx::read(vmx::vmcs::vm_instruction_error));
    return false;
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
    __stosb(reinterpret_cast<unsigned char*>(vmxon),       0, sizeof(vmx::vmcs_t));
    __stosb(reinterpret_cast<unsigned char*>(vmcs),        0, sizeof(vmx::vmcs_t));
    __stosb(reinterpret_cast<unsigned char*>(msr_bitmap),  0, sizeof(PAGE_SIZE));
    __stosb(reinterpret_cast<unsigned char*>(stack),       0, sizeof(stack));
    __stosb(reinterpret_cast<unsigned char*>(io_bitmap_a), 0, sizeof(io_bitmap_a));
    __stosb(reinterpret_cast<unsigned char*>(io_bitmap_b), 0, sizeof(io_bitmap_b));
    // Mark state as `off`.
    //
    state = state_t::off;
}

bool vcpu_t::setup_vmcs(const cpu::context_t* ctx)
{
    const auto es   = read<es_t>();
    const auto cs   = read<cs_t>();
    const auto ss   = read<ss_t>();
    const auto ds   = read<ds_t>();
    const auto fs   = read<fs_t>();
    const auto gs   = read<gs_t>();
    const auto tr   = read<tr_t>();
    const auto ldtr = read<ldtr_t>();
    const auto gdtr = read<gdtr_t>();
    const auto idtr = read<idtr_t>();

    const auto dr7  = read<dr7_t>();
    const auto cr0  = read<cr0_t>();
    const auto cr3  = read<cr3_t>();
    const auto cr4  = read<cr4_t>();

    const auto sysenter_cs  = read<msr::sysenter_cs>();
    const auto sysenter_eip = read<msr::sysenter_eip>();
    const auto sysenter_esp = read<msr::sysenter_esp>();
    const auto debugctl     = read<msr::debugctl>();
    const auto vmx_basic    = read<msr::vmx_basic>();

    cr0_t cr0_mask{};
    cr3_t cr3_mask{};
    cr4_t cr4_mask{};

    uint8_t err{};

    err |= vmx::write(vmx::vmcs::virtual_processor_id, 1);

    err |= vmx::write(vmx::vmcs::guest_es_selector,   es.selector);
	err |= vmx::write(vmx::vmcs::guest_cs_selector,   cs.selector);
	err |= vmx::write(vmx::vmcs::guest_ss_selector,   ss.selector);
	err |= vmx::write(vmx::vmcs::guest_ds_selector,   ds.selector);
	err |= vmx::write(vmx::vmcs::guest_fs_selector,   fs.selector);
	err |= vmx::write(vmx::vmcs::guest_gs_selector,   gs.selector);
	err |= vmx::write(vmx::vmcs::guest_tr_selector,   tr.selector);
    err |= vmx::write(vmx::vmcs::guest_ldtr_selector, ldtr.selector);

    err |= vmx::write(vmx::vmcs::host_es_selector, es.selector & 0xf8);
	err |= vmx::write(vmx::vmcs::host_cs_selector, cs.selector & 0xf8);
	err |= vmx::write(vmx::vmcs::host_ss_selector, ss.selector & 0xf8);
	err |= vmx::write(vmx::vmcs::host_ds_selector, ds.selector & 0xf8);
	err |= vmx::write(vmx::vmcs::host_fs_selector, fs.selector & 0xf8);
	err |= vmx::write(vmx::vmcs::host_gs_selector, gs.selector & 0xf8);
	err |= vmx::write(vmx::vmcs::host_tr_selector, tr.selector & 0xf8);

    err |= vmx::write(vmx::vmcs::msr_bitmap,  pa_from_va(msr_bitmap));
    err |= vmx::write(vmx::vmcs::ept_pointer, ept_t::get().ept_pointer().flags);

    err |= vmx::write(vmx::vmcs::vmcs_link_pointer,   ~0ull);
    err |= vmx::write(vmx::vmcs::guest_ia32_debugctl, debugctl.flags);

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

    err |= vmx::write(vmx::vmcs::guest_es_ar_bytes,   es.rights);
    err |= vmx::write(vmx::vmcs::guest_cs_ar_bytes,   cs.rights);
    err |= vmx::write(vmx::vmcs::guest_ss_ar_bytes,   ss.rights);
    err |= vmx::write(vmx::vmcs::guest_ds_ar_bytes,   ds.rights);
    err |= vmx::write(vmx::vmcs::guest_fs_ar_bytes,   fs.rights);
    err |= vmx::write(vmx::vmcs::guest_gs_ar_bytes,   gs.rights);
    err |= vmx::write(vmx::vmcs::guest_tr_ar_bytes,   tr.rights);
    err |= vmx::write(vmx::vmcs::guest_ldtr_ar_bytes, ldtr.rights);

    err |= vmx::write(vmx::vmcs::guest_sysenter_cs, sysenter_cs.flags);
    err |= vmx::write(vmx::vmcs::host_sysenter_cs,  sysenter_cs.flags);

    err |= vmx::write(vmx::vmcs::cr0_guest_host_mask, cr0_mask.flags);
    err |= vmx::write(vmx::vmcs::cr4_guest_host_mask, cr4_mask.flags);

    err |= vmx::write(vmx::vmcs::cr0_read_shadow, cr0.flags);
    err |= vmx::write(vmx::vmcs::cr4_read_shadow, cr4.flags);

    err |= vmx::write(vmx::vmcs::guest_cr0, cr0.flags);
    err |= vmx::write(vmx::vmcs::guest_cr3, cr3.flags);
    err |= vmx::write(vmx::vmcs::guest_cr4, cr4.flags);

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

    err |= vmx::write(vmx::vmcs::guest_dr7, dr7.flags);

    err |= vmx::write(vmx::vmcs::guest_rsp,    ctx->rsp);
    err |= vmx::write(vmx::vmcs::guest_rip,    reinterpret_cast<uintptr_t>(vmresume_stub));
    err |= vmx::write(vmx::vmcs::guest_rflags, ctx->rflags);

    err |= vmx::write(vmx::vmcs::guest_sysenter_esp, sysenter_esp.flags);
    err |= vmx::write(vmx::vmcs::guest_sysenter_eip, sysenter_eip.flags);

    err |= vmx::write(vmx::vmcs::host_cr0, cr0.flags);
    // Host cr3 is taken from hypervisor intance.
    //
    err |= vmx::write(vmx::vmcs::host_cr3, hypervisor::get().system_process_pagetable().flags);
    err |= vmx::write(vmx::vmcs::host_cr4, cr4.flags);

    err |= vmx::write(vmx::vmcs::host_fs_base,   fs.base);
    err |= vmx::write(vmx::vmcs::host_gs_base,   gs.base);
    err |= vmx::write(vmx::vmcs::host_tr_base,   tr.base);
    err |= vmx::write(vmx::vmcs::host_idtr_base, idtr.base);
    err |= vmx::write(vmx::vmcs::host_gdtr_base, gdtr.base);

    err |= vmx::write(vmx::vmcs::host_sysenter_esp, sysenter_esp.flags);
    err |= vmx::write(vmx::vmcs::host_sysenter_eip, sysenter_eip.flags);

    err |= vmx::write(vmx::vmcs::host_rip, reinterpret_cast<uintptr_t>(vmexit_stub));
    err |= vmx::write(vmx::vmcs::host_rsp, reinterpret_cast<uintptr_t>(stack + KERNEL_STACK_SIZE - sizeof(void*)));

    return err == 0;
}
