#include "heye/arch/memory.hpp"

#include <ntddk.h>

uint64_t pa_from_va(const void* va)
{
    return static_cast<uint64_t>(MmGetPhysicalAddress((PVOID)va).QuadPart);
}

void* va_from_pa(uint64_t pa)
{
    return MmGetVirtualForPhysical({ .QuadPart = static_cast<LONGLONG>(pa) });
}
