#pragma once
#include "traits.hpp"
#include "utility.hpp"

namespace std
{
template<typename T>
struct unique_ptr
{
    using value_type = remove_extent<T>::type;

    unique_ptr()                : ptr(nullptr) {}
    unique_ptr(value_type* ptr) : ptr(ptr)     {}

    ~unique_ptr()
    {
        if constexpr (is_array<T>::value)
        {
            delete[] ptr;
        }
        else
            delete ptr;
    }

    unique_ptr(unique_ptr<T>&& other)
    {
        operator=(std::move(other));
    }

    unique_ptr& operator=(unique_ptr<T>&& other)
    {
        if (this != &other)
        {
            this->~unique_ptr();
            this->ptr = std::exchange(other.ptr, nullptr);
        }
        return *this;
    }

    value_type* operator->() { return  ptr; }
    value_type* get()        { return  ptr; }
    value_type& operator*()  { return *ptr; }

    const value_type* operator->() const { return  ptr; }
    const value_type& operator*()  const { return *ptr; }

    operator bool() const { return ptr; }

    unique_ptr(const unique_ptr<T>& ptr)            = delete;
    unique_ptr& operator=(const unique_ptr<T>& ptr) = delete;

private:
    value_type* ptr;
};

template<typename T, typename... A>
unique_ptr<T> make_unique(A&&... args)
{
    return unique_ptr<T>(new T(forward<A>(args)...));
}

};

