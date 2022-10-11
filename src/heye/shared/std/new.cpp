#include "heye/shared/std/new.hpp"
#include "heye/config.hpp"

#include <ntddk.h>

void* operator new(size_t size) noexcept
{
    PAGED_CODE();
    return ExAllocatePoolZero(NonPagedPool, size, POOL_TAG);
}

void* operator new[](size_t size) noexcept
{
    PAGED_CODE();
    return ExAllocatePoolZero(NonPagedPool, size, POOL_TAG);
}

// MmAllocateContiguousMemory always allocates aligned memory address so there is no need for manual alignment.
// NOTE: It should be used with care since contiguous physical memory is not cheap.
// Also there is no checks for `alignment` being power of 2.
//
void* operator new(size_t size, std::align_val_t alignment) noexcept
{
    PAGED_CODE();
    UNREFERENCED_PARAMETER(alignment);
    auto ptr = MmAllocateContiguousMemory((size / PAGE_SIZE) * PAGE_SIZE, { .QuadPart = ~0 });
    if (ptr != nullptr)
        RtlSecureZeroMemory(ptr, size);
    return ptr;
}

void operator delete  (void* ptr)                   noexcept { PAGED_CODE(); if (ptr != nullptr) ExFreePoolWithTag(ptr, POOL_TAG); }
void operator delete[](void* ptr)                   noexcept { PAGED_CODE(); if (ptr != nullptr) ExFreePoolWithTag(ptr, POOL_TAG); }
void operator delete[](void* ptr, size_t)           noexcept { PAGED_CODE(); if (ptr != nullptr) ExFreePoolWithTag(ptr, POOL_TAG); }
void operator delete  (void* ptr, size_t)           noexcept { PAGED_CODE(); if (ptr != nullptr) ExFreePoolWithTag(ptr, POOL_TAG); }
void operator delete  (void* ptr, std::align_val_t) noexcept { PAGED_CODE(); if (ptr != nullptr) MmFreeContiguousMemory(ptr);      }
