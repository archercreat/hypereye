#include "heye/shared/cpu.hpp"
#include "heye/shared/trace.hpp"
#include "heye/shared/asserts.hpp"

#include <ntddk.h>
#include <winmeta.h>
#include <TraceLoggingProvider.h>

#define _NO_CRT_STDIO_INLINE
#include <cstdarg>
#include <stdio.h>

/// @brief GUID: {60c3d354-edc4-4d20-8132-e16d9eeba96c}
///
TRACELOGGING_DECLARE_PROVIDER(provider);
TRACELOGGING_DEFINE_PROVIDER(
    provider, "HyperEyeProvider",
    (0x60c3d354, 0xedc4, 0x4d20, 0x81, 0x32, 0xe1, 0x6d, 0x9e, 0xeb, 0xa9, 0x6c)
);

namespace detail
{
static bool initialized = false;

void do_trace(const char* message)
{
    TraceLoggingWrite(
        provider,
        "MessageEvent",
        TraceLoggingLevel(WINEVENT_LEVEL_INFO),
        TraceLoggingValue(cpu::current(), "Core"),
        TraceLoggingValue(message, "Message")
   );
}
};

logger::logger()
{
    fassert(NT_SUCCESS(TraceLoggingRegister(provider)));
    // This class is a singleton meaning this constructor will only be called once.
    // So we don't have to use atomics here.
    //
    detail::initialized = true;
}

logger::~logger()
{
    TraceLoggingUnregister(provider);
    detail::initialized = false;
}

void logger::info(const char* format, ...)
{
    if (detail::initialized)
    {
        va_list args;
        va_start(args, format);

        char message[512];
        vsprintf_s(message, sizeof(message), format, args);
        detail::do_trace(message);
    }
}
