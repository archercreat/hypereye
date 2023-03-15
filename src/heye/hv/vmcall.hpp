#pragma once

#include <cstdint>

namespace heye
{
namespace cpu
{
struct regs_t;
};

enum class vmcall_reason : uint64_t
{
    /// Smoke test.
    ///
    ping   = 0,
    /// Turn of hypervisor.
    ///
    vmxoff = 1,
};

struct vcpu_t;

bool handle_vmcall(vcpu_t*);
};
