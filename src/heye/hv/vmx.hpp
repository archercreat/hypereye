#pragma once

#include "heye/arch/arch.hpp"

#include "vmcall.hpp"

namespace heye::vmx
{
bool vmcall(vmcall_reason reason, void* a1 = nullptr, void* a2 = nullptr, void* a3 = nullptr);

enum status_t : uint8_t
{
    success   = 0,
    failed_ex = 1,
    failed    = 2
};

[[nodiscard]] status_t on(uint64_t pa);
[[nodiscard]] status_t off();
[[nodiscard]] status_t clear(uint64_t pa);
[[nodiscard]] status_t vmptrld(uint64_t pa);
[[nodiscard]] status_t launch();

template<typename T>
T adjust(T value)
{
    static_assert(std::is_same_v<T, cr0_t>
        || std::is_same_v<T, cr4_t>
        || std::is_same_v<T, msr::vmx_exit_controls>
        || std::is_same_v<T, msr::vmx_entry_controls>
        || std::is_same_v<T, msr::vmx_pinbased_controls>
        || std::is_same_v<T, msr::vmx_procbased_controls>
        || std::is_same_v<T, msr::vmx_procbased_controls2>);

    uint64_t mask{};
    const auto true_controls = heye::read<msr::vmx_basic>().true_controls;
    if constexpr (std::is_same_v<T, msr::vmx_entry_controls>)
    {
        mask = true_controls ? heye::read<msr::vmx_true_entry_controls>().flags : heye::read<msr::vmx_entry_controls>().flags;
        value.flags &= mask >> 32;
        value.flags |= mask & 0xffffffff;
    }
    else if constexpr (std::is_same_v<T, msr::vmx_exit_controls>)
    {
        mask = true_controls ? heye::read<msr::vmx_true_exit_controls>().flags : heye::read<msr::vmx_exit_controls>().flags;
        value.flags &= mask >> 32;
        value.flags |= mask & 0xffffffff;
    }
    else if constexpr (std::is_same_v<T, msr::vmx_pinbased_controls>)
    {
        mask = true_controls ? heye::read<msr::vmx_true_pinbased_controls>().flags : heye::read<msr::vmx_pinbased_controls>().flags;
        value.flags &= mask >> 32;
        value.flags |= mask & 0xffffffff;
    }
    else if constexpr (std::is_same_v<T, msr::vmx_procbased_controls>)
    {
        mask = true_controls ? heye::read<msr::vmx_true_procbased_controls>().flags : heye::read<msr::vmx_procbased_controls>().flags;
        value.flags &= mask >> 32;
        value.flags |= mask & 0xffffffff;
    }
    else if constexpr (std::is_same_v<T, msr::vmx_procbased_controls2>)
    {
        mask = heye::read<msr::vmx_procbased_controls2>().flags;
        value.flags &= mask >> 32;
        value.flags |= mask & 0xffffffff;
    }
    else if constexpr (std::is_same_v<T, cr0_t>)
    {
        value.flags |= heye::read<msr::vmx_cr0_fixed0>().flags;
        value.flags &= heye::read<msr::vmx_cr0_fixed1>().flags;
    }
    else if constexpr (std::is_same_v<T, cr4_t>)
    {
        value.flags |= heye::read<msr::vmx_cr4_fixed0>().flags;
        value.flags &= heye::read<msr::vmx_cr4_fixed1>().flags;
    }
    return value;
}

void inject_exception(vm_interrupt_info_t interrupt);
void inject_exception(exception_t vector, interrupt_t type);
void inject_exception(exception_t vector, interrupt_t type, uint32_t code);
void inject_bp();
void inject_gp();
void inject_ud();
};
