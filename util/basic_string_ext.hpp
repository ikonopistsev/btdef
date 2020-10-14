#pragma once

#include "btdef/util/basic_string.hpp"
#include "btdef/hash/fnv1a.hpp"

#include <string>
#include <string_view>

namespace std {

template<class A>
struct hash<btdef::util::basic_string<char, A>>
{
    size_t
    operator()(const btdef::util::basic_string<char, A>& str) const BTDEF_NOEXCEPT
    {
        btdef::hash::fnv1a fn;
        return static_cast<size_t>(fn(str.data(), str.size()));
    }
};

template<typename T, std::size_t N>
std::basic_string<T> mkstr(
    std::reference_wrapper<const T[N]> text_ref)
{
    return std::basic_string<T>(text_ref.get(), N - 1);
}

template<typename T, std::size_t N>
std::basic_string<T> mkstr(
    std::reference_wrapper<const T[N]> text_ref,
    const std::size_t size)
{
    std::basic_string<T> result;
    result.reserve(size > N ? size : N);
    result.assign(text_ref.get(), N - 1);
    return result;
}

} // namespace std

namespace btdef {
namespace util {

template<typename T, std::size_t N>
basic_string<T, allocator::basic<T>> mkstr(
    std::reference_wrapper<const T[N]> text_ref)
{
    return basic_string<T, allocator::basic<T>>(text_ref.get(), N - 1);
}

template<typename T, std::size_t N>
basic_string<T, allocator::basic<T>> mkstr(
    std::reference_wrapper<const T[N]> text_ref,
    const std::size_t size)
{
    basic_string<T, allocator::basic<T>> result;
    result.reserve_ex(size > N ? size : N);
    result.assign(text_ref.get(), N - 1);
    return result;
}

} // namespace util
} // namespace btdef

// ---- std::basic_string

template<typename C, typename T, typename A1, typename A2>
inline std::basic_string<C, T, A1>&
    operator+=(std::basic_string<C, T, A1>& lhs,
        const btdef::util::basic_string<C, A2>& rhs)
{
    return lhs.append(rhs.data(), rhs.size());
}

template<typename C, typename T, typename A1, typename A2>
inline std::basic_string<C, T, A1>
    operator+(const std::basic_string<C, T, A1>& lhs,
        const btdef::util::basic_string<C, A2>& rhs)
{
    std::basic_string<C, T, A1> result(lhs);
    result.append(rhs.data(), rhs.size());
    return result;
}

template<typename C, typename T, typename A1, typename A2>
inline std::basic_string<C, T, A1>
    operator+(std::basic_string<C, T, A1>&& lhs,
        const btdef::util::basic_string<C, A2>& rhs)
{
    return std::move(lhs.append(rhs.data(), rhs.size()));
}

template<class C, class T, class A>
std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& os,
    const btdef::util::basic_string<C, A>& rhs)
{
    return os.write(rhs.data(), rhs.size());
}

// ---- std::basic_string

template<typename C, typename T, typename A1, typename A2>
bool operator==(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) BTDEF_NOEXCEPT
{
    return rhs == lhs;
}

template<typename C, typename T, typename A1, typename A2>
bool operator!=(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) BTDEF_NOEXCEPT
{
    return !(lhs == rhs);
}

template<typename C, typename T, typename A1, typename A2>
bool operator<(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) BTDEF_NOEXCEPT
{
    return rhs > lhs;
}

template<typename C, typename T, typename A1, typename A2>
bool operator>(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) BTDEF_NOEXCEPT
{
    return rhs < lhs;
}

template<typename C, typename T, typename A1, typename A2>
bool operator<=(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) BTDEF_NOEXCEPT
{
    return !(rhs < lhs);
}

template<typename C, typename T, typename A1, typename A2>
bool operator>=(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) BTDEF_NOEXCEPT
{
    return !(lhs < rhs);
}

// ---- std::basic_string
