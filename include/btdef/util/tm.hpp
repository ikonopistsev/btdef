// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/util/time.hpp"
#include "btdef/num/itoa.hpp"
#include "btdef/util/text.hpp"
#include "btdef/conv/string_traits.hpp"

#include <utility>
#include <clocale>
#include <new>

namespace btdef {
namespace util {

class tm
{
public:
    typedef unsigned short millisecond_t;

protected:
    std::tm tm_{};
    millisecond_t millisecond_{};

    enum {
        buffer_size = 64,
        yd = 1900,
        md = 1
    };

public:
    tm() = default;
    tm(const tm&) = default;
    tm& operator=(const tm&) = default;

    tm(const std::tm& stdtm, millisecond_t ms) noexcept
        : tm_{stdtm}
        , millisecond_{ms}
    {  }

    template<class T1, class T2>
    tm(const std::pair<T1, T2>& p) noexcept
        : tm_{p.first}
        , millisecond_{p.second}
    {   }

    std::intptr_t year() const noexcept
    {
        return tm_.tm_year + yd;
    }

    std::intptr_t mon() const noexcept
    {
        return tm_.tm_mon + md;
    }

    std::intptr_t mday() const noexcept
    {
        return tm_.tm_mday;
    }

    std::intptr_t wday() const noexcept
    {
        return tm_.tm_wday;
    }

    std::intptr_t yday() const noexcept
    {
        return tm_.tm_yday;
    }

    std::intptr_t hour() const noexcept
    {
        return tm_.tm_hour;
    }

    std::intptr_t min() const noexcept
    {
        return tm_.tm_min;
    }

    std::intptr_t sec() const noexcept
    {
        return tm_.tm_sec;
    }

    millisecond_t msec() const noexcept
    {
        return millisecond_;
    }

    const std::tm& data() const noexcept
    {
        return tm_;
    }

    char* put_date_json(char *p) const noexcept
    {
        using num::detail::itoa2zf;
        using num::detail::itoa4zf;

        p = itoa4zf(static_cast<std::uint32_t>(tm_.tm_year) + yd, p);
        *p++ = '-';
        p = itoa2zf(static_cast<std::uint32_t>(tm_.tm_mon) + md, p);
        *p++ = '-';
        p = itoa2zf(static_cast<std::uint32_t>(tm_.tm_mday), p);

        return p;
    }

    char* put_time_json(char *p) const noexcept
    {
        using num::detail::itoa2zf;
        using num::detail::itoa3zf;

        p = itoa2zf(static_cast<std::uint32_t>(tm_.tm_hour), p);
        *p++ = ':';
        p = itoa2zf(static_cast<std::uint32_t>(tm_.tm_min), p);
        *p++ = ':';
        p = itoa2zf(static_cast<std::uint32_t>(tm_.tm_sec), p);
        *p++ = '.';
        p = itoa3zf(millisecond_, p);

        return p;
    }

    char* put_json(char *p) const noexcept
    {
        p = put_date_json(p);
        *p++ = 'T';
        p = put_time_json(p);
        *p++ = 'Z';

        return p;
    }

    util::text to_json() const noexcept
    {
        util::text result;
        char* p = result.data();
        result.resize(static_cast<std::size_t>(
            std::distance(p, put_json(p))));
        return result;
    }

    util::text text(const char *fmt) const noexcept
    {
        assert(fmt);

        util::text result;
        result.resize(std::strftime(result.data(),
            util::text::cache_size, fmt, &tm_));

        return result;
    }

    util::text text() const noexcept
    {
        return text("%a, %d %b %Y %H:%M:%S GMT");
    }

    util::text millisecond() const noexcept
    {
        using num::detail::itoa3zf;

        util::text result;
        char* p = result.data();
        result.resize(static_cast<std::size_t>(
            std::distance(p, itoa3zf(millisecond_, p))));
        return result;
    }

    util::text locale() const noexcept
    {
        return text("%x, %X");
    }
};

} // namespace util
} // namespace btdef
