#pragma once

#include <cstdint>

/// @brief All `new` allocations are made from nonpaged pool.
///
[[nodiscard]] void* operator new(size_t)                    noexcept;
[[nodiscard]] void* operator new[](size_t)                  noexcept;
[[nodiscard]] void* operator new(size_t, std::align_val_t)  noexcept;
void operator delete  (void* ptr)                           noexcept;
void operator delete  (void* ptr, std::align_val_t)         noexcept;
void operator delete[](void* ptr)                           noexcept;
void operator delete[](void* ptr, size_t)                   noexcept;
void operator delete  (void* ptr, size_t)                   noexcept;
