#pragma once

#include "btdef/util/basic_text.hpp"
#include "btdef/hash/fnv1a.hpp"

#include <string>

namespace std {

template<size_t N>
struct hash<btdef::util::basic_text<char, N>>
{
    size_t operator()(const btdef::util::basic_text<char, N>& t) const noexcept
    {
        btdef::hash::fnv1a fn;
        return static_cast<size_t>(fn(t.data(), t.size()));
    }
};

} // namespace std

// ---- std::basic_string

template<typename C, typename T, typename A1, std::size_t N>
inline std::basic_string<C, T, A1>&
    operator+=(std::basic_string<C, T, A1>& lhs,
        const btdef::util::basic_text<C, N>& rhs)
{
    return lhs.append(rhs.data(), rhs.size());
}

template<typename C, typename T, typename A1, std::size_t N>
inline std::basic_string<C, T, A1>
    operator+(const std::basic_string<C, T, A1>& lhs,
        const btdef::util::basic_text<C, N>& rhs)
{
    std::basic_string<C, T, A1> result(lhs);
    result.append(rhs.data(), rhs.size());
    return result;
}

template<typename C, typename T, typename A1, std::size_t N>
inline std::basic_string<C, T, A1>
    operator+(std::basic_string<C, T, A1>&& lhs,
        const btdef::util::basic_text<C, N>& rhs)
{
    return std::move(lhs.append(rhs.data(), rhs.size()));
}

template<class C, class T, std::size_t N>
std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& os,
    const btdef::util::basic_text<C, N>& rhs)
{
    return os.write(rhs.data(), rhs.size());
}

// ---- std::basic_string

template<typename C, typename T, typename A, std::size_t N>
bool operator==(const std::basic_string<C, T, A>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return rhs == lhs;
}

template<typename C, typename T, typename A, std::size_t N>
bool operator!=(const std::basic_string<C, T, A>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<typename C, typename T, typename A, std::size_t N>
bool operator<(const std::basic_string<C, T, A>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return rhs > lhs;
}

template<typename C, typename T, typename A, std::size_t N>
bool operator>(const std::basic_string<C, T, A>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return rhs < lhs;
}

template<typename C, typename T, typename A, std::size_t N>
bool operator<=(const std::basic_string<C, T, A>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return !(rhs < lhs);
}

template<typename C, typename T, typename A, std::size_t N>
bool operator>=(const std::basic_string<C, T, A>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return !(lhs < rhs);
}

// ---- std::basic_string
