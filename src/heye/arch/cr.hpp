#pragma once

#include <cstdint>

namespace heye
{
struct cr0_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// If 1, system is in protected mode, else system is in real mode.
            ///
            uint64_t pe : 1;
            /// Controls interaction of WAIT/FWAIT instructions with TS flag in CR0.
            ///
            uint64_t mp : 1;
            /// If set, no x87 floating-point unit present, if clear, x87 FPU present.
            ///
            uint64_t em : 1;
            /// Allows saving x87 task context upon a task switch only after x87 instruction used.
            ///
            uint64_t ts : 1;
            /// On the 386, it allowed to specify whether the external math coprocessor was an 80287 or 80387.
            ///
            uint64_t et : 1;
            /// Enable internal x87 floating point error reporting when set, else enables PC style x87 error detection.
            ///
            uint64_t ne : 1;
            /// Reserved.
            ///
            uint64_t reserved0 : 10;
            /// When set, the CPU can't write to read-only pages when privilege level is 0.
            ///
            uint64_t wp : 1;
            /// Reserved.
            ///
            uint64_t reserved1 : 1;
            /// Alignment check enabled if AM set, AC flag (in EFLAGS register) set, and privilege level is 3.
            ///
            uint64_t am : 1;
            /// Reserved.
            ///
            uint64_t reserved2 : 10;
            /// Globally enables/disable write-through caching.
            ///
            uint64_t nw : 1;
            /// Globally enables/disable the memory cache.
            ///
            uint64_t cd : 1;
            /// If 1, enable paging and use the § CR3 register, else disable paging.
            ///
            uint64_t pg : 1;
        };
    };
};
static_assert(sizeof(cr0_t) == sizeof(uint64_t), "cr0_t size mismatch");

struct cr2_t
{
    uint64_t address;
};
static_assert(sizeof(cr2_t) == sizeof(uint64_t), "cr2_t size mismatch");

struct cr3_t
{
    uint64_t flags;
};
static_assert(sizeof(cr3_t) == sizeof(uint64_t), "cr3_t size mismatch");

struct cr4_t
{
    union
    {
        uint64_t flags;

        struct
        {
            /// If set, enables support for the virtual interrupt flag (VIF) in virtual-8086 mode.
            ///
            uint64_t vme        : 1;
            /// If set, enables support for the virtual interrupt flag (VIF) in protected mode.
            ///
            uint64_t pvi        : 1;
            /// If set, RDTSC instruction can only be executed when in ring 0, otherwise RDTSC can be used at any privilege level.
            ///
            uint64_t tsd        : 1;
            /// If set, enables debug register based breaks on I/O space access.
            ///
            uint64_t de         : 1;
            /// If unset, page size is 4 KiB, else page size is increased to 4 MiB. If PAE is enabled or the processor is in x86-64 long mode this bit is ignored.
            ///
            uint64_t pse        : 1;
            /// If set, changes page table layout to translate 32-bit virtual addresses into extended 36-bit physical addresses.
            ///
            uint64_t pae        : 1;
            /// If set, enables machine check interrupts to occur.
            ///
            uint64_t mce        : 1;
            /// If set, address translations (PDE or PTE records) may be shared between address spaces.
            ///
            uint64_t pge        : 1;
            /// If set, RDPMC can be executed at any privilege level, else RDPMC can only be used in ring 0.
            ///
            uint64_t pce        : 1;
            /// If set, enables Streaming SIMD Extensions (SSE) instructions and fast FPU save & restore.
            ///
            uint64_t osfxsr     : 1;
            /// If set, enables unmasked SSE exceptions.
            ///
            uint64_t osxmmexcpt : 1;
            /// If set, the SGDT, SIDT, SLDT, SMSW and STR instructions cannot be executed if CPL > 0.
            ///
            uint64_t umip       : 1;
            /// If set, enables 5-Level Paging.
            ///
            uint64_t la57       : 1;
            /// If set, Virtual Machine Extensions supported.
            ///
            uint64_t vmxe       : 1;
            /// If set, Safer Mode Extensions supported.
            ///
            uint64_t smxe       : 1;
            ///
            ///
            uint64_t _reserved1 : 1;
            /// Enables the instructions RDFSBASE, RDGSBASE, WRFSBASE, and WRGSBASE.
            ///
            uint64_t fsgsbase   : 1;
            /// If set, enables process-context identifiers (PCIDs).
            ///
            uint64_t pcide      : 1;
            /// XSAVE and Processor Extended States Enable.
            ///
            uint64_t osxsave    : 1;
            ///
            ///
            uint64_t _reserved2 : 1;
            /// If set, execution of code in a higher ring generates a fault.
            ///
            uint64_t smep       : 1;
            /// If set, access of data in a higher ring generates a fault.
            ///
            uint64_t smap       : 1;
            /// Protection Key Enable.
            ///
            uint64_t pke        : 1;
            /// If set, enables control-flow enforcement technology.
            ///
            uint64_t cet        : 1;
            /// If set, each supervisor-mode linear address is associated with a protection key when 4-level or 5-level paging is in use.
            ///
            uint64_t pks        : 1;
        };
    };

};
static_assert(sizeof(cr4_t) == sizeof(uint64_t), "cr4_t size mismatch");
};
