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
    typedef std::uint32_t value_t;

private:
    const value_t salt_{ 0x811c9dc5 };

public:
    basic_fnv1a() = default;

    basic_fnv1a(value_t salt) noexcept
        : salt_(salt)
    {   }

    value_t operator()(const char *ptr) const noexcept
    {
        value_t hval = salt_;
        while (*ptr != '\0')
        {
            hval ^= static_cast<value_t>(*ptr++);
            hval += (hval << 1) + (hval << 4) +
                (hval << 7) + (hval << 8) + (hval << 24);
        }
        return hval;
    }

    value_t operator()(std::size_t& len, const char *ptr) const noexcept
    {
        value_t hval = salt_;
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

    value_t operator()(const char* p, const char* e) const noexcept
    {
        value_t hval = salt_;
        while (p < e)
        {
            hval ^= static_cast<value_t>(*p++);
            hval += (hval << 1) + (hval << 4) +
                (hval << 7) + (hval << 8) + (hval << 24);
        }
        return hval;
    }

    value_t operator()(const void *ptr, std::size_t len) const noexcept
    {
        auto p = static_cast<const char*>(ptr);
        return this->operator()(p, p + len);
    }

    template<class T>
    value_t operator()(const T& text) const noexcept
    {
        return this->operator()(text.data(), text.size());
    }
};

// x86_64
template<>
class basic_fnv1a<8>
{
public:
    typedef std::uint64_t value_t;

private:
    const value_t salt_{ 0xcbf29ce484222325ull };

public:
    basic_fnv1a() = default;

    basic_fnv1a(value_t salt) noexcept
        : salt_(salt)
    {   }

    value_t operator()(const char *ptr) const noexcept
    {
        value_t hval = salt_;
        while (*ptr != '\0')
        {
            hval ^= static_cast<value_t>(*ptr++);
            hval += (hval << 1) + (hval << 4) + (hval << 5) +
                (hval << 7) + (hval << 8) + (hval << 40);
        }
        return hval;
    }

    value_t operator()(std::size_t& len, const char *ptr) const noexcept
    {
        value_t hval = salt_;
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

    value_t operator()(const char *p, const char *e) const noexcept
    {
        value_t hval = salt_;
        while (p < e)
        {
            hval ^= static_cast<value_t>(*p++);
            hval += (hval << 1) + (hval << 4) + (hval << 5) +
                (hval << 7) + (hval << 8) + (hval << 40);
        }
        return hval;
    }

    value_t operator()(const void *ptr, std::size_t len) const noexcept
    {
        auto p = static_cast<const char*>(ptr);
        return this->operator()(p, p + len);
    }

    template<class T>
    value_t operator()(const T& text) const noexcept
    {
        return this->operator()(text.data(), text.size());
    }
};

typedef basic_fnv1a<sizeof(std::size_t)> fnv1a;

} // namespace hash
} // namespace btdef
