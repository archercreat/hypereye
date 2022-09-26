#pragma once
#include "unique.hpp"

namespace std
{
/// cv paste from stackoverflow
///
template<typename F>
struct function{};

template<typename R, typename... A>
struct function<R(A...)>
{
private:
    struct callable
    {
        virtual ~callable()              = default;
        virtual R operator()(A...) const = 0;
    };

    template<typename T>
    struct callable_t : callable
    {
        callable_t(T&& fn) : fn(std::forward<T>(fn)) {}

        R operator()(A... args) const { return fn(std::forward<A>(args)...); }

    private:
        T fn;
    };

    unique_ptr<callable> callable_fn;

public:
    function() = default;

    template<typename T>
    function(T&& fn) : callable_fn(new callable_t<T>(std::forward<T>(fn))) {}

    R operator()(A... args) const
    {
        return (*callable_fn)(std::forward<A>(args)...);
    }

    operator bool() const { return callable_fn; }

    function(const function<R(A ...)>&)            = delete;
    function& operator=(const function<R(A ...)>&) = delete;
};
};
