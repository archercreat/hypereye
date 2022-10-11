#pragma once

#include "heye/arch/memory.hpp"

#include <cstdint>

namespace vmx
{
enum class vmcs : uint64_t
{
    virtual_processor_id          = 0x00000000,
    posted_interrupt_notification = 0x00000002,
    eptp_index                    = 0x00000004,
    /// @brief 16-bit guest fields.
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
    /// @brief 16-bit host fields.
    ///
    host_es_selector              = 0x00000c00,
    host_cs_selector              = 0x00000c02,
    host_ss_selector              = 0x00000c04,
    host_ds_selector              = 0x00000c06,
    host_fs_selector              = 0x00000c08,
    host_gs_selector              = 0x00000c0a,
    host_tr_selector              = 0x00000c0c,
    /// @brief 64-bit control fields.
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
    /// @brief 64-bit read-only fields.
    ///
    guest_physical_address        = 0x00002400,
    guest_physical_address_high   = 0x00002401,
    /// @brief 64-bit guest fields.
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
    /// @brief 64-bit host fields.
    ///
    host_ia32_pat                 = 0x00002c00,
    host_ia32_pat_high            = 0x00002c01,
    host_ia32_efer                = 0x00002c02,
    host_ia32_efer_high           = 0x00002c03,
    host_ia32_perf_ctrl           = 0x00002c04,
    host_ia32_perf_ctrl_high      = 0x00002c05,
    /// @brief 32-bit control fields.
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
    vm_entry_intr_info_field      = 0x00004016,
    vm_entry_exception_error_code = 0x00004018,
    vm_entry_instruction_len      = 0x0000401a,
    tpr_threshold                 = 0x0000401c,
    secondary_vm_exec_control     = 0x0000401e,
    ple_gap                       = 0x00004020,
    ple_window                    = 0x00004022,
    /// @brief 32-bit read-only fields.
    ///
    vm_instruction_error          = 0x00004400,
    vm_exit_reason                = 0x00004402,
    vm_exit_intr_info             = 0x00004404,
    vm_exit_intr_error_code       = 0x00004406,
    idt_vectoring_info_field      = 0x00004408,
    idt_vectoring_error_code      = 0x0000440a,
    vm_exit_instruction_len       = 0x0000440c,
    vmx_instruction_info          = 0x0000440e,
    /// @brief 32-bit guest fields.
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
    /// @brief 32-bit host fields.
    ///
    host_sysenter_cs              = 0x00004c00,
    /// @brief 64-bit control fields.
    ///
    cr0_guest_host_mask           = 0x00006000,
    cr4_guest_host_mask           = 0x00006002,
    cr0_read_shadow               = 0x00006004,
    cr4_read_shadow               = 0x00006006,
    cr3_target_value0             = 0x00006008,
    cr3_target_value1             = 0x0000600a,
    cr3_target_value2             = 0x0000600c,
    cr3_target_value3             = 0x0000600e,
    /// @brief 64-bit read-only fields.
    ///
    exit_qualification            = 0x00006400,
    io_rcx                        = 0x00006402,
    io_rsi                        = 0x00006404,
    io_rdi                        = 0x00006406,
    io_rip                        = 0x00006408,
    guest_linear_address          = 0x0000640a,
    /// @brief 64-bit guest fields.
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
    /// @brief 64-bit host fields.
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

struct vmcs_t
{
    uint32_t revision_id;
    uint32_t abort_indicator;
    uint8_t  data[PAGE_SIZE - 8];
};
static_assert(sizeof(vmcs_t) == PAGE_SIZE, "VMCS size mismatch");
};



















