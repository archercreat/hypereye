#pragma once
#include "heye/shared/std/utility.hpp"
#include "heye/shared/std/unique.hpp"

#include "ept.hpp"
#include "vcpu.hpp"

#include "heye/arch/cr.hpp"

struct hypervisor : public std::singleton<hypervisor>
{
    friend struct std::singleton<hypervisor>;

    bool enter();
    bool leave();

    bool is_running() const;

    /// @brief Get system process cr3 value.
    ///
    cr3_t system_process_pagetable() const;

    void ping() const;

    /// @brief Check for vmx support.
    ///
    static bool supported();

    /// @brief Virtual machines per core.
    ///
    vcpu_t* vcpu;

protected:
    hypervisor();
    ~hypervisor();

private:
    /// @brief Hypervisor running state.
    ///
    state_t state;

    /// @brief CR3 value of the system process.
    /// Used as `host cr3` value in vmcs. Initialized during class construction.
    ///
    cr3_t kernel_page_table;
};
