#pragma once

#include <cstdint>

namespace heye::msr
{
struct feature_control
{
    static constexpr unsigned id = 0x3a;

    union
    {
        uint64_t flags;

        struct
        {
            /// When set, locks this MSR from being written; writes to this bit will result in GP(0).
            ///
            uint64_t lock               : 1;
            /// Enable VMX inside SMX operation.
            ///
            uint64_t smx_vmxon          : 1;
            /// Enable VMX outside SMX operation.
            ///
            uint64_t vmxon              : 1;
            /// @brief
            ///
            uint64_t _reserved1         : 6;
            /// When set, each bit in the field represents an enable control for a corresponding SENTER function.
            ///
            uint64_t senter_lf_enables  : 6;
            /// This bit must be set to enable SENTER leaf functions.
            ///
            uint64_t senter_enable      : 1;
            /// @brief
            ///
            uint64_t _reserved2         : 1;
            /// This bit must be set to enable runtime reconfiguration of SGX Launch Control via the IA32_SGXLEPUBKEYHASHn MSR.
            ///
            uint64_t sgx_lc_enable      : 1;
            /// This bit must be set to enable SGX leaf functions.
            ///
            uint64_t sgx_enable         : 1;
            /// @brief
            ///
            uint64_t _reserved3         : 1;
            /// When set, system software can program the MSRs associated with LMCE to configure delivery of some machine check exceptions to a single logical processor.
            ///
            uint64_t lmce_on            : 1;
        };
    };
};

struct mtrrcap
{
    static constexpr unsigned id = 0xfe;

    union
    {
        uint64_t flags;

        struct
        {
            /// Indicates the number of variable ranges implemented on the processor.
            ///
            uint64_t vnct            : 8;
            /// Fixed range MTRRs (IA32_MTRR_FIX64K_00000 through IA32_MTRR_FIX4K_0F8000) are supported when set.
            ///
            uint64_t fix             : 1;
            /// @brief
            ///
            uint64_t _reserved1      : 1;
            /// The write-combining (WC) memory type is supported when set.
            ///
            uint64_t write_combining : 1;
            /// The system-management range register interface is supported when bit 11 is set.
            ///
            uint64_t smrr            : 1;

        };
    };
};

struct mtrr_physbase
{
    static constexpr unsigned id = 0x200;

    union
    {
        uint64_t flags;

        struct
        {
            /// Specifies the memory type for the range.
            ///
            uint64_t type       : 8;
            /// @brief
            ///
            uint64_t _reserved1 : 4;
            /// @brief
            ///
            uint64_t pfn        : 36;
        };
    };
};

struct mtrr_physmask
{
    static constexpr unsigned id = 0x201;

    union
    {
        uint64_t flags;

        struct
        {
            /// @brief
            ///
            uint64_t _reserved1 : 11;
            /// Enables the register pair when set; disables register pair when clear.
            ///
            uint64_t valid      : 1;
            /// @brief
            ///
            uint64_t pfn        : 36;
        };
    };
};

struct mtrr_fix
{
    union
    {
        uint64_t flags;
        uint8_t  types[8];
    };
};

struct mtrr_fix_64k : mtrr_fix
{
    static constexpr unsigned id   = 0x250;
    static constexpr unsigned base = 0x00000;
    static constexpr unsigned size = 0x10000;
};

struct mtrr_fix_16k_0 : mtrr_fix
{
    static constexpr unsigned id   = 0x258;
    static constexpr unsigned base = 0x80000;
    static constexpr unsigned size = 0x04000;
};

struct mtrr_fix_16k_1 : mtrr_fix
{
    static constexpr unsigned id   = 0x259;
    static constexpr unsigned base = 0xa0000;
    static constexpr unsigned size = 0x04000;
};

struct mtrr_fix_4k_0 : mtrr_fix
{
    static constexpr unsigned id   = 0x268;
    static constexpr unsigned base = 0xc0000;
    static constexpr unsigned size = 0x01000;
};

struct mtrr_fix_4k_1 : mtrr_fix
{
    static constexpr unsigned id   = 0x269;
    static constexpr unsigned base = 0xc8000;
    static constexpr unsigned size = 0x01000;
};

struct mtrr_fix_4k_2 : mtrr_fix
{
    static constexpr unsigned id   = 0x26a;
    static constexpr unsigned base = 0xd0000;
    static constexpr unsigned size = 0x01000;
};

struct mtrr_fix_4k_3 : mtrr_fix
{
    static constexpr unsigned id   = 0x26b;
    static constexpr unsigned base = 0xd8000;
    static constexpr unsigned size = 0x01000;
};

struct mtrr_fix_4k_4 : mtrr_fix
{
    static constexpr unsigned id   = 0x26c;
    static constexpr unsigned base = 0xe0000;
    static constexpr unsigned size = 0x01000;
};

struct mtrr_fix_4k_5 : mtrr_fix
{
    static constexpr unsigned id   = 0x26d;
    static constexpr unsigned base = 0xe8000;
    static constexpr unsigned size = 0x01000;
};

struct mtrr_fix_4k_6 : mtrr_fix
{
    static constexpr unsigned id   = 0x26e;
    static constexpr unsigned base = 0xf0000;
    static constexpr unsigned size = 0x01000;
};

struct mtrr_fix_4k_7 : mtrr_fix
{
    static constexpr unsigned id   = 0x26f;
    static constexpr unsigned base = 0xf8000;
    static constexpr unsigned size = 0x01000;
};

struct mtrr_def_type
{
    static constexpr unsigned id = 0x2ff;

    union
    {
        uint64_t flags;

        struct
        {
            /// Default Memory Type.
            ///
            uint64_t memory_type             : 3;
            /// @brief
            ///
            uint64_t _reserved1              : 7;
            /// Fixed Range MTRR Enable.
            ///
            uint64_t fixed_range_mtrr_enable : 1;
            /// MTRR Enable.
            ///
            uint64_t mtrr_enable             : 1;
        };
    };
};

struct vmx_basic
{
    static constexpr unsigned id = 0x480;

    union
    {
        uint64_t flags;

        struct
        {
            /// VMCS revision identifier used by the processor.
            ///
            uint64_t vmcs_id        : 31;
            /// Bit 31 is always 0.
            ///
            uint64_t zero           : 1;
            /// Size of the VMCS.
            ///
            uint64_t vmcs_size      : 12;
            /// @brief
            ///
            uint64_t _reserved1     : 4;
            /// Width of physical address used for the VMCS.
            ///
            uint64_t phys_width     : 1;
            /// Whether the processor supports the dual-monitor treatment of system-management interrupts and system-management code (always 1).
            ///
            uint64_t dual_mon       : 1;
            /// Memory type that must be used for the VMCS.
            ///
            uint64_t mem_type       : 4;
            /// Whether the processor provides additional information for exits due to INS/OUTS.
            ///
            uint64_t ins_outs       : 1;
            /// Whether default 1 bits in control MSRs (pin/proc/exit/entry) may be cleared to 0 and that 'true' control MSRs are supported.
            ///
            uint64_t true_controls  : 1;
        };
    };
};

struct vmx_pinbased_controls
{
    static constexpr unsigned id = 0x481;

    union
    {
        uint64_t flags;

        struct
        {
            /// External interrupts cause VM-exits if set; otherwise dispatched through the guest's IDT.
            ///
            uint64_t external_interrupt_exiting : 1;
            /// @brief
            ///
            uint64_t _reserved1                 : 2;
            /// Non-maskable interrupts cause VM-exits if set; otherwise dispatched through the guest's IDT.
            ///
            uint64_t nmi_exiting                : 1;
            /// @brief
            ///
            uint64_t _reserved2                 : 1;
            /// If this control is 1, NMIs are never blocked.
            ///
            uint64_t virtual_nmi                : 1;
            /// If this control is 1, the VMX-preemption timer counts down in VMX non-root operation.
            /// A VM exit occurs when the timer counts down to zero.
            ///
            uint64_t preemption_timer           : 1;
            /// If this control is 1, the processor treats interrupts with the posted-interrupt notification vector
            /// specially, updating the virtual-APIC page with posted-interrupt requests.
            ///
            uint64_t posted_interrupts          : 1;
        };
    };
};

struct vmx_exit_controls
{
    static constexpr unsigned id = 0x483;

    union
    {
        uint64_t flags;

        struct
        {
            /// @brief
            ///
            uint64_t _reserved1                    : 2;
            /// This control determines whether DR7 and the IA32_DEBUGCTL MSR are saved on VM exit.
            ///
            uint64_t save_debug_controls           : 1;
            /// @brief
            ///
            uint64_t _reserved2                    : 6;
            /// On processors that support Intel 64 architecture, this control determines whether a logical
            /// processor is in 64-bit mode after the next VM exit.
            ///
            uint64_t host_address_space_size       : 1;
            /// @brief
            ///
            uint64_t _reserved3                    : 2;
            ///  Whether the IA32_PERF_GLOBAL_CTRL MSR is loaded on VM-exit.
            ///
            uint64_t load_perf_global_ctrl         : 1;
            /// @brief
            ///
            uint64_t _reserved4                    : 2;
            /// Acknowledge external interrupts with the irq controller if one caused a VM-exit.
            ///
            uint64_t acknowledge_interrupt_on_exit : 1;
            /// @brief
            ///
            uint64_t _reserved5                    : 2;
            /// This control determines whether the IA32_PAT MSR is saved on VM exit.
            ///
            uint64_t save_guest_pat_msr            : 1;
            /// This control determines whether the IA32_PAT MSR is loaded on VM exit.
            ///
            uint64_t load_host_pat_msr             : 1;
            /// This control determines whether the IA32_EFER MSR is saved on VM exit.
            ///
            uint64_t save_guest_efer_msr           : 1;
            /// This control determines whether the IA32_EFER MSR is loaded on VM exit.
            ///
            uint64_t load_host_efer_msr            : 1;
            /// This control determines whether the value of the VMX-preemption timer is saved on VM exit.
            ///
            uint64_t save_vmx_preempt_timer        : 1;
            /// This control determines whether the IA32_BNDCFGS MSR is cleared on VM exit.
            ///
            uint64_t clear_bndcfgs                 : 1;
            /// If this control is 1, Intel Processor Trace does not produce a paging information packet (PIP) on
            /// a VM exit or a VMCS packet on an SMM VM exit.
            ///
            uint64_t conceal_vmx_from_pt           : 1;
            /// This control determines whether the IA32_RTIT_CTL MSR is cleared on VM exit.
            ///
            uint64_t clear_rtit_ctl                : 1;
            /// This control determines whether the IA32_LBR_CTL MSR is cleared on VM exit.
            ///
            uint64_t clear_lbr_ctl                 : 1;
            /// @brief
            ///
            uint64_t _reserved6                    : 1;
            /// This control determines whether CET-related MSRs and SPP are loaded on VM exit.
            ///
            uint64_t load_host_cet_state           : 1;
            /// This control determines whether the IA32_LBR_CTL MSR is cleared on VM exit.
            ///
            uint64_t load_host_pkrs                : 1;
            /// @brief
            ///
            uint64_t _reserved7                    : 1;
            /// This control determines whether the secondary VM-exit controls are used.
            ///
            uint64_t use_secondary_controls        : 1;
        };
    };
};

struct vmx_entry_controls
{
    static constexpr unsigned id = 0x484;

    union
    {
        uint64_t flags;

        struct
        {
            /// @brief
            ///
            uint64_t _reserved1              : 2;
            /// This control determines whether DR7 and the IA32_DEBUGCTL MSR are loaded on VM entry.
            ///
            uint64_t load_debug_controls     : 1;
            /// @brief
            ///
            uint64_t _reserved2              : 6;
            /// On processors that support Intel 64 architecture, this control determines whether the logical
            /// processor is in IA-32e mode after VM entry.
            ///
            uint64_t ia32_mode_guest         : 1;
            /// This control determines whether the logical processor is in system-management mode (SMM) after VM entry.
            ///
            uint64_t entry_smm               : 1;
            ///  If set to 1, the default treatment of SMIs and SMM is in effect after the VM entry.
            ///
            uint64_t deactivate_dual_monitor : 1;
            /// @brief
            ///
            uint64_t _reserved3              : 1;
            /// This control determines whether the IA32_PERF_GLOBAL_CTRL MSR is loaded on VM entry.
            ///
            uint64_t load_guest_perf_msr     : 1;
            /// This control determines whether the IA32_PAT MSR is loaded on VM entry.
            ///
            uint64_t load_guest_pat_msr      : 1;
            /// This control determines whether the IA32_EFER MSR is loaded on VM entry.
            ///
            uint64_t load_guest_efer_msr     : 1;
            /// This control determines whether the IA32_BNDCFGS MSR is loaded on VM entry.
            ///
            uint64_t load_guest_bndcfgs      : 1;
            /// If this control is 1, Intel Processor Trace does not produce a paging information packet (PIP) on a VM entry or a VMCS packet on a VM entry that returns from SMM.
            ///
            uint64_t conceal_vmx_from_pt     : 1;
            /// This control determines whether the IA32_RTIT_CTL MSR is loaded on VM entry.
            ///
            uint64_t load_guest_rtit_ctl     : 1;
            /// @brief
            ///
            uint64_t _reserved4              : 1;
            /// This control determines whether CET-related MSRs and SPP are loaded on VM entry.
            ///
            uint64_t load_cet_state          : 1;
            /// This control determines whether the IA32_LBR_CTL MSR is loaded on VM entry.
            ///
            uint64_t load_guest_lbr_ctl      : 1;
            /// This control determines whether the IA32_PKRS MSR is loaded on VM entry.
            ///
            uint64_t load_guest_prks         : 1;
        };
    };
};

struct vmx_procbased_controls
{
    static constexpr unsigned id = 0x482;

    union
    {
        uint64_t flags;

        struct
        {
            /// @brief
            ///
            uint64_t _reserved1                 : 2;
            /// If this control is 1, a VM exit occurs at the beginning of any instruction if RFLAGS.IF = 1 and
            /// there are no other blocking of interrupts.
            ///
            uint64_t interrupt_window_exiting   : 1;
            /// This control determines whether executions of RDTSC, executions of RDTSCP, and executions
            /// of RDMSR that read from the IA32_TIME_STAMP_COUNTER MSR return a value modified by the TSC offset field.
            ///
            uint64_t use_tsc_offsetting         : 1;
            /// @brief
            ///
            uint64_t _reserved2                 : 3;
            /// This control determines whether executions of HLT cause VM exits.
            ///
            uint64_t hlt_exiting                : 1;
            /// @brief
            ///
            uint64_t _reserved3                 : 1;
            /// This control determines whether executions of INVLPG cause VM exits.
            ///
            uint64_t invlpg_exiting             : 1;
            /// This control determines whether executions of MWAIT cause VM exits.
            ///
            uint64_t mwait_exiting              : 1;
            /// This control determines whether executions of RDPMC cause VM exits.
            ///
            uint64_t rdpmc_exiting              : 1;
            /// This control determines whether executions of RDTSC and RDTSCP cause VM exits.
            ///
            uint64_t rtdsc_exiting              : 1;
            /// @brief
            ///
            uint64_t _reserved4                 : 2;
            /// VM-exit when executing the MOV to CR3 instruction.
            ///
            uint64_t cr3_load_exiting           : 1;
            /// VM-exit when executing the MOV from CR3 instruction.
            ///
            uint64_t cr3_store_exiting          : 1;
            /// This control determines whether the tertiary processor-based VM-execution controls are used.
            ///
            uint64_t use_tertiary_controls      : 1;
            /// @brief
            ///
            uint64_t _reserved5                 : 1;
            /// This control determines whether executions of MOV to CR8 cause VM exits.
            ///
            uint64_t cr8_load_exiting           : 1;
            /// This control determines whether executions of MOV from CR8 cause VM exits.
            ///
            uint64_t cr8_store_exiting          : 1;
            /// Setting this control to 1 enables TPR virtualization and other APIC-virtualization features.
            ///
            uint64_t use_tpr_shadow             : 1;
            /// If this control is 1, a VM exit occurs at the beginning of any instruction if there is no virtual-NMI blocking.
            ///
            uint64_t nmi_window_exiting         : 1;
            /// This control determines whether executions of MOV DR cause VM exits.
            ///
            uint64_t mov_dr_exiting             : 1;
            /// VM-exit when executing IO instructions.
            ///
            uint64_t unconditional_io_exiting   : 1;
            /// This control determines whether I/O bitmaps are used to restrict executions of I/O instructions.
            ///
            uint64_t use_io_bitmaps             : 1;
            /// @brief
            ///
            uint64_t _reserved6                 : 1;
            /// If this control is 1, the monitor trap flag debugging feature is enabled.
            ///
            uint64_t monitor_trap_flag          : 1;
            /// This control determines whether MSR bitmaps are used to control execution of the RDMSR and WRMSR instructions.
            ///
            uint64_t use_msr_bitmaps            : 1;
            /// This control determines whether executions of MONITOR cause VM exits.
            ///
            uint64_t monitor_exiting            : 1;
            /// This control determines whether executions of PAUSE cause VM exits.
            ///
            uint64_t pause_exiting              : 1;
            /// Determines whether the secondary processor based VM-execution controls are used.
            ///
            uint64_t use_secondary_controls     : 1;
        };
    };
};

struct vmx_procbased_controls2
{
    static constexpr unsigned id = 0x48b;

    union
    {
        uint64_t flags;

        struct
        {
            /// If this control is 1, the logical processor treats specially accesses to the page with the APICaccess address.
            ///
            uint64_t virtualize_apic                : 1;
            /// If this control is 1, extended page tables (EPT) are enabled.
            ///
            uint64_t enable_ept                     : 1;
            /// Descriptor table instructions cause VM-exits.
            ///
            uint64_t descriptor_table_exiting       : 1;
            /// If this control is 0, any execution of RDTSCP causes an invalid-opcode exception (#UD).
            ///
            uint64_t enable_rdtcp                   : 1;
            /// If this control is 1, the logical processor treats specially RDMSR and WRMSR to APIC MSRs.
            ///
            uint64_t virtualize_x2apic              : 1;
            /// If this control is 1, cached translations of linear addresses are associated with a VPID.
            ///
            uint64_t enable_vpid                    : 1;
            /// This control determines whether executions of WBINVD cause VM exits.
            ///
            uint64_t wbinvd_exiting                 : 1;
            /// This control determines whether guest software may run in unpaged protected mode or in realaddress mode.
            ///
            uint64_t unrestricted_guest             : 1;
            /// If this control is 1, the logical processor virtualizes certain APIC accesses.
            ///
            uint64_t apic_register_virtualization   : 1;
            /// This controls enables the evaluation and delivery of pending virtual interrupts as well as the
            /// emulation of writes to the APIC registers that control interrupt prioritization.
            ///
            uint64_t virtualize_interrupt_delivery  : 1;
            /// This control determines whether a series of executions of PAUSE can cause a VM exit.
            ///
            uint64_t pause_loop_exiting             : 1;
            /// This control determines whether executions of RDRAND cause VM exits.
            ///
            uint64_t rdrand_exiting                 : 1;
            /// If this control is 0, any execution of INVPCID causes a #UD.
            ///
            uint64_t enable_invpcid                 : 1;
            /// Setting this control to 1 enables use of the VMFUNC instruction in VMX non-root operation.
            ///
            uint64_t enable_vm_functions            : 1;
            /// If this control is 1, executions of VMREAD and VMWRITE in VMX non-root operation may access
            /// a shadow VMCS (instead of causing VM exits).
            ///
            uint64_t vmcs_shadowing                 : 1;
            /// If this control is 1, executions of ENCLS consult the ENCLS-exiting bitmap to determine whether
            /// the instruction causes a VM exit.
            ///
            uint64_t enable_encls_exiting           : 1;
            /// This control determines whether executions of RDSEED cause VM exits.
            ///
            uint64_t rdseed_exiting                 : 1;
            /// If this control is 1, an access to a guest-physical address that sets an EPT dirty bit first adds an
            /// entry to the page-modification log.
            ///
            uint64_t enable_pml                     : 1;
            ///  If this control is 1, EPT violations may cause virtualization exceptions (#VE) instead of VM exits.
            ///
            uint64_t ept_violation                  : 1;
            /// Conceal VMX non-root operation from Intel processor trace (PT).
            ///
            uint64_t conceal_vmx_from_pt            : 1;
            /// If this control is 0, any execution of XSAVES or XRSTORS causes a #UD.
            ///
            uint64_t enable_xsaves                  : 1;
            /// @brief
            ///
            uint64_t _reserved1                     : 1;
            /// If this control is 1, EPT execute permissions are based on whether the linear address being
            /// accessed is supervisor mode or user mode.
            ///
            uint64_t mb_exec_ctl_ept                : 1;
            /// If this control is 1, EPT write permissions may be specified at the granularity of 128 bytes.
            ///
            uint64_t sp_write_perm_ept              : 1;
            /// If this control is 1, all output addresses used by Intel Processor Trace are treated as guestphysical
            /// addresses and translated using EPT.
            ///
            uint64_t pt_guest_pa                    : 1;
            /// This control determines whether executions of RDTSC, executions of RDTSCP, and executions
            /// of RDMSR that read from the IA32_TIME_STAMP_COUNTER MSR return a value modified by the TSC multiplier field.
            ///
            uint64_t use_tsc_scaling                : 1;
            /// If this control is 0, any execution of TPAUSE, UMONITOR, or UMWAIT causes a #UD.
            ///
            uint64_t enable_user_wait_pause         : 1;
            /// @brief
            ///
            uint64_t _reserved2                     : 1;
            /// If this control is 1, executions of ENCLV consult the ENCLV-exiting bitmap to determine whether
            /// the instruction causes a VM exit.
            ///
            uint64_t enable_enclv_exiting           : 1;
        };
    };

};

struct vmx_true_pinbased_controls
{
    static constexpr unsigned id = 0x48d;

    uint64_t flags;
};

struct vmx_true_procbased_controls
{
    static constexpr unsigned id = 0x48e;

    uint64_t flags;
};

struct vmx_true_exit_controls
{
    static constexpr unsigned id = 0x48f;

    uint64_t flags;
};

struct vmx_true_entry_controls
{
    static constexpr unsigned id = 0x490;

    uint64_t flags;
};

struct vmx_cr0_fixed0
{
    static constexpr unsigned id = 0x486;

    uint64_t flags;
};

struct vmx_cr0_fixed1
{
    static constexpr unsigned id = 0x487;

    uint64_t flags;
};

struct vmx_cr4_fixed0
{
    static constexpr unsigned id = 0x488;

    uint64_t flags;
};

struct vmx_cr4_fixed1
{
    static constexpr unsigned id = 0x489;

    uint64_t flags;
};

struct vmx_ept_vpid_cap
{
    static constexpr unsigned id = 0x48c;

    union
    {
        uint64_t flags;

        struct
        {
            /// When set to 1, the processor supports execute-only translations by EPT.
            ///
            uint64_t rwx_x_only                             : 1;
            /// @brief
            ///
            uint64_t _reserved1                             : 5;
            /// Indicates support for a page-walk length of 4.
            ///
            uint64_t page_walk_length_4                     : 1;
            /// @brief
            ///
            uint64_t _reserved2                             : 1;
            /// When set to 1, the logical processor allows software to configure the EPT paging-structure memory type to be uncacheable (UC).
            ///
            uint64_t memory_type_uc                         : 1;
            /// @brief
            ///
            uint64_t _reserved3                             : 5;
            /// When set to 1, the logical processor allows software to configure the EPT paging-structure memory type to be write-back (WB).
            ///
            uint64_t memory_type_wb                         : 1;
            /// @brief
            ///
            uint64_t _reserved4                             : 1;
            /// When set to 1, the logical processor allows software to configure a EPT PDE to map a 2-Mbyte page.
            ///
            uint64_t pde_2m                                 : 1;
            /// When set to 1, the logical processor allows software to configure a EPT PDPTE to map a 1-Gbyte page.
            ///
            uint64_t pde_1g                                 : 1;
            /// @brief
            ///
            uint64_t _reserved5                             : 2;
            /// Support for the INVEPT instruction.
            ///
            uint64_t invept                                 : 1;
            /// When set to 1, accessed and dirty flags for EPT are supported.
            ///
            uint64_t ept_access_dirty                       : 1;
            /// When set to 1, the processor reports advanced VM-exit information for EPT violations.
            ///
            uint64_t advanced_vmexit_ept_info               : 1;
            /// When set to 1, supervisor shadow-stack control is supported.
            ///
            uint64_t supervisor_shadow_stack                : 1;
            /// @brief
            ///
            uint64_t _reserved6                             : 1;
            /// Support for the INVEPT instruction.
            ///
            uint64_t invept_single_context                  : 1;
            /// When set to 1, the all-context INVEPT type is supported.
            ///
            uint64_t invept_all_contexts                    : 1;
            /// @brief
            ///
            uint64_t _reserved7                             : 5;
            /// When set to 1, the INVVPID instruction is supported.
            ///
            uint64_t invvpid                                : 1;
            /// @brief
            ///
            uint64_t _reserved8                             : 7;
            /// When set to 1, the individual-address INVVPID type is supported.
            ///
            uint64_t invvpid_indiv_addr                     : 1;
            /// When set to 1, the single-context INVVPID type is supported.
            ///
            uint64_t invvpid_single_context                 : 1;
            /// When set to 1, the all-context INVVPID type is supported.
            ///
            uint64_t invvpid_all_contexts                   : 1;
            /// When set to 1, the single-context-retaining-globals INVVPID type is supported.
            ///
            uint64_t invvpid_single_context_retain_globals  : 1;
            /// @brief
            ///
            uint64_t _reserved9                             : 4;
            /// Enumerate the maximum HLAT prefix size.
            ///
            uint64_t max_hlat_prefix_size                   : 6;
        };
    };
};
static_assert(sizeof(vmx_ept_vpid_cap) == sizeof(uint64_t), "vmx_ept_vpid_cap size mismatch");

struct debugctl
{
    static constexpr unsigned id = 0x1d9;

    uint64_t flags;
};

struct sysenter_cs
{
    static constexpr unsigned id = 0x174;

    uint64_t flags;
};

struct sysenter_esp
{
    static constexpr unsigned id = 0x175;

    uint64_t flags;
};

struct sysenter_eip
{
    static constexpr unsigned id = 0x176;

    uint64_t flags;
};

struct fsbase
{
    static constexpr unsigned id = 0xC0000100;

    uint64_t flags;
};

struct gsbase
{
    static constexpr unsigned id = 0xC0000101;

    uint64_t flags;
};

struct gsbase_shadow
{
    static constexpr unsigned id = 0xC0000102;

    uint64_t flags;
};

};
