#pragma once
#include "traits.hpp"

namespace std
{
template <typename T>
[[nodisard]] remove_reference_t<T>&& move(T&& arg) noexcept
{
  return static_cast<remove_reference_t<T>&&>(arg);
}

template <typename T>
[[nodisard]] constexpr T&& forward(remove_reference_t<T>& arg) noexcept
{
    return static_cast<T&&>(arg);
}

template <class T>
[[nodisard]] constexpr T&& forward(remove_reference_t<T>&& arg) noexcept
{
    static_assert(!is_lvalue_reference_v<T>, "bad forward call");
    return static_cast<T&&>(arg);
}

template<class T, class U = T>
constexpr T exchange(T& object, U&& value) noexcept
{
    T old_value = std::move(object);
    object      = std::forward<U>(value);
    return old_value;
}

template<typename T>
struct singleton
{
    singleton(const singleton&)            = delete;
    singleton(singleton&&)                 = delete;
    singleton& operator=(const singleton&) = delete;
    singleton& operator=(singleton&&)      = delete;

    static T& get()
    {
        static T object;
        return object;
    }

protected:
    singleton(){};
};
};
