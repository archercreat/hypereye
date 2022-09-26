#pragma once

#include "std/callable.hpp"
#include <cstdint>

namespace cpu
{
struct alignas(8) context_t
{
    union
    {
        uint64_t rax;
        uint32_t eax;
    };

    union
    {
        uint64_t rbx;
        uint32_t ebx;
    };

    union
    {
        uint64_t rcx;
        uint32_t ecx;
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
    uint64_t rflags;
};
static_assert(sizeof(context_t) == 17 * 8, "context_t size mismatch");

using core_cb = std::function<void(uint64_t)>;

/// @brief Get total number of cores.
///
uint64_t count();

/// @brief Get current cpu number.
///
uint64_t current();

void for_each(core_cb fn);
};
