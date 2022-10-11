#pragma once
#include "heye/hv/vmcs.hpp"
#include "heye/config.hpp"
#include "heye/arch/arch.hpp"

namespace cpu
{
struct context_t;
};

struct ept_t;

enum class state_t : uint32_t
{
    /// @brief Hypervisor is active.
    ///
    on,
    /// @brief Hypervisor is disabled.
    ///
    off,
    /// @brief Hypervisor is initializing.
    ///
    init
};

/// @brief Virtual machine context.
///
struct vcpu_t
{
    vcpu_t();
    ~vcpu_t();

    /// @brief
    ///
    bool enter();

    /// @brief
    ///
    void leave();

    bool is_on()   const { return state == state_t::on;   }
    bool is_off()  const { return state == state_t::off;  }
    bool is_init() const { return state == state_t::init; }

private:
    vmx::vmcs_t* vmcs;
    vmx::vmcs_t* vmxon;
    uint8_t*     stack;
    uint8_t*     msr_bitmap;
    uint8_t*     io_bitmap_a;
    uint8_t*     io_bitmap_b;

    /// @brief VCPU running state.
    ///
    state_t state;

    bool setup_guest();
    bool setup_host();
    bool setup_controls();
};
