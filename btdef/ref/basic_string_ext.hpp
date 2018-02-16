// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/ref/basic_string.hpp"
#include "btdef/hash/fnv1a.hpp"

#include <string>

namespace std {

template<>
struct hash<btref::basic_string<char>>
{
    typedef size_t result_type;
    typedef btref::basic_string<char> argument_type;

    size_t operator()(argument_type str) const noexcept
    {
        btdef::hash::fnv1a fn;
        return static_cast<size_t>(fn(str.data(), str.size()));
    }
};

} // namespace std

// ---- std::basic_string

template<typename C, typename T, typename A>
inline std::basic_string<C, T, A>&
    operator+=(std::basic_string<C, T, A>& lhs,
        const btref::basic_string<C>& rhs)
{
    return lhs.append(rhs.data(), rhs.size());
}

template<typename C, typename T, typename A>
inline std::basic_string<C, T, A>
    operator+(const std::basic_string<C, T, A>& lhs,
        const btref::basic_string<C>& rhs)
{
    std::basic_string<C, T, A> result(lhs);
    result.append(rhs.data(), rhs.size());
    return result;
}

template<typename C, typename T, typename A>
inline std::basic_string<C, T, A>
    operator+(std::basic_string<C, T, A>&& lhs,
        const btref::basic_string<C>& rhs)
{
    return std::move(lhs.append(rhs.data(), rhs.size()));
}

template<typename C, typename T>
std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& os,
    const btref::basic_string<C>& rhs)
{
    return os.write(rhs.data(), rhs.size());
}

// ---- std::basic_string

template<typename C, typename T, typename A>
bool operator==(const std::basic_string<C, T, A>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return rhs == lhs;
}

template<typename C, typename T, typename A>
bool operator!=(const std::basic_string<C, T, A>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<typename C, typename T, typename A>
bool operator<(const std::basic_string<C, T, A>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return rhs > lhs;
}

template<typename C, typename T, typename A>
bool operator>(const std::basic_string<C, T, A>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return rhs < lhs;
}

template<typename C, typename T, typename A>
bool operator<=(const std::basic_string<C, T, A>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return !(rhs < lhs);
}

template<typename C, typename T, typename A>
bool operator>=(const std::basic_string<C, T, A>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return !(lhs < rhs);
}

// ---- std::basic_string
