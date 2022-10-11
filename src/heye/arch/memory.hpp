#pragma once
#include "heye/config.hpp"

#include <cstdint>

enum memory_type_t
{
    uncachable      = 0,
    write_combining = 1,
    write_through   = 4,
    write_protected = 5,
    write_back      = 6
};

inline uint64_t operator""_kb(uint64_t size) { return size * 1024;    }
inline uint64_t operator""_mb(uint64_t size) { return size * 1024_kb; }
inline uint64_t operator""_gb(uint64_t size) { return size * 1024_mb; }


inline uint64_t pfn(uint64_t pa) { return pa >> PAGE_SHIFT; }

uint64_t pa_from_va(const void* va);
void*    va_from_pa(uint64_t    pa);
