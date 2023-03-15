#include "heye/hv/hypervisor.hpp"
#include "heye/hv/vmx.hpp"

#include "heye/shared/trace.hpp"
#include "heye/shared/cpu.hpp"
#include "heye/arch/arch.hpp"

namespace heye
{
hv_t::hv_t(setup_cb_t setup, teardown_cb_t teardown, vmexit_cb_t vmexit) : state(state_t::off), kernel_page_table(read<cr3_t>())
{
    // Although all `new` allocations come from `ExAllocatePoolZero`, we zero initialize vcpu array.
    //
    __stosb(reinterpret_cast<unsigned char*>(vcpu), 0, sizeof(vcpu));
    // Allocate and initialize vcpu.
    //
    for (size_t core = 0; core < cpu::count(); core++)
    {
        vcpu[core] = new vcpu_t(this, setup, teardown, vmexit);
    }
    // Allocate and initialize ept.
    //
    ept = new ept_t;
}

hv_t::~hv_t()
{
    stop();
    // Delete all vcpu instances.
    //
    for (auto core : vcpu)
    {
        if (core != nullptr)
        {
            delete core;
        }
    }
    delete ept;
}

bool hv_t::supported()
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

bool hv_t::start()
{
    if (is_running() || !supported())
        return false;

    volatile bool failed_start{ false };
    // Enter VMM on all cores. This function runs at IPI_LEVEL.
    //
    cpu::for_each([&, this](uint64_t cpu_number)
    {
        auto current_vcpu = vcpu[cpu_number];
        if (current_vcpu != nullptr && current_vcpu->is_off())
        {
            if (!current_vcpu->start())
            {
                logger::info("Failed to virtualize %ld core", cpu_number);
                failed_start = true;
            }
        }
        else
        {
            failed_start = true;
        }
    });

    _mm_mfence();

    if (failed_start)
    {
        logger::info("Failed to start hypervisor");
        stop();
        return false;
    }

    state = state_t::on;
    return true;
}

void hv_t::stop()
{
    // Leave vmm on all cores.
    //
    cpu::for_each([this](uint64_t cpu_number)
    {
        if (vcpu[cpu_number] != nullptr)
        {
            vcpu[cpu_number]->stop();
        }
    });
    // Mark state as off.
    //
    state = state_t::off;
}

bool hv_t::is_running() const
{
    return state == state_t::on;
}

cr3_t hv_t::system_process_pagetable() const
{
    return kernel_page_table;
}
};
