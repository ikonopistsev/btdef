// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include <cstdint>

namespace btdef {
namespace hash {

template<int N>
class basic_fnv1a;

// x86
template<>
class basic_fnv1a<4>
{
public:
    typedef unsigned long value_t;

private:
    const value_t salt_;

public:
    basic_fnv1a() noexcept
        : salt_(0x811c9dc5)
    {   }

    basic_fnv1a(value_t salt) noexcept
        : salt_(salt)
    {   }

    value_t operator()(const void *ptr, std::size_t len) const noexcept
    {
        value_t hval = salt_;
        auto p = static_cast<const unsigned char*>(ptr);
        auto e = p + len;
        while (p < e)
        {
            hval ^= static_cast<value_t>(*p++);
            hval += (hval << 1) + (hval << 4) +
                (hval << 7) + (hval << 8) + (hval << 24);
        }

        return hval;
    }
};

// x86_64
template<>
class basic_fnv1a<8>
{
public:
    typedef unsigned long long value_t;

private:
    const value_t salt_;

public:
    basic_fnv1a() noexcept
        : salt_(0xcbf29ce484222325ull)
    {   }

    basic_fnv1a(value_t salt) noexcept
        : salt_(salt)
    {   }

    value_t operator()(const void *ptr, std::size_t len) const noexcept
    {
        value_t hval = salt_;
        auto p = static_cast<const unsigned char*>(ptr);
        auto e = p + len;
        while (p < e)
        {
            hval ^= static_cast<value_t>(*p++);
            hval += (hval << 1) + (hval << 4) + (hval << 5) +
                (hval << 7) + (hval << 8) + (hval << 40);
        }

        return hval;
    }
};

typedef basic_fnv1a<sizeof(std::size_t)> fnv1a;

} // namespace hash
} // namespace btdef
