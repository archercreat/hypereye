#pragma once

#include "ept.hpp"
#include "vcpu.hpp"

#include "heye/arch/cr.hpp"

namespace heye
{
struct hv_t
{
    hv_t ();
    ~hv_t();

    bool start();
    void stop();

    bool is_running() const;

    /// @brief Get system process cr3 value.
    ///
    cr3_t system_process_pagetable() const;

    /// @brief Check for vmx support.
    ///
    static bool supported();

    /// @brief Virtual machines per core.
    ///
    vcpu_t* vcpu[MAX_CPU_COUNT];

    /// @brief Global EPT pointer used by all vcpus.
    ///
    ept_t* ept;

private:
    /// @brief Hypervisor running state.
    ///
    state_t state;

    /// @brief CR3 value of the system process.
    /// Used as `host cr3` value in vmcs. Initialized during class construction.
    ///
    cr3_t kernel_page_table;
};
};
