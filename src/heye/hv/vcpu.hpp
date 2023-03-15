#pragma once
#include "vmx.hpp"
#include "callbacks.hpp"
#include "heye/config.hpp"
#include "heye/arch/arch.hpp"

#include "heye/shared/cpu.hpp"

namespace heye
{
struct hv_t;
struct ept_t;
struct vcpu_t;

struct stack_t
{
    struct frame_t
    {
        uint64_t rip;
        uint64_t cs;
        uint64_t rflags;
        uint64_t rsp;
        uint64_t ss;
    };

    uint8_t        data[kernel_stack_size
                            - sizeof(cpu::regs_t)
                            - sizeof(frame_t)
                            - sizeof(uint64_t) * 3];
    cpu::regs_t    regs;
    frame_t        frame;
    uint64_t       unused;
    vmexit_cb_t    vmexit_handler;
    vcpu_t*        vcpu;
};
static_assert(sizeof(stack_t) == kernel_stack_size);

enum class state_t : uint32_t
{
    /// Hypervisor is active.
    ///
    on,
    /// Hypervisor is disabled.
    ///
    off,
    /// Hypervisor is initializing.
    ///
    init
};

/// Virtual machine context.
///
struct vcpu_t
{
    vcpu_t(hv_t* owner, setup_cb_t setup, teardown_cb_t teardown, vmexit_cb_t vmexit);
    ~vcpu_t();

    /// Enter vmx non root.
    ///
    bool start();

    /// Leave vmx non root.
    ///
    void stop();

    bool is_on()   const { return state == state_t::on;   }
    bool is_off()  const { return state == state_t::off;  }
    bool is_init() const { return state == state_t::init; }

    void skip_instruction();

    uint64_t id() const;

    cpu::regs_t& regs();

    vmx::exit_reason          exit_reason()          const;
    vmx::exit_qualification_t exit_qualification()   const;
    vmx::vm_interrupt_info_t  exit_interrupt_info()  const;
    vmx::vm_interrupt_info_t  entry_interrupt_info() const;

private:
    bool setup_guest();
    bool setup_host();
    bool setup_controls();

    /// Hypervisor instance that owns this vcpu.
    ///
    hv_t* hv;

    /// VCPU running state.
    ///
    state_t state;

    /// User provided callbacks.
    ///
    setup_cb_t    setup_cb;
    teardown_cb_t teardown_cb;
    vmexit_cb_t   vmexit_cb;

    /// Per core global variables.
    ///
    vmx::vmcs_t*       vmcs;
    vmx::vmcs_t*       vmxon;
    vmx::io_bitmap_t*  io_bitmap;
    vmx::msr_bitmap_t* msr_bitmap;
    stack_t*           stack;
};
};
