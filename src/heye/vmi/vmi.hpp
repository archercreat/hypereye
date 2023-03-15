#pragma once
#include "process.hpp"

#include <cstdint>

namespace heye
{
uint64_t get_current_thread();
uint64_t get_current_process();
} // namespace heye
