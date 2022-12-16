// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/conv/to_hex.hpp"
#include "btdef/util/text.hpp"
#include <type_traits>

#ifdef _WIN32
#include <Winsock2.h>
#else
#include <arpa/inet.h>
#endif // WIN32

namespace btdef {
namespace conv {

template<class, int>
struct cast_conv;

template<class T>
struct cast_conv<T, 1>
{
    static inline auto hton(T val)
    {
        return static_cast<std::uint8_t>(val);
    }
};

template<class T>
struct cast_conv<T, 2>
{
    static inline auto hton(T val)
    {
        return static_cast<std::uint16_t>(
            htons(static_cast<std::uint16_t>(val)));
    }
};

template<class T>
struct cast_conv<T, 4>
{
    static inline auto hton(T val)
    {
        return static_cast<std::uint32_t>(
            htonl(static_cast<std::uint32_t>(val)));
    }
};

template<class T>
struct cast_conv<T, 8>
{
    static inline auto hton(T val)
    {
#ifndef htonll
        val = (((static_cast<std::uint64_t>(htonl(val & 0xffffffff)) << 32) |
            htonl(static_cast<std::uint32_t>((val) >> 32))));
#undef htonll
#else
        val = htonll(val);
#endif // htonll
        return static_cast<std::uint64_t>(val);
    }
};

template<class T,
    typename std::enable_if_t<std::is_unsigned<T>::value, int> = 1>
auto cv_htount(T value)
{
    return cast_conv<T, sizeof(value)>::hton(value);
}

template<class T,
    typename std::enable_if_t<std::is_signed<T>::value, int> = 1>
auto cv_htount(T value)
{
    using type = typename std::make_unsigned<T>::type;
    return cv_htount(static_cast<type>(value));
}

template<class T>
void to_hex_print(T& rc, std::uint8_t val)
{
    rc += to_hex(static_cast<unsigned char>(val));
}

template<class T, class V,
    typename std::enable_if_t<std::numeric_limits<V>::is_integer, int> = 1>
void to_hex_print(T& rc, V value)
{
    auto val = cv_htount(value);

    if (!val)
        rc += to_hex(static_cast<unsigned char>(val));
    else do {
        auto v = static_cast<unsigned char>(val & 0xff);
        if (v)
            rc += to_hex(v);
        val >>= 8;
    } while (val);
}

template<class T>
void to_hex00_print(T& rc, std::uint8_t val)
{
    rc += to_hex(static_cast<unsigned char>(val));
}

template<class T, class V,
    typename std::enable_if_t<std::numeric_limits<V>::is_integer, int> = 1>
void to_hex00_print(T& rc, V value)
{
    auto val = cv_htount(value);

    if (!val)
    {
        constexpr static std::string_view t = "0000000000000000";
        rc += t.substr(0, sizeof(val) * 2);
    }
    else do {
        rc += to_hex(static_cast<unsigned char>(val & 0xff));
        val >>= 8;
    } while (val);
}

template<class V>
auto to_hex(V value)
{
    btdef::util::text rc;
    to_hex_print(rc, value);
    return rc;
}

template<class T, class V>
auto to_hex(T prefix, V value)
{
    btdef::util::text rc;
    rc += prefix;
    to_hex_print(rc, value);
    return rc;
}

template<class V>
auto to_hex00(V value)
{
    btdef::util::text rc;
    to_hex00_print(rc, value);
    return rc;
}

template<class T, class V>
auto to_hex00(T prefix, V value)
{
    btdef::util::text rc;
    rc += prefix;
    to_hex00_print(rc, value);
    return rc;
}

template<class T>
void to_hex_print(T& rc, const char *ptr, std::size_t len) noexcept
{
    assert(ptr);

    auto end = ptr + len;
    while (ptr < end)
        rc += to_hex(*ptr++);
}

static inline auto to_hex(const char *ptr, std::size_t len) noexcept
{
    btdef::util::text rc;
    assert(len < (rc.capacity() / 2));

    to_hex_print(rc, ptr, len);

    return rc;
}

} // namespace conv
} // namespace btdef
