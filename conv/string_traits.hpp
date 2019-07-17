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
static std::intptr_t antout(const char *ptr) BTDEF_NOEXCEPT
{
    return detail::antout<std::intptr_t, N>::conv(ptr);
}

template<std::size_t N, typename T>
static const char* antoutp(const char *ptr, T& res) BTDEF_NOEXCEPT
{
    return detail::antout<T, N>::read(ptr, res);
}

// i - init value
template<std::size_t N, typename T>
static const char* antoutp(const char *ptr, T i, T& res) BTDEF_NOEXCEPT
{
    return detail::antout<T, N>::read(ptr, i, res);
}

// return UNSIGNED result as SIGNED value;
// if result < 0 then parse error;
static inline std::intptr_t antou(const char *ptr, std::size_t n) BTDEF_NOEXCEPT
{
    std::intptr_t res = 0;
    switch (n)
    {
        case 10: {
            auto i = ptr[n - 10] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += i * 1000000000;
        }
        case  9: {
            auto i = ptr[n - 9] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += i * 100000000;
        }
        case  8: {
            auto i = ptr[n - 8] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += i * 10000000;
        }
        case  7: {
            auto i = ptr[n - 7] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += i * 1000000;
        }
        case  6: {
            auto i = ptr[n - 6] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += i * 100000;
        }
        case  5: {
            auto i = ptr[n - 5] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += i * 10000;
        }
        case  4: {
            auto i = ptr[n - 4] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += i * 1000;
        }
        case  3: {
            auto i = ptr[n - 3] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += i * 100;
        }
        case  2: {
            auto i = ptr[n - 2] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += i * 10;
        }
        case  1: {
            auto i = ptr[n - 1] - '0';
            if ((i < 0) || (i > 9))
                return std::numeric_limits<std::intptr_t>::min();
            res += i;
        }
            break;
    default:
        res = std::numeric_limits<std::intptr_t>::min();
    }

    return res;
}

} // namespace conv
} // namespace btdef
