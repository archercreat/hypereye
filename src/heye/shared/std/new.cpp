#include "heye/config.hpp"

#include <cstdint>
#include <ntddk.h>

void* operator new(size_t size) noexcept
{
    PAGED_CODE();
    return ExAllocatePoolZero(NonPagedPool, size, pool_tag);
}

void* operator new[](size_t size) noexcept
{
    PAGED_CODE();
    return ExAllocatePoolZero(NonPagedPool, size, pool_tag);
}

void operator delete  (void* ptr)         noexcept { PAGED_CODE(); if (ptr != nullptr) ExFreePoolWithTag(ptr, pool_tag); }
void operator delete[](void* ptr)         noexcept { PAGED_CODE(); if (ptr != nullptr) ExFreePoolWithTag(ptr, pool_tag); }
void operator delete[](void* ptr, size_t) noexcept { PAGED_CODE(); if (ptr != nullptr) ExFreePoolWithTag(ptr, pool_tag); }
void operator delete  (void* ptr, size_t) noexcept { PAGED_CODE(); if (ptr != nullptr) ExFreePoolWithTag(ptr, pool_tag); }
