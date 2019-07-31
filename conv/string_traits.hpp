// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/config.hpp"

#include <limits>

namespace btdef {
namespace conv {
namespace detail {

template<typename T, std::size_t N>
struct antout
{
    static constexpr auto pow = 10 * antout<T, N - 1>::pow;
    static constexpr auto size = 1u + antout<T, N - 1>::size;

    static const char* read(const char *ptr, T& res) BTDEF_NOEXCEPT
    {
        res = static_cast<T>(conv(ptr));
        return ptr + size;
    }

    static const char* read(const char *ptr, T i, T& res) BTDEF_NOEXCEPT
    {
        ptr = read(ptr, res);
        res += i;
        return ptr;
    }

    static std::intptr_t conv(const char *ptr) BTDEF_NOEXCEPT
    {
        auto i = *ptr++ - '0';
        if ((i < 0) || (i > 9))
            return std::numeric_limits<T>::min();
        return i * pow + antout<T, N - 1>::conv(ptr);
    }
};

template<typename T>
struct antout<T, 1>
{
    static constexpr auto pow = 1;
    static constexpr auto size = 1u;

    static const char* read(const char *ptr, T& res) BTDEF_NOEXCEPT
    {
        res = static_cast<T>(conv(ptr));
        return ptr + size;
    }

    static const char* read(const char *ptr, T i, T& res) BTDEF_NOEXCEPT
    {
        ptr = read(ptr, res);
        res += i;
        return ptr;
    }

    static std::intptr_t conv(const char *ptr) BTDEF_NOEXCEPT
    {
        auto i = *ptr - '0';
        if ((i < 0) || (i > 9))
            return std::numeric_limits<T>::min();
        return i;
    }
};

} // namespace detail

// return UNSIGNED result as SIGNED value;
// if result < 0 then parse error;
template<std::size_t N>
std::intptr_t antout(const char *ptr) BTDEF_NOEXCEPT
{
    return detail::antout<std::intptr_t, N>::conv(ptr);
}

template<std::size_t N, typename T>
const char* antoutp(const char *ptr, T& res) BTDEF_NOEXCEPT
{
    return detail::antout<T, N>::read(ptr, res);
}

// i - init value
template<std::size_t N, typename T>
const char* antoutp(const char *ptr, T i, T& res) BTDEF_NOEXCEPT
{
    return detail::antout<T, N>::read(ptr, i, res);
}

// return UNSIGNED result as SIGNED value;
// if result < 0 then parse error;
static inline std::intptr_t antou(const char *ptr, std::size_t n) BTDEF_NOEXCEPT
{
    std::uintptr_t res = 0;
    switch (n)
    {
        case 10: {
            auto i = ptr[n - 10] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += static_cast<std::size_t>(i) * 1000000000u;
        }
        case 9: {
            auto i = ptr[n - 9] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += static_cast<std::size_t>(i) * 100000000u;
        }
        case 8: {
            auto i = ptr[n - 8] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += static_cast<std::size_t>(i) * 10000000u;
        }
        case 7: {
            auto i = ptr[n - 7] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += static_cast<std::size_t>(i) * 1000000u;
        }
        case 6: {
            auto i = ptr[n - 6] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += static_cast<std::size_t>(i) * 100000u;
        }
        case 5: {
            auto i = ptr[n - 5] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += static_cast<std::size_t>(i) * 10000u;
        }
        case 4: {
            auto i = ptr[n - 4] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += static_cast<std::size_t>(i) * 1000u;
        }
        case 3: {
            auto i = ptr[n - 3] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += static_cast<std::size_t>(i) * 100u;
        }
        case 2: {
            auto i = ptr[n - 2] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += static_cast<std::size_t>(i) * 10u;
        }
        case 1: {
            auto i = ptr[n - 1] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += static_cast<std::size_t>(i);
        }
            break;
    default:
        return std::numeric_limits<std::intptr_t>::min();
    }

    return static_cast<std::intptr_t>(res);
}

} // namespace conv
} // namespace btdef
