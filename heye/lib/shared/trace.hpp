#pragma once
#include "std/utility.hpp"

/// @brief The only reason I use singleton here is because it lets me use crt to automatically unregister
/// ETW provider after driver unloading.
struct logger : public std::singleton<logger>
{
    friend struct std::singleton<logger>;

    static void info(const char* format, ...);

protected:
    /// @brief Registers ETW provider.
    ///
    logger();

    /// @brief Unregisters ETW provider.
    ///
    ~logger();
};
