// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/config.hpp"

#include <limits>
#include <cstdint>
#include <type_traits>

namespace btdef {
namespace conv {
namespace detail {

template<typename T, std::size_t N>
struct antout
{
    constexpr static auto pow = 10 * antout<T, N - 1>::pow;
    constexpr static auto size = 1u + antout<T, N - 1>::size;

    static
    const char* read(const char *ptr, T& res) BTDEF_NOEXCEPT
    {
        res = convn(ptr);
        return ptr + size;
    }

    static
    const char* read(const char *ptr, T i, T& res) BTDEF_NOEXCEPT
    {
        ptr = read(ptr, res);
        res += i;
        return ptr;
    }

    static
    T convn(const char *ptr) BTDEF_NOEXCEPT
    {
        auto i = static_cast<T>(*ptr - '0');
        return i * pow + antout<T, N - 1>::convn(++ptr);
    }

    static
    T conv(const char *ptr) BTDEF_NOEXCEPT
    {
        static_assert (std::is_signed<T>::value, "type not signed");

        auto i = static_cast<std::intptr_t>(*ptr - '0');
        if ((i < 0) || (i > 9))
            return std::numeric_limits<T>::min();
        return static_cast<T>(i * pow) + antout<T, N - 1>::conv(++ptr);
    }
};

template<typename T>
struct antout<T, 1>
{
    constexpr static auto size = 1u;
    constexpr static auto pow = T{1};

    static
    const char* read(const char *ptr, T& res) BTDEF_NOEXCEPT
    {
        res = convn(ptr);
        return ptr + size;
    }

    static
    const char* read(const char *ptr, T i, T& res) BTDEF_NOEXCEPT
    {
        ptr = read(ptr, res);
        res += i;
        return ptr;
    }

    static
    T convn(const char *ptr) BTDEF_NOEXCEPT
    {
        return static_cast<T>(*ptr) - '0';
    }

    static
    T conv(const char *ptr) BTDEF_NOEXCEPT
    {
        static_assert (std::is_signed<T>::value, "type not signed");

        auto i = static_cast<std::intptr_t>(*ptr) - '0';
        return ((i < 0) || (i > 9)) ?
            std::numeric_limits<T>::min() : static_cast<T>(i);
    }
};

template<std::size_t N>
struct basic_conv;

template<std::size_t N>
struct basic_convn;

template<>
struct basic_conv<4>
{
    static inline
    std::uint32_t convn(const char *ptr, std::size_t n) BTDEF_NOEXCEPT
    {
        switch (n)
        {
            case 0x09: return detail::antout<std::uint32_t, 0x09>::convn(ptr);
            case 0x08: return detail::antout<std::uint32_t, 0x08>::convn(ptr);
            case 0x07: return detail::antout<std::uint32_t, 0x07>::convn(ptr);
            case 0x06: return detail::antout<std::uint32_t, 0x06>::convn(ptr);
            case 0x05: return detail::antout<std::uint32_t, 0x05>::convn(ptr);
            case 0x04: return detail::antout<std::uint32_t, 0x04>::convn(ptr);
            case 0x03: return detail::antout<std::uint32_t, 0x03>::convn(ptr);
            case 0x02: return detail::antout<std::uint32_t, 0x02>::convn(ptr);
            case 0x01: return detail::antout<std::uint32_t, 0x01>::convn(ptr);
            default:;
        }
        return 0;
    }

    static inline
    std::int32_t conv(const char *ptr, std::size_t n) BTDEF_NOEXCEPT
    {
        switch (n)
        {
            case 0x09: return detail::antout<std::int32_t, 0x09>::conv(ptr);
            case 0x08: return detail::antout<std::int32_t, 0x08>::conv(ptr);
            case 0x07: return detail::antout<std::int32_t, 0x07>::conv(ptr);
            case 0x06: return detail::antout<std::int32_t, 0x06>::conv(ptr);
            case 0x05: return detail::antout<std::int32_t, 0x05>::conv(ptr);
            case 0x04: return detail::antout<std::int32_t, 0x04>::conv(ptr);
            case 0x03: return detail::antout<std::int32_t, 0x03>::conv(ptr);
            case 0x02: return detail::antout<std::int32_t, 0x02>::conv(ptr);
            case 0x01: return detail::antout<std::int32_t, 0x01>::conv(ptr);
            default:;
        }
        return 0;
    }
};

template<>
struct basic_conv<8>
{
    static inline
    std::uint64_t convn(const char *ptr, std::size_t n) BTDEF_NOEXCEPT
    {
        switch (n)
        {
            case 0x12: return detail::antout<std::uint64_t, 0x12>::convn(ptr);
            case 0x11: return detail::antout<std::uint64_t, 0x11>::convn(ptr);
            case 0x10: return detail::antout<std::uint64_t, 0x10>::convn(ptr);
            case 0x0f: return detail::antout<std::uint64_t, 0x0f>::convn(ptr);
            case 0x0e: return detail::antout<std::uint64_t, 0x0e>::convn(ptr);
            case 0x0d: return detail::antout<std::uint64_t, 0x0d>::convn(ptr);
            case 0x0c: return detail::antout<std::uint64_t, 0x0c>::convn(ptr);
            case 0x0b: return detail::antout<std::uint64_t, 0x0b>::convn(ptr);
            case 0x0a: return detail::antout<std::uint64_t, 0x0a>::convn(ptr);
            case 0x09: return detail::antout<std::uint64_t, 0x09>::convn(ptr);
            case 0x08: return detail::antout<std::uint64_t, 0x08>::convn(ptr);
            case 0x07: return detail::antout<std::uint64_t, 0x07>::convn(ptr);
            case 0x06: return detail::antout<std::uint64_t, 0x06>::convn(ptr);
            case 0x05: return detail::antout<std::uint64_t, 0x05>::convn(ptr);
            case 0x04: return detail::antout<std::uint64_t, 0x04>::convn(ptr);
            case 0x03: return detail::antout<std::uint64_t, 0x03>::convn(ptr);
            case 0x02: return detail::antout<std::uint64_t, 0x02>::convn(ptr);
            case 0x01: return detail::antout<std::uint64_t, 0x01>::convn(ptr);
            default:;
        }
        return 0;
    }

    static inline
    std::int64_t conv(const char *ptr, std::size_t n) BTDEF_NOEXCEPT
    {
        switch (n)
        {
            case 0x12: return detail::antout<std::int64_t, 0x12>::conv(ptr);
            case 0x11: return detail::antout<std::int64_t, 0x11>::conv(ptr);
            case 0x10: return detail::antout<std::int64_t, 0x10>::conv(ptr);
            case 0x0f: return detail::antout<std::int64_t, 0x0f>::conv(ptr);
            case 0x0e: return detail::antout<std::int64_t, 0x0e>::conv(ptr);
            case 0x0d: return detail::antout<std::int64_t, 0x0d>::conv(ptr);
            case 0x0c: return detail::antout<std::int64_t, 0x0c>::conv(ptr);
            case 0x0b: return detail::antout<std::int64_t, 0x0b>::conv(ptr);
            case 0x0a: return detail::antout<std::int64_t, 0x0a>::conv(ptr);
            case 0x09: return detail::antout<std::int64_t, 0x09>::conv(ptr);
            case 0x08: return detail::antout<std::int64_t, 0x08>::conv(ptr);
            case 0x07: return detail::antout<std::int64_t, 0x07>::conv(ptr);
            case 0x06: return detail::antout<std::int64_t, 0x06>::conv(ptr);
            case 0x05: return detail::antout<std::int64_t, 0x05>::conv(ptr);
            case 0x04: return detail::antout<std::int64_t, 0x04>::conv(ptr);
            case 0x03: return detail::antout<std::int64_t, 0x03>::conv(ptr);
            case 0x02: return detail::antout<std::int64_t, 0x02>::conv(ptr);
            case 0x01: return detail::antout<std::int64_t, 0x01>::conv(ptr);
            case 0x00: return 0;
            default:;
        }
        return std::numeric_limits<std::intptr_t>::min();
    }
};

} // namespace detail

static inline
std::intptr_t antoi(const char *ptr, std::size_t n) BTDEF_NOEXCEPT
{
    return detail::basic_conv<sizeof(std::intptr_t)>::conv(ptr, n);
}

static inline
std::size_t antoin(const char *ptr, std::size_t n) BTDEF_NOEXCEPT
{
    return detail::basic_conv<sizeof(std::size_t)>::convn(ptr, n);
}

static inline
std::int64_t antou(const char *ptr, std::size_t n) BTDEF_NOEXCEPT
{
    return detail::basic_conv<sizeof(std::int64_t)>::conv(ptr, n);
}

static inline
std::uint64_t antoun(const char *ptr, std::size_t n) BTDEF_NOEXCEPT
{
    return detail::basic_conv<sizeof(std::uint64_t)>::convn(ptr, n);
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

} // namespace conv
} // namespace btdef
