#include "heye/shared/globals.hpp"

#define _CRTALLOC$(x) __declspec(allocate(x))

/// C Initializer.
///
#pragma section(".CRT$XIA", long, read)
#pragma section(".CRT$XIZ", long, read)
/// C++ Initializer.
///
#pragma section(".CRT$XCA", long, read)
#pragma section(".CRT$XCZ", long, read)
/// C pre-terminators.
///
#pragma section(".CRT$XPA", long, read)
#pragma section(".CRT$XPZ", long, read)
/// C terminators.
///
#pragma section(".CRT$XTA", long, read)
#pragma section(".CRT$XTZ", long, read)

extern "C" _CRTALLOC$(".CRT$XIA") _PIFV __xi_a[] = { nullptr };
extern "C" _CRTALLOC$(".CRT$XIZ") _PIFV __xi_z[] = { nullptr };
extern "C" _CRTALLOC$(".CRT$XCA") _PVFV __xc_a[] = { nullptr };
extern "C" _CRTALLOC$(".CRT$XCZ") _PVFV __xc_z[] = { nullptr };
extern "C" _CRTALLOC$(".CRT$XPA") _PVFV __xp_a[] = { nullptr };
extern "C" _CRTALLOC$(".CRT$XPZ") _PVFV __xp_z[] = { nullptr };
extern "C" _CRTALLOC$(".CRT$XTA") _PVFV __xt_a[] = { nullptr };
extern "C" _CRTALLOC$(".CRT$XTZ") _PVFV __xt_z[] = { nullptr };
/// Merge everything into rdata.
///
#pragma comment(linker, "/merge:.CRT=.rdata")

extern "C"
static void __cdecl _initterm(_PVFV* first, _PVFV* last)
{
    for (auto it = first; it != last; ++it)
    {
        if (*it != nullptr)
            (**it)();
    }
}

extern "C"
static int __cdecl _initterm_e(_PIFV* first, _PIFV* last)
{
    for (auto it = first; it != last; ++it)
    {
        if (*it != nullptr)
        {
            if (auto result = (**it)(); result != 0)
                return result;
        }
    }
    return 0;
}

struct onexit_entry
{
    onexit_entry* next       = nullptr;
    _PVFV         destructor = nullptr;

    onexit_entry(onexit_entry* next, _PVFV destructor) : next{ next }, destructor{ destructor }
    {}

    ~onexit_entry()
    {
        destructor();
    }
};

static onexit_entry* onexit_table = nullptr;

static int __cdecl register_onexit(onexit_entry* table, _PVFV function)
{
    const auto entry = new onexit_entry(table, function);
    if (nullptr == entry)
    {
        return -1;
    }
    onexit_table = entry;
    return 0;
}

static int __cdecl execute_onexit(onexit_entry* table)
{
    for (auto entry = table; entry != nullptr;)
    {
        const auto next = entry->next;
        delete entry;
        entry = next;
    }
    return 0;
}

extern "C"
int __cdecl atexit(_PVFV function)
{
    return register_onexit(onexit_table, function);
}

extern "C"
int __cdecl onexit()
{
    return execute_onexit(onexit_table);
}

namespace globals
{
void initialize()
{
    // do C initializations.
    //
    _initterm_e(__xi_a, __xi_z);
    // do C++ initializations.
    //
    _initterm(__xc_a, __xc_z);
}

void teardown()
{
    // do exit() of atexit().
    //
    onexit();
}
};
