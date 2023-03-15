#pragma once

#include "ept.hpp"
#include "vcpu.hpp"
#include "vmexit.hpp"

#include "heye/arch/cr.hpp"

namespace heye
{
struct hv_t
{
    hv_t (setup_cb_t setup, teardown_cb_t teardown, vmexit_cb_t vmexit);
    ~hv_t();

    bool start();
    void stop();

    bool is_running() const;

    /// Get system process cr3 value.
    ///
    cr3_t system_process_pagetable() const;

    /// Check for vmx support.
    ///
    static bool supported();

    /// Virtual machines per core.
    ///
    vcpu_t* vcpu[max_cpu_count];

    /// Global EPT pointer used by all vcpus.
    ///
    ept_t* ept;

private:
    /// Hypervisor running state.
    ///
    state_t state;

    /// CR3 value of the system process.
    /// Used as `host cr3` value in vmcs. Initialized during class construction.
    ///
    cr3_t kernel_page_table;
};
};
