#pragma once

#include <cstdint>

namespace cpuid
{
/// @brief CPUID EAX=0.
///
struct manufacturer_id
{
    static constexpr int leaf = 0;

    union
    {
        int data[4];

        struct
        {
            union
            {
                uint32_t eax;
                uint32_t highest_function_parameter;
            };

            char ebx[4];
            char ecx[4];
            char edx[4];
        };
    };
};
static_assert(sizeof(manufacturer_id) == sizeof(uint32_t) * 4, "CPUID EAX=0 size mismatch");

/// @brief CPUID EAX=1.
///
struct processor_features
{
    static constexpr int leaf = 1;

    union
    {
        int data[4];

        struct
        {
            union
            {
                uint32_t eax;

                struct
                {
                    /// @brief Stepping ID is a product revision number assigned due to fixed errata or other changes.
                    ///
                    uint32_t stepping_id    : 4;
                    /// @brief
                    ///
                    uint32_t model_id       : 4;
                    /// @brief
                    ///
                    uint32_t family_id      : 4;
                    /// @brief
                    ///
                    uint32_t processor_type : 2;
                    /// @brief
                    ///
                    uint32_t _reserved1     : 2;
                    /// @brief
                    ///
                    uint32_t ext_model_id   : 4;
                    /// @brief
                    ///
                    uint32_t ext_family_id  : 8;
                    /// @brief
                    ///
                    uint32_t _reserved2     : 4;
                };
            };

            union
            {
                uint32_t ebx;

                struct
                {
                    /// @brief Brand Index.
                    ///
                    uint32_t brand_index     : 8;
                    /// @brief CLFLUSH line size (Value * 8 = cache line size in bytes).
                    ///
                    uint32_t clfush_size     : 8;
                    /// @brief Maximum number of addressable IDs for logical processors in this physical package.
                    ///
                    uint32_t addressable_ids : 8;
                    /// @brief Local APIC ID: The initial APIC-ID is used to identify the executing logical processor.
                    ///
                    uint32_t local_apic_id   : 8;
                };
            };

            union
            {
                uint32_t ecx;

                struct
                {
                    /// @brief Prescott New Instructions-SSE3 (PNI).
                    ///
                    uint32_t sse3           : 1;
                    /// @brief PCLMULQDQ.
                    ///
                    uint32_t pclmulqdq      : 1;
                    /// @brief 	64-bit debug store (edx bit 21).
                    ///
                    uint32_t dtes64         : 1;
                    /// @brief MONITOR and MWAIT instructions (SSE3).
                    ///
                    uint32_t monitor        : 1;
                    /// @brief CPL qualified debug store.
                    ///
                    uint32_t ds_cpl         : 1;
                    /// @brief Virtual Machine eXtensions.
                    ///
                    uint32_t vmx            : 1;
                    /// @brief Safer Mode Extensions (LaGrande).
                    ///
                    uint32_t smx            : 1;
                    /// @brief Enhanced SpeedStep.
                    ///
                    uint32_t est            : 1;
                    /// @brief Thermal Monitor 2.
                    ///
                    uint32_t tm2            : 1;
                    /// @brief Supplemental SSE3 instructions.
                    ///
                    uint32_t ssse3          : 1;
                    /// @brief L1 Context ID.
                    ///
                    uint32_t cnxt_id        : 1;
                    /// @brief Silicon Debug interface.
                    ///
                    uint32_t sdbg           : 1;
                    /// @brief Fused multiply-add (FMA3).
                    ///
                    uint32_t fma            : 1;
                    /// @brief CMPXCHG16B instruction.
                    ///
                    uint32_t cx16           : 1;
                    /// @brief Can disable sending task priority messages
                    ///
                    uint32_t xtpr           : 1;
                    /// @brief Perfmon & debug capability.
                    ///
                    uint32_t pdcm           : 1;
                    /// @brief
                    ///
                    uint32_t _reserved1     : 1;
                    /// @brief Process context identifiers (CR4 bit 17).
                    ///
                    uint32_t pcid           : 1;
                    /// @brief Direct cache access for DMA writes.
                    ///
                    uint32_t dca            : 1;
                    /// @brief SSE4.1 instructions.
                    ///
                    uint32_t sse41          : 1;
                    /// @brief SSE4.2 instructions.
                    ///
                    uint32_t sse42          : 1;
                    /// @brief x2APIC.
                    ///
                    uint32_t x2apic         : 1;
                    /// @brief MOVBE instruction (big-endian).
                    ///
                    uint32_t movbe          : 1;
                    /// @brief POPCNT instruction.
                    ///
                    uint32_t popcnt         : 1;
                    /// @brief APIC implements one-shot operation using a TSC deadline value.
                    ///
                    uint32_t tsc_deadline   : 1;
                    /// @brief AES instruction set.
                    ///
                    uint32_t aes            : 1;
                    /// @brief XSAVE, XRESTOR, XSETBV, XGETBV.
                    ///
                    uint32_t xsave          : 1;
                    /// @brief XSAVE enabled by OS.
                    ///
                    uint32_t osxsave        : 1;
                    /// @brief Advanced Vector Extensions.
                    ///
                    uint32_t avx            : 1;
                    /// @brief F16C (half-precision) FP feature.
                    ///
                    uint32_t f16c           : 1;
                    /// @brief RDRAND (on-chip random number generator) feature.
                    ///
                    uint32_t rdrnd          : 1;
                    /// @brief Hypervisor present (always zero on physical CPUs).
                    ///
                    uint32_t hypervisor     : 1;
                };
            };

            union
            {
                uint32_t edx;

                struct
                {
                    /// @brief 	Onboard x87 FPU.
                    ///
                    uint32_t fpu        : 1;
                    /// @brief Virtual 8086 mode extensions (such as VIF, VIP, PIV).
                    ///
                    uint32_t vme        : 1;
                    /// @brief Debugging extensions (CR4 bit 3).
                    ///
                    uint32_t de         : 1;
                    /// @brief Page Size Extension.
                    ///
                    uint32_t pse        : 1;
                    /// @brief Time Stamp Counter.
                    ///
                    uint32_t tsc        : 1;
                    /// @brief Model-specific registers.
                    ///
                    uint32_t msr        : 1;
                    /// @brief Physical Address Extension.
                    ///
                    uint32_t pae        : 1;
                    /// @brief Machine Check Exception.
                    ///
                    uint32_t mce        : 1;
                    /// @brief CMPXCHG8 (compare-and-swap) instruction.
                    ///
                    uint32_t cx8        : 1;
                    /// @brief Onboard Advanced Programmable Interrupt Controller.
                    ///
                    uint32_t apic       : 1;
                    /// @brief
                    ///
                    uint32_t _reserved1 : 1;
                    /// @brief SYSENTER and SYSEXIT instructions.
                    ///
                    uint32_t sep        : 1;
                    /// @brief Memory Type Range Registers.
                    ///
                    uint32_t mtrr       : 1;
                    /// @brief Page Global Enable bit in CR4.
                    ///
                    uint32_t pge        : 1;
                    /// @brief Machine check architecture.
                    ///
                    uint32_t mca        : 1;
                    /// @brief Conditional move and FCMOV instructions.
                    ///
                    uint32_t cmov       : 1;
                    /// @brief Page Attribute Table.
                    ///
                    uint32_t pat        : 1;
                    /// @brief 36-bit page size extension.
                    ///
                    uint32_t pse_36     : 1;
                    /// @brief Processor Serial Number.
                    ///
                    uint32_t psn        : 1;
                    /// @brief CLFLUSH instruction (SSE2).
                    ///
                    uint32_t clfsh      : 1;
                    /// @brief
                    ///
                    uint32_t _reserved2 : 1;
                    /// @brief Debug store: save trace of executed jumps.
                    ///
                    uint32_t ds         : 1;
                    /// @brief Onboard thermal control MSRs for ACPI.
                    ///
                    uint32_t acpi       : 1;
                    /// @brief MMX instructions.
                    ///
                    uint32_t mmx        : 1;
                    /// @brief FXSAVE, FXRESTOR instructions, CR4 bit 9.
                    ///
                    uint32_t fxsr       : 1;
                    /// @brief SSE instructions (a.k.a. Katmai New Instructions).
                    ///
                    uint32_t sse        : 1;
                    /// @brief SSE2 instructions.
                    ///
                    uint32_t sse2       : 1;
                    /// @brief CPU cache implements self-snoop.
                    ///
                    uint32_t ss         : 1;
                    /// @brief Hyper-threading.
                    ///
                    uint32_t htt        : 1;
                    /// @brief Thermal monitor automatically limits temperature.
                    ///
                    uint32_t tm         : 1;
                    /// @brief IA64 processor emulating x86.
                    ///
                    uint32_t ia64       : 1;
                    /// @brief Pending Break Enable (PBE# pin) wakeup capability.
                    ///
                    uint32_t pbe        : 1;
                };
            };
        };
    };
};
static_assert(sizeof(processor_features) == sizeof(uint32_t) * 4, "CPUID EAX=1 size mismatch");
};
