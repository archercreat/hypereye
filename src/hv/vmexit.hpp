#pragma once

#include <cstdint>

namespace cpu
{
struct context_t;
};

namespace vmx
{
enum class exit_reason
{
    exception_nmi                = 0,
    external_interrupt           = 1,
    triple_fault                 = 2,
    init                         = 3,
    sipi                         = 4,
    io_smi                       = 5,
    other_smi                    = 6,
    pending_virt_intr            = 7,
    pending_virt_nmi             = 8,
    task_switch                  = 9,
    cpuid                        = 10,
    getsec                       = 11,
    hlt                          = 12,
    invd                         = 13,
    invlpg                       = 14,
    rdpmc                        = 15,
    rdtsc                        = 16,
    rsm                          = 17,
    vmcall                       = 18,
    vmclear                      = 19,
    vmlaunch                     = 20,
    vmptrld                      = 21,
    vmptrst                      = 22,
    vmread                       = 23,
    vmresume                     = 24,
    vmwrite                      = 25,
    vmxoff                       = 26,
    vmxon                        = 27,
    cr_access                    = 28,
    dr_access                    = 29,
    io_instruction               = 30,
    msr_read                     = 31,
    msr_write                    = 32,
    invalid_guest_state          = 33,
    msr_loading                  = 34,
    mwait_instruction            = 36,
    monitor_trap_flag            = 37,
    monitor_instruction          = 39,
    pause_instruction            = 40,
    mce_during_vmentry           = 41,
    tpr_below_threshold          = 43,
    apic_access                  = 44,
    access_gdtr_or_idtr          = 46,
    access_ldtr_or_tr            = 47,
    ept_violation                = 48,
    ept_misconfig                = 49,
    invept                       = 50,
    rdtscp                       = 51,
    vmx_preemption_timer_expired = 52,
    invvpid                      = 53,
    wbinvd                       = 54,
    xsetbv                       = 55,
    apic_write                   = 56,
    rdrand                       = 57,
    invpcid                      = 58,
    rdseed                       = 61,
    pml_full                     = 62,
    xsaves                       = 63,
    xrstors                      = 64,
    pcommit                      = 65,
};
};

extern "C" bool vmexit_handler(cpu::context_t* context);
