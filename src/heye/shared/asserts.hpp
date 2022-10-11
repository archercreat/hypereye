#pragma once

#include <intrin.h>

inline static constexpr void abort_if(bool condition)
{
    if (condition)
    {
        __debugbreak();
    }
}

#define fassert(...) abort_if(!bool(__VA_ARGS__))
