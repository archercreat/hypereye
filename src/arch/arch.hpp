#pragma once

#include "dr.hpp"
#include "cpuid.hpp"
#include "msr.hpp"
#include "cr.hpp"
#include "segments.hpp"
#include "asm.hpp"
#include "mtrr.hpp"
#include "memory.hpp"

#include "shared/std/traits.hpp"

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

// template <> inline void write(gdtr_t gdtr)
// {
//     asm_write_gdtr(&gdtr);
// }

// template <> inline void write(idtr_t idtr)
// {
//     asm_write_idtr(&idtr);
// }

/// @brief Generic segment access implementation.
///
#define impl_read(name)                                             \
    template<> inline name ##_t read()                              \
    {                                                               \
        const auto name = asm_read_ ##name();                       \
        const auto base = !std::scmp(#name, "gs") ?                 \
            read<msr::gsbase>().flags : !std::scmp(#name, "fs") ?   \
            read<msr::fsbase>().flags : 0;                          \
        return name ##_t {                                          \
            name,                                                   \
            __segmentlimit(name),                                   \
            name ? (asm_lar(name) >> 8) & 0xf0ff : 0x10000,         \
            base                                                    \
        };                                                          \
    }

impl_read(es);
impl_read(cs);
impl_read(ss);
impl_read(ds);
impl_read(fs);
impl_read(gs);

#undef impl_read

#define impl_read(name)                                                                                 \
    template<> inline name ##_t read()                                                                  \
    {                                                                                                   \
        const auto gdtr    = read<gdtr_t>();                                                            \
        const auto sel     = selector_t{ asm_read_ ##name() };                                          \
        descriptor_t* desc = nullptr;                                                                   \
        if (sel.ti)                                                                                     \
        {                                                                                               \
            const auto ldtr = selector_t{ asm_read_ldtr() };                                            \
            desc = reinterpret_cast<descriptor_t*>(gdtr.base    + ldtr.index * sizeof(descriptor_t));   \
            desc = reinterpret_cast<descriptor_t*>(desc->base() + sel.index  * sizeof(descriptor_t));   \
        }                                                                                               \
        else                                                                                            \
            desc = reinterpret_cast<descriptor_t*>(gdtr.base + sel.index * sizeof(descriptor_t));       \
                                                                                                        \
        return name ##_t {                                                                              \
            sel.flags,                                                                                  \
            __segmentlimit(sel.flags),                                                                  \
            sel.flags ? (asm_lar(sel.flags) >> 8) & 0xf0ff : 0x10000,                                   \
            desc->base()                                                                                \
        };                                                                                              \
    }

impl_read(ldtr);
impl_read(tr);

#undef impl_read
