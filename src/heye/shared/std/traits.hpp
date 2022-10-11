#pragma once

namespace std
{
struct false_type
{
    static constexpr bool value = false;
    constexpr operator bool() const noexcept { return value; }
};

struct true_type
{
    static constexpr bool value = true;
    constexpr operator bool() const noexcept { return value; }
};

/// @brief cpuid concepts.
///
template<typename T, typename = void>
struct has_subleaf : false_type {};

template<typename T>
struct has_subleaf<T, decltype(T::subleaf, void())> : true_type {};

template<typename T>
constexpr bool has_subleaf_v = has_subleaf<T>::value;

template<typename T, typename = void>
struct has_leaf : false_type {};

template<typename T>
struct has_leaf<T, decltype(T::leaf, void())> : true_type {};

template<typename T>
constexpr bool has_leaf_v = has_leaf<T>::value;

/// @brief MSR concepts.
///
template<typename T, typename = void>
struct has_id : false_type {};

template<typename T>
struct has_id<T, decltype(T::id, void())> : true_type {};

template<typename T>
constexpr bool has_id_v = has_id<T>::value;

/// @brief std::remove_extent
///
template<typename T>           struct remove_extent       { typedef T type; };
template<typename T>           struct remove_extent<T[]>  { typedef T type; };
template<typename T, size_t N> struct remove_extent<T[N]> { typedef T type; };

/// @brief std::is_array
///
template<typename T>           struct is_array       : false_type {};
template<typename T>           struct is_array<T[]>  : true_type  {};
template<typename T, size_t N> struct is_array<T[N]> : true_type  {};

/// @brief std::remove_reference
///
template<typename T> struct remove_reference      { typedef T type; };
template<typename T> struct remove_reference<T&>  { typedef T type; };
template<typename T> struct remove_reference<T&&> { typedef T type; };
template<typename T> using  remove_reference_t = typename remove_reference<T>::type;

/// @brief std::is_lvalue_reference
///
template<typename T> struct is_lvalue_reference     : std::false_type {};
template<typename T> struct is_lvalue_reference<T&> : std::true_type  {};
template<typename T>
constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;

/// @brief std::is_same_v
///
template<typename T, typename U> struct is_same       : false_type {};
template<typename T>             struct is_same<T, T> : true_type  {};
template<typename T, typename U>
constexpr bool is_same_v = is_same<T, U>::value;

template<size_t N, typename T>
constexpr auto countof(T(&)[N]) { return N; }

/// @brief Compile-time string compare.
///
consteval int scmp(const char* lhs, const char* rhs)
{
    return (('\0' == lhs[0]) && ('\0' == rhs[0])) ? 0
        :  (lhs[0] != rhs[0]) ? (lhs[0] - rhs[0])
        :  scmp(lhs+1, rhs+1);
}
};
