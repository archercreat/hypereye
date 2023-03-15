#pragma once
#include "../config.hpp"

#include <cstdint>

namespace heye::vmx
{
enum class vmcs : uint64_t
{
    virtual_processor_id          = 0x00000000,
    posted_interrupt_notification = 0x00000002,
    eptp_index                    = 0x00000004,
    /// 16-bit guest fields.
    ///
    guest_es_selector             = 0x00000800,
    guest_cs_selector             = 0x00000802,
    guest_ss_selector             = 0x00000804,
    guest_ds_selector             = 0x00000806,
    guest_fs_selector             = 0x00000808,
    guest_gs_selector             = 0x0000080a,
    guest_ldtr_selector           = 0x0000080c,
    guest_tr_selector             = 0x0000080e,
    guest_interrupt_status        = 0x00000810,
    pml_index                     = 0x00000812,
    /// 16-bit host fields.
    ///
    host_es_selector              = 0x00000c00,
    host_cs_selector              = 0x00000c02,
    host_ss_selector              = 0x00000c04,
    host_ds_selector              = 0x00000c06,
    host_fs_selector              = 0x00000c08,
    host_gs_selector              = 0x00000c0a,
    host_tr_selector              = 0x00000c0c,
    /// 64-bit control fields.
    ///
    io_bitmap_a                   = 0x00002000,
    io_bitmap_a_high              = 0x00002001,
    io_bitmap_b                   = 0x00002002,
    io_bitmap_b_high              = 0x00002003,
    msr_bitmap                    = 0x00002004,
    msr_bitmap_high               = 0x00002005,
    vm_exit_msr_store_addr        = 0x00002006,
    vm_exit_msr_store_addr_high   = 0x00002007,
    vm_exit_msr_load_addr         = 0x00002008,
    vm_exit_msr_load_addr_high    = 0x00002009,
    vm_entry_msr_load_addr        = 0x0000200a,
    vm_entry_msr_load_addr_high   = 0x0000200b,
    executive_vmcs_pointer        = 0x0000200c,
    executive_vmcs_pointer_high   = 0x0000200d,
    tsc_offset                    = 0x00002010,
    tsc_offset_high               = 0x00002011,
    virtual_apic_page_addr        = 0x00002012,
    virtual_apic_page_addr_high   = 0x00002013,
    apic_access_addr              = 0x00002014,
    apic_access_addr_high         = 0x00002015,
    vmfunc_controls               = 0x00002018,
    vmfunc_controls_high          = 0x00002019,
    ept_pointer                   = 0x0000201a,
    ept_pointer_high              = 0x0000201b,
    eptp_list                     = 0x00002024,
    eptp_list_high                = 0x00002025,
    /// 64-bit read-only fields.
    ///
    guest_physical_address        = 0x00002400,
    guest_physical_address_high   = 0x00002401,
    /// 64-bit guest fields.
    ///
    vmcs_link_pointer             = 0x00002800,
    vmcs_link_pointer_high        = 0x00002801,
    guest_ia32_debugctl           = 0x00002802,
    guest_ia32_debugctl_high      = 0x00002803,
    guest_ia32_pat                = 0x00002804,
    guest_ia32_pat_high           = 0x00002805,
    guest_ia32_efer               = 0x00002806,
    guest_ia32_efer_high          = 0x00002807,
    guest_ia32_perf_ctrl          = 0x00002808,
    guest_ia32_perf_ctrl_high     = 0x00002809,
    guest_pdptr0                  = 0x0000280a,
    guest_pdptr0_high             = 0x0000280b,
    guest_pdptr1                  = 0x0000280c,
    guest_pdptr1_high             = 0x0000280d,
    guest_pdptr2                  = 0x0000280e,
    guest_pdptr2_high             = 0x0000280f,
    guest_pdptr3                  = 0x00002810,
    guest_pdptr3_high             = 0x00002811,
    guest_ia32_bndcfgs            = 0x00002812,
    guest_ia32_bndcfgs_high       = 0x00002813,
    /// 64-bit host fields.
    ///
    host_ia32_pat                 = 0x00002c00,
    host_ia32_pat_high            = 0x00002c01,
    host_ia32_efer                = 0x00002c02,
    host_ia32_efer_high           = 0x00002c03,
    host_ia32_perf_ctrl           = 0x00002c04,
    host_ia32_perf_ctrl_high      = 0x00002c05,
    /// 32-bit control fields.
    ///
    pin_based_vm_exec_control     = 0x00004000,
    cpu_based_vm_exec_control     = 0x00004002,
    exception_bitmap              = 0x00004004,
    page_fault_error_code_mask    = 0x00004006,
    page_fault_error_code_match   = 0x00004008,
    cr3_target_count              = 0x0000400a,
    vm_exit_controls              = 0x0000400c,
    vm_exit_msr_store_count       = 0x0000400e,
    vm_exit_msr_load_count        = 0x00004010,
    vm_entry_controls             = 0x00004012,
    vm_entry_msr_load_count       = 0x00004014,
    vm_entry_intr_info            = 0x00004016,
    vm_entry_intr_error_code = 0x00004018,
    vm_entry_instruction_len      = 0x0000401a,
    tpr_threshold                 = 0x0000401c,
    secondary_vm_exec_control     = 0x0000401e,
    ple_gap                       = 0x00004020,
    ple_window                    = 0x00004022,
    /// 32-bit read-only fields.
    ///
    vm_instruction_error          = 0x00004400,
    vm_exit_reason                = 0x00004402,
    vm_exit_intr_info             = 0x00004404,
    vm_exit_intr_error_code       = 0x00004406,
    idt_vectoring_info_field      = 0x00004408,
    idt_vectoring_error_code      = 0x0000440a,
    vm_exit_instruction_len       = 0x0000440c,
    vmx_instruction_info          = 0x0000440e,
    /// 32-bit guest fields.
    ///
    guest_es_limit                = 0x00004800,
    guest_cs_limit                = 0x00004802,
    guest_ss_limit                = 0x00004804,
    guest_ds_limit                = 0x00004806,
    guest_fs_limit                = 0x00004808,
    guest_gs_limit                = 0x0000480a,
    guest_ldtr_limit              = 0x0000480c,
    guest_tr_limit                = 0x0000480e,
    guest_gdtr_limit              = 0x00004810,
    guest_idtr_limit              = 0x00004812,
    guest_es_ar_bytes             = 0x00004814,
    guest_cs_ar_bytes             = 0x00004816,
    guest_ss_ar_bytes             = 0x00004818,
    guest_ds_ar_bytes             = 0x0000481a,
    guest_fs_ar_bytes             = 0x0000481c,
    guest_gs_ar_bytes             = 0x0000481e,
    guest_ldtr_ar_bytes           = 0x00004820,
    guest_tr_ar_bytes             = 0x00004822,
    guest_interruptibility_info   = 0x00004824,
    guest_activity_state          = 0x00004826,
    guest_sm_base                 = 0x00004828,
    guest_sysenter_cs             = 0x0000482a,
    vmx_preemption_timer          = 0x000048ae,
    /// 32-bit host fields.
    ///
    host_sysenter_cs              = 0x00004c00,
    /// 64-bit control fields.
    ///
    cr0_guest_host_mask           = 0x00006000,
    cr4_guest_host_mask           = 0x00006002,
    cr0_read_shadow               = 0x00006004,
    cr4_read_shadow               = 0x00006006,
    cr3_target_value0             = 0x00006008,
    cr3_target_value1             = 0x0000600a,
    cr3_target_value2             = 0x0000600c,
    cr3_target_value3             = 0x0000600e,
    /// 64-bit read-only fields.
    ///
    exit_qualification            = 0x00006400,
    io_rcx                        = 0x00006402,
    io_rsi                        = 0x00006404,
    io_rdi                        = 0x00006406,
    io_rip                        = 0x00006408,
    guest_linear_address          = 0x0000640a,
    /// 64-bit guest fields.
    ///
    guest_cr0                     = 0x00006800,
    guest_cr3                     = 0x00006802,
    guest_cr4                     = 0x00006804,
    guest_es_base                 = 0x00006806,
    guest_cs_base                 = 0x00006808,
    guest_ss_base                 = 0x0000680a,
    guest_ds_base                 = 0x0000680c,
    guest_fs_base                 = 0x0000680e,
    guest_gs_base                 = 0x00006810,
    guest_ldtr_base               = 0x00006812,
    guest_tr_base                 = 0x00006814,
    guest_gdtr_base               = 0x00006816,
    guest_idtr_base               = 0x00006818,
    guest_dr7                     = 0x0000681a,
    guest_rsp                     = 0x0000681c,
    guest_rip                     = 0x0000681e,
    guest_rflags                  = 0x00006820,
    guest_pending_dbg_exceptions  = 0x00006822,
    guest_sysenter_esp            = 0x00006824,
    guest_sysenter_eip            = 0x00006826,
    /// 64-bit host fields.
    ///
    host_cr0                      = 0x00006c00,
    host_cr3                      = 0x00006c02,
    host_cr4                      = 0x00006c04,
    host_fs_base                  = 0x00006c06,
    host_gs_base                  = 0x00006c08,
    host_tr_base                  = 0x00006c0a,
    host_gdtr_base                = 0x00006c0c,
    host_idtr_base                = 0x00006c0e,
    host_sysenter_esp             = 0x00006c10,
    host_sysenter_eip             = 0x00006c12,
    host_rsp                      = 0x00006c14,
    host_rip                      = 0x00006c16,
};

enum class exit_reason : uint64_t
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
    max                          = 66
};

enum interrupt_t
{
    external_interrupt            = 0,
    reserved                      = 1,
    nmi                           = 2,
    hardware_exception            = 3,
    software_interrupt            = 4,
    privileged_software_interrupt = 5,
    software_exception            = 6,
    other_event                   = 7
};

enum class invept_t : uint64_t
{
    single_context = 1,
    all_contexts   = 2
};

enum class invvpid_t : uint64_t
{
  linear_address                    = 0,
  single_context                    = 1,
  all_contexts                      = 2,
  single_context_retaining_globals  = 3,
};

struct vmcs_t
{
    uint32_t revision_id;
    uint32_t abort_indicator;
    uint8_t  data[page_size - 8];
};
static_assert(sizeof(vmcs_t) == page_size, "VMCS size mismatch");

/// Table 28-1. Exit Qualification for Debug Exceptions
///
struct exit_qualification_debug_exception_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// When set, each of these bits indicates that the corresponding breakpoint condition was met.
            /// Any of these bits may be set even if its corresponding enabling bit in DR7 is not set.
            ///
            uint64_t breakpoint_condition          : 4;
            /// Reserved.
            ///
            uint64_t reserved0                     : 9;
            /// When set, this bit indicates that the cause of the debug exception is "debug register access detected."
            ///
            uint64_t debug_register_access_deteced : 1;
            /// When set, this bit indicates that the cause of the debug exception is either the execution of a single
            /// instruction (if RFLAGS.TF = 1 and IA32_DEBUGCTL.BTF = 0) or a taken branch (if RFLAGS.TF = DEBUGCTL.BTF = 1).
            ///
            uint64_t single_instruction            : 1;
            /// Reserved.
            ///
            uint64_t reserved1                     : 1;
            /// When set, this bit indicates that a debug exception (#DB) or a breakpoint exception (#BP) occurred
            /// inside an RTM region while advanced debugging of RTM transactional regions was enabled.
            ///
            uint64_t rtm_region_exception          : 1;
        };
    };
};

/// Table 28-2. Exit Qualification for Task Switches.
///
struct exit_qualification_task_switch_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// Selector of task-state segment (TSS) to which the guest attempted to switch.
            ///
            uint64_t selector   : 16;
            /// Reserved.
            ///
            uint64_t reserved0  : 14;
            /// Source of task switch initiation:
            /// 0: CALL instruction
            /// 1: IRET instruction
            /// 2: JMP instruction
            /// 3: Task gate in IDT
            ///
            uint64_t type       : 2;
        };
    };
};

/// Table 28-3. Exit Qualification for Control-Register Accesses.
///
struct exit_qualification_mov_cr_t
{
    enum
    {
        mov_to_cr   = 0,
        mov_from_cr = 1,
        clts        = 2,
        lmsw        = 3,
    };

    enum
    {
        lmsw_op_register = 0,
        lmsw_op_memory   = 1
    };

    union
    {
        uint64_t flags;

        struct
        {
            /// Number of control register (0 for CLTS and LMSW).
            ///
            uint64_t cr_number   : 4;
            /// Access type:
            /// 0 = MOV to CR
            /// 1 = MOV from CR
            /// 2 = CLTS
            /// 3 = LMSW
            ///
            uint64_t access_type : 2;
            /// LMSW operand type:
            /// 0 = register
            /// 1 = memory
            ///
            uint64_t lmsw_type   : 1;
            /// Reserved.
            ///
            uint64_t reserved0   : 1;
            /// For MOV CR, the general-purpose register.
            ///
            uint64_t gpr         : 4;
            /// Reserved.
            ///
            uint64_t reserved1   : 4;
            /// The LMSW source data.
            ///
            uint64_t lmsw_data   : 16;
        };
    };
};

/// Table 28-4. Exit Qualification for MOV DR
///
struct exit_qualification_mov_dr_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// Number of debug register.
            ///
            uint64_t dr_number   : 3;
            /// Reserved.
            ///
            uint64_t reserved0   : 1;
            /// Direction of access (0 = MOV to DR; 1 = MOV from DR).
            ///
            uint64_t access_type : 1;
            /// Reserved.
            ///
            uint64_t reserved1   : 3;
            /// General-purpose register.
            ///
            uint64_t grp         : 4;
        };
    };
};

/// Table 28-5. Exit Qualification for I/O Instructions
///
struct exit_qualification_io_t
{
    enum
    {
        access_out = 0,
        access_in  = 1
    };

    enum
    {
        op_encoding_dx  = 0,
        op_encoding_imm = 1
    };

    union
    {
        uint64_t flags;

        struct
        {
            /// Size of access.
            ///
            uint64_t access_size  : 3;
            /// Direction of the attempted access (0 = OUT, 1 = IN).
            ///
            uint64_t access_type  : 1;
            /// String instruction (0 = not string; 1 = string).
            ///
            uint64_t string_instruction : 1;
            /// REP prefixed (0 = not REP; 1 = REP).
            ///
            uint64_t rep_prefixed : 1;
            /// Operand encoding (0 = DX, 1 = immediate).
            uint64_t op_encoding  : 1;
            /// Reserved.
            ///
            uint64_t reserved0    : 9;
            /// Port number.
            ///
            uint64_t port_number  : 16;
        };
    };
};

/// Table 28-6. Exit Qualification for APIC-Access VM Exits from Linear Accesses and Guest-Physical Accesses
///
struct exit_qualification_apic_access_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// If the APIC-access VM exit is due to a linear access, the offset of access within the APIC page.
            ///
            uint64_t page_offset  : 12;
            /// Access type.
            ///
            uint64_t access_type  : 4;
            /// This bit is set for certain accesses that are asynchronous to instruction execution and not part of event delivery.
            ///
            uint64_t access_async : 1;
        };
    };
};

/// Table 28-7. Exit Qualification for EPT Violations
///
struct exit_qualification_ept_violation_t
{
    union
    {
        uint64_t flags;

        struct
        {
            uint64_t read                       : 1;
            uint64_t write                      : 1;
            uint64_t execute                    : 1;
            uint64_t gpa_readable               : 1;
            uint64_t gpa_writable               : 1;
            uint64_t gpa_executable             : 1;
            uint64_t gpa_execute_for_user_mode  : 1;
            uint64_t guest_linear_address_valid : 1;
            uint64_t violation_type             : 1;
            uint64_t user_mode_linear_address   : 1;
            uint64_t readable_writable_page     : 1;
            uint64_t execute_disable_page       : 1;
            uint64_t nmi_unblocking             : 1;
        };
    };
};

/// https://github.com/wbenny/hvpp/blob/84b3f3c241e1eec3ab42f75cad9deef3ad67e6ab/src/hvpp/hvpp/ia32/vmx/exit_qualification.h#L157
///
struct exit_qualification_t
{
    // For INVEPT, INVPCID, INVVPID, LGDT, LIDT, LLDT, LTR, SGDT, SIDT,
    // SLDT, STR, VMCLEAR, VMPTRLD, VMPTRST, VMREAD, VMWRITE, VMXON,
    // XRSTORS, and XSAVES, the exit qualification receives the value
    // of the instruction’s displacement field, which is sign-extended
    // to 64 bits if necessary (32 bits on processors that do not support
    // Intel 64 architecture).  If the instruction has no displacement
    // (for example, has a register operand), zero is stored into the
    // exit qualification.  On processors that support Intel 64 architecture,
    // an exception is made for RIP-relative addressing (used only in 64-bit
    // mode).  Such addressing causes an instruction to use an address
    // that is the sum of the displacement field and the value of RIP
    // that references the following instruction.  In this case, the
    // exit qualification is loaded with the sum of the displacement field
    // and the appropriate RIP value.
    // (ref: Vol3C[27.2.1(Basic VM-Exit Information)])
    //
    uint64_t                              displacement;

    // For a page-fault exception, the exit qualification contains the
    // linear-address that caused the page fault.
    //
    // For INVLPG, the exit qualification contains the linear-address operand
    // of the instruction.
    //
    uint64_t                              linear_address;

    exit_qualification_debug_exception_t debug_exception;
    exit_qualification_task_switch_t     task_switch;
    exit_qualification_mov_cr_t          mov_cr;
    exit_qualification_mov_dr_t          mov_dr;
    exit_qualification_io_t              io_instruction;
    exit_qualification_apic_access_t     apic_access;
    exit_qualification_ept_violation_t   ept_violation;
};

struct msr_bitmap_t
{
    static constexpr auto low_min  = 0x00000000;
    static constexpr auto low_max  = 0x00001FFF;
    static constexpr auto high_min = 0xC0000000;
    static constexpr auto high_max = 0xC0001FFF;

    union
    {
        uint8_t buffer[0x1000];

        struct
        {
            uint8_t read_low  [0x400];
            uint8_t read_high [0x400];
            uint8_t write_low [0x400];
            uint8_t write_high[0x400];
        };
    };
};

struct io_bitmap_t
{
    static constexpr auto a_min = 0x00000000;
    static constexpr auto a_max = 0x00007FFF;
    static constexpr auto b_min = 0x00008000;
    static constexpr auto b_max = 0x0000FFFF;
    uint8_t io_a[0x1000];
    uint8_t io_b[0x1000];
};

struct vm_interrupt_info_t
{
    union
    {
        uint32_t flags;
        struct
        {
            uint32_t vector         : 8;
            uint32_t type           : 3;
            uint32_t code           : 1;
            uint32_t nmi_unblocking : 1;
            uint32_t reserved       : 18;
            uint32_t valid          : 1;
        };
    };
};

/// INVEPT descriptor.
///
struct invept_desc_t
{
    uint64_t eptp;
    uint64_t reserved;
};
static_assert(sizeof(invept_desc_t) == 16);

/// INVVPID descriptor.
///
struct invvpid_desc_t
{
    uint64_t vpid     : 16;
    uint64_t reserved : 48;
    uint64_t linear_address;
};
static_assert(sizeof(invvpid_desc_t) == 16);

void invept(vmx::invept_t type, uint64_t eptp = 0);
void invvpid(vmx::invvpid_t type, uint64_t vpid = 0, uint64_t address = 0);
};
