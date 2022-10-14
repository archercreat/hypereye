#pragma once
#include "std/utility.hpp"

namespace heye::logger
{
bool setup();
void teardown();
void info(const char* format, ...);
};
