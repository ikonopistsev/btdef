// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/config.hpp"

namespace btdef {
namespace hash {

template<int>
struct basic_fnv1a;

// x86
template<>
struct basic_fnv1a<4>
{
    typedef std::uint32_t value_t;
    constexpr static auto salt = value_t{ 0x811c9dc5 };

    constexpr value_t operator()(const char *ptr) const noexcept
    {
        value_t hval = salt;
        while (*ptr != '\0')
        {
            hval ^= static_cast<value_t>(*ptr++);
            hval += (hval << 1) + (hval << 4) +
                (hval << 7) + (hval << 8) + (hval << 24);
        }
        return hval;
    }

    value_t calc(value_t hval, const char c) const noexcept
    {
        hval ^= static_cast<value_t>(c);
        hval += (hval << 1) + (hval << 4) +
            (hval << 7) + (hval << 8) + (hval << 24);
        return hval;
    }

    value_t operator()(std::size_t& len, const char *ptr) const noexcept
    {
        auto hval = salt;
        const char *p = ptr;
        while (*p != '\0')
        {
            hval ^= static_cast<value_t>(*p++);
            hval += (hval << 1) + (hval << 4) +
                (hval << 7) + (hval << 8) + (hval << 24);
        }
        len = static_cast<std::size_t>(p - ptr);
        return hval;
    }

    constexpr value_t operator()(const char* p, const char* e) const noexcept
    {
        auto hval = salt;
        while (p < e)
        {
            hval ^= static_cast<value_t>(*p++);
            hval += (hval << 1) + (hval << 4) +
                (hval << 7) + (hval << 8) + (hval << 24);
        }
        return hval;
    }

    constexpr value_t operator()(const void *ptr,
        std::size_t len) const noexcept
    {
        auto p = static_cast<const char*>(ptr);
        return this->operator()(p, p + len);
    }

    template<class T>
    constexpr value_t operator()(const T& text) const noexcept
    {
        return this->operator()(text.begin(), text.end());
    }
};

// x86_64
template<>
struct basic_fnv1a<8>
{
    typedef std::uint64_t value_t;
    constexpr static auto salt = value_t{ 0xcbf29ce484222325ull };

    constexpr value_t operator()(const char *ptr) const noexcept
    {
        auto hval = salt;
        while (*ptr != '\0')
        {
            hval ^= static_cast<value_t>(*ptr++);
            hval += (hval << 1) + (hval << 4) + (hval << 5) +
                (hval << 7) + (hval << 8) + (hval << 40);
        }
        return hval;
    }

    value_t calc(value_t hval, const char c) const noexcept
    {
        hval ^= static_cast<value_t>(c);
        hval += (hval << 1) + (hval << 4) + (hval << 5) +
            (hval << 7) + (hval << 8) + (hval << 40);
        return hval;
    }

    value_t operator()(std::size_t& len, const char *ptr) const noexcept
    {
        auto hval = salt;
        const char *p = ptr;
        while (*p != '\0')
        {
            hval ^= static_cast<value_t>(*p++);
            hval += (hval << 1) + (hval << 4) + (hval << 5) +
                (hval << 7) + (hval << 8) + (hval << 40);
        }
        len = static_cast<std::size_t>(p - ptr);
        return hval;
    }

    constexpr value_t operator()(const char *p, const char *e) const noexcept
    {
        auto hval = salt;
        while (p < e)
        {
            hval ^= static_cast<value_t>(*p++);
            hval += (hval << 1) + (hval << 4) + (hval << 5) +
                (hval << 7) + (hval << 8) + (hval << 40);
        }
        return hval;
    }

    constexpr value_t operator()(const void *ptr,
        std::size_t len) const noexcept
    {
        auto p = static_cast<const char*>(ptr);
        return this->operator()(p, p + len);
    }

    template<class T>
    constexpr value_t operator()(const T& text) const noexcept
    {
        return this->operator()(text.begin(), text.end());
    }
};

typedef basic_fnv1a<sizeof(std::size_t)> fnv1a;

} // namespace hash
} // namespace btdef
