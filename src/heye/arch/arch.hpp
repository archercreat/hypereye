#pragma once

#include "dr.hpp"
#include "cpuid.hpp"
#include "msr.hpp"
#include "cr.hpp"
#include "segments.hpp"
#include "asm.hpp"
#include "mtrr.hpp"
#include "memory.hpp"

#include "heye/shared/std/traits.hpp"

#include <intrin.h>

template<typename T> requires (!std::has_subleaf_v<T> && std::has_leaf_v<T>)
inline T read()
{
    T result{};
    __cpuid(result.data, T::leaf);
    return result;
}

template<typename T> requires (std::has_subleaf_v<T> && std::has_leaf_v<T>)
inline T read()
{
    T result{};
    __cpuidex(result, T::leaf, T::subleaf);
    return result;
}

template<typename T> requires (std::has_id_v<T>)
inline T read() { return T{ __readmsr(T::id) }; }

template<typename T> requires (std::has_id_v<T>)
inline T read(uint32_t value) { return T{ __readmsr(value) };}

template<typename T> requires (std::has_id_v<T>)
inline void write(T value) { __writemsr(T::id,  value.flags); }

template<typename T> T    read()     { __debugbreak(); }
template<typename T> void write(T v) { __debugbreak(); }

template<> inline cr0_t read() { return cr0_t{ __readcr0() }; }
template<> inline cr2_t read() { return cr2_t{ __readcr2() }; }
template<> inline cr3_t read() { return cr3_t{ __readcr3() }; }
template<> inline cr4_t read() { return cr4_t{ __readcr4() }; }

template<> inline void write(cr0_t cr0) { __writecr0(cr0.flags); }
template<> inline void write(cr3_t cr3) { __writecr3(cr3.flags); }
template<> inline void write(cr4_t cr4) { __writecr4(cr4.flags); }

template<> inline dr7_t read() { return dr7_t{ __readdr(7) }; }

template<> inline gdtr_t read()
{
    gdtr_t gdtr{};
    asm_read_gdtr(&gdtr);
    return gdtr;
}

template<> inline idtr_t read()
{
    idtr_t idtr{};
    asm_read_idtr(&idtr);
    return idtr;
}

template <> inline void write(gdtr_t gdtr)
{
    asm_write_gdtr(&gdtr);
}

template <> inline void write(idtr_t idtr)
{
    asm_write_idtr(&idtr);
}

#define impl_read(name)                                                                 \
    template<> inline segment_t<name ##_t> read()                                       \
    {                                                                                   \
        segment_t<name ##_t> segment{};                                                 \
        const auto selector = name ##_t{ asm_read_ ##name() };                          \
        segment.selector    = selector;                                                 \
        segment.limit       = __segmentlimit(selector.flags);                           \
        segment.rights      = access_t                                                  \
        {                                                                               \
            .flags = static_cast<uint32_t>((asm_lar(selector.flags) >> 8) & 0xf0ff)     \
        };                                                                              \
        segment.rights.unusable = selector.flags ? 0 : 1;                               \
                                                                                        \
        if (!std::scmp(#name, "gs"))                                                    \
        {                                                                               \
            segment.base = read<msr::gsbase>().flags;                                   \
        }                                                                               \
        else if (!std::scmp(#name, "fs"))                                               \
        {                                                                               \
            segment.base = read<msr::fsbase>().flags;                                   \
        }                                                                               \
        else                                                                            \
        {                                                                               \
            const auto desc = reinterpret_cast<descriptor_t*>(                          \
                read<gdtr_t>().base + static_cast<uint64_t>(selector.index) * 8);       \
            segment.base = desc->base();                                                \
        }                                                                               \
        return segment;                                                                 \
    }

impl_read(es);
impl_read(cs);
impl_read(ss);
impl_read(ds);
impl_read(fs);
impl_read(gs);
impl_read(tr);
impl_read(ldtr);

#undef impl_read
