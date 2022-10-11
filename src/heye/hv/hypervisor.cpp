#include "heye/hv/hypervisor.hpp"
#include "heye/hv/vmx.hpp"

#include "heye/shared/std/new.hpp"
#include "heye/shared/asserts.hpp"
#include "heye/shared/trace.hpp"
#include "heye/shared/cpu.hpp"
#include "heye/arch/arch.hpp"

hypervisor::hypervisor()
    : vcpu(new vcpu_t[cpu::count()]), state(state_t::off), kernel_page_table(read<cr3_t>())
{
}

hypervisor::~hypervisor()
{
    delete[] vcpu;
}

bool hypervisor::supported()
{
    const auto features  = read<cpuid::processor_features>();
    const auto controls  = read<msr::feature_control>();
    const auto ept_vpid  = read<msr::vmx_ept_vpid_cap>();
    const auto mtrr_type = read<msr::mtrr_def_type>();

    return features.vmx
        && controls.vmxon
        && ept_vpid.page_walk_length_4
        && ept_vpid.memory_type_wb
        && ept_vpid.invept
        && ept_vpid.invept_single_context
        && ept_vpid.invept_all_contexts
        && ept_vpid.invvpid
        && ept_vpid.invvpid_indiv_addr
        && ept_vpid.invvpid_single_context
        && ept_vpid.invvpid_single_context_retain_globals
        && ept_vpid.invvpid_all_contexts
        && mtrr_type.mtrr_enable;
}

bool hypervisor::enter()
{
    if (state == state_t::on || !supported())
        return false;

    bool failed_setup{ false };

    cpu::for_each([&, this](uint64_t cpu_number)
    {
        if (vcpu[cpu_number].is_off())
        {
            if (!vcpu[cpu_number].enter())
            {
                logger::info("Failed to virtualize %ld core.", cpu_number);
                failed_setup |= true;
            }
        }
    });

    if (failed_setup)
    {
        logger::info("hypervisor::enter() failed.");
        leave();
        return false;
    }

    state = state_t::on;
    return true;
}

bool hypervisor::leave()
{
    cpu::for_each([this](uint64_t cpu_number)
    {
        vcpu[cpu_number].leave();
    });

    state = state_t::off;
    return true;
}

bool hypervisor::is_running() const
{
    return state == state_t::on;
}

cr3_t hypervisor::system_process_pagetable() const
{
    return kernel_page_table;
}

void hypervisor::ping() const
{
    cpu::for_each([](uint64_t)
    {
        vmx::vmcall(vmcall_reason::ping);
    });
}
