#pragma once

#include "std/callable.hpp"
#include <cstdint>

namespace heye::cpu
{
struct uint128_t
{
    uint64_t low;
    uint64_t high;
};
static_assert(sizeof(uint128_t) == 16);

struct alignas(8) regs_t
{
    union
    {
        uint64_t gpr[16];
        struct
        {
            union
            {
                uint64_t rax;
                uint32_t eax;
            };
            union
            {
                uint64_t rcx;
                uint32_t ecx;
            };
            union
            {
                uint64_t rbx;
                uint32_t ebx;
            };
            uint64_t rdx;
            uint64_t rsp;
            uint64_t rbp;
            uint64_t rsi;
            uint64_t rdi;
            uint64_t r8;
            uint64_t r9;
            uint64_t r10;
            uint64_t r11;
            uint64_t r12;
            uint64_t r13;
            uint64_t r14;
            uint64_t r15;
        };
    };
    uint64_t  rip;
    uint64_t  rflags;
    uint128_t xmm0;
    uint128_t xmm1;
    uint128_t xmm2;
    uint128_t xmm3;
    uint128_t xmm4;
    uint128_t xmm5;
    uint128_t xmm6;
    uint128_t xmm7;
    uint128_t xmm8;
    uint128_t xmm9;
    uint128_t xmm10;
    uint128_t xmm11;
    uint128_t xmm12;
    uint128_t xmm13;
    uint128_t xmm14;
    uint128_t xmm15;
};
static_assert(sizeof(regs_t) == 18 * 8 + 16 * 16, "regs_t size mismatch");

using core_cb = std::function<void(uint64_t)>;

/// Get total number of cores.
///
uint64_t count();

/// Get current cpu number.
///
uint64_t current();

/// Run IPI routine on each core.
///
void for_each(core_cb fn);
};
