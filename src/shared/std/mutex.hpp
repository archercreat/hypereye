#pragma once

#include <intrin.h>

namespace std
{
/// @brief https://rayanfam.com/topics/hypervisor-from-scratch-part-8/#designing-a-spinlock
///
struct lock_guard
{
    void lock()
    {
        // unsigned wait = 1;

        // while (!_lock && !InterlockedBitTestAndSet(&_lock, 0))
        // {
        //     for (unsigned i = 0; i < wait; i++)
        //         _mm_pause();

        //     if (wait * 2 > max_wait)
        //         wait = max_wait;
        //     else
        //         wait *= 2;
        // }
    }

    void unlock()
    {
        _lock = 0;
    }

private:
    static constexpr auto max_wait = 65536;
    volatile long _lock = 0;
};
};
