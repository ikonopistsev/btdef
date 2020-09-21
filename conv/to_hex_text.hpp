// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/conv/to_hex.hpp"
#include "btdef/util/text.hpp"

#ifdef WIN32
#include <Winsock2.h>
#else
#include <arpa/inet.h>
#endif // WIN32

namespace btdef {
namespace conv {

static inline btdef::util::text to_hex(const char *ptr,
    std::size_t len) BTDEF_NOEXCEPT
{
    btdef::util::text rc;

    assert(ptr);
    assert(len < (rc.capacity() / 2));

    auto end = ptr + len;

    while (ptr < end)
        rc += to_hex(*ptr++);

    return rc;
}

static inline btdef::util::text to_hex(std::uint64_t val) BTDEF_NOEXCEPT
{
    btdef::util::text rc;
#ifndef htonll
    val = (((static_cast<std::uint64_t>(htonl(val & 0xffffffff)) << 32) |
        htonl(static_cast<std::uint32_t>((val) >> 32))));
#undef htonll
#else
    val = htonll(val);
#endif // htonll

    do
    {
        rc += to_hex(static_cast<unsigned char>(val & 0xff));
        val >>= 8;
    } while (val);

    return rc;
}

static inline btdef::util::text to_hex(std::uint32_t val) BTDEF_NOEXCEPT
{

    btdef::util::text rc;
    val = htonl(val);
    do
    {
        rc += to_hex(static_cast<unsigned char>(val & 0xff));
        val >>= 8;
    } while (val);

    return rc;
}

static inline btdef::util::text to_hex(std::uint16_t val) BTDEF_NOEXCEPT
{

    btdef::util::text rc;
    val = htons(val);
    do
    {
        rc += to_hex(static_cast<unsigned char>(val & 0xff));
        val >>= 8;
    } while (val);

    return rc;
}

static inline btdef::util::text to_hex(std::int64_t val) BTDEF_NOEXCEPT
{
    return to_hex(static_cast<std::uint64_t>(val));
}

static inline btdef::util::text to_hex(std::int32_t val) BTDEF_NOEXCEPT
{
    return to_hex(static_cast<std::uint64_t>(val));
}

static inline btdef::util::text to_hex(std::int16_t val) BTDEF_NOEXCEPT
{
    return to_hex(static_cast<std::uint16_t>(val));
}

} // namespace conv
} // namespace btdef
