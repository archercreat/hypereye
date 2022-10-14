#pragma once
#include "vmcs.hpp"
#include "heye/config.hpp"
#include "heye/arch/arch.hpp"

namespace heye
{
namespace cpu
{
struct context_t;
};


struct hv_t;
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
    vcpu_t(hv_t* owner);
    ~vcpu_t();

    /// @brief Enter vmx non root.
    ///
    bool start();

    /// @brief Leave vmx non root.
    ///
    void stop();

    bool is_on()   const { return state == state_t::on;   }
    bool is_off()  const { return state == state_t::off;  }
    bool is_init() const { return state == state_t::init; }

private:
    bool setup_guest();
    bool setup_host();
    bool setup_controls();

    /// @brief Hypervisor instance that owns this vcpu.
    ///
    hv_t* hv;

    /// @brief VCPU running state.
    ///
    state_t state;

    /// @brief Per core global variables.
    ///
    vmx::vmcs_t* vmcs;
    vmx::vmcs_t* vmxon;
    uint8_t*     stack;
    uint8_t*     msr_bitmap;
    uint8_t*     io_bitmap_a;
    uint8_t*     io_bitmap_b;
};
};
