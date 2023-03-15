#pragma once

#include <cstdint>

namespace heye
{
enum exception_t
{
    devide_error                = 0,
    debug_breakpoint            = 1,
    nmi                         = 2,
    breakpoint                  = 3,
    overflow                    = 4,
    bound_range_exceeded        = 5,
    undefined_opcode            = 6,
    no_math_coprocessor         = 7,
    double_fault                = 8,
    reserved0                   = 9,
    invalid_task_segment        = 10,
    segment_not_present         = 11,
    stack_segment_fault         = 12,
    general_protection_fault    = 13,
    page_fault                  = 14,
    reserved1                   = 15,
    math_fault                  = 16,
    alignment_check             = 17,
    machine_check               = 18,
    simd_floating_numeric_error = 19,
    virtual_exception           = 20,
    reserved2                   = 21,
    reserved3                   = 22,
    reserved4                   = 23,
    reserved5                   = 24,
    reserved6                   = 25,
    reserved7                   = 26,
    reserved8                   = 27,
    reserved9                   = 28,
    reserved10                  = 29,
    reserved11                  = 30,
    reserved12                  = 31,
};
};
