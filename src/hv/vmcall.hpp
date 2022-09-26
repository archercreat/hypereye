#pragma once

#include <cstdint>

namespace cpu
{
struct context_t;
};

enum class vmcall_reason : uint64_t
{
    /// @brief For debugging.
    ///
    hello  = 0,
    /// @brief Turn of hypervisor.
    ///
    vmxoff = 1,
};

/// @brief VMCALL abi:
/// rcx - vmcall_reason;
/// rdx - arbitrary context.
///
bool handle_vmcall(cpu::context_t* context);
