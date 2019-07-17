// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/util/time.hpp"
#include "btdef/num/itoa.hpp"
#include "btdef/util/text.hpp"
#include "btdef/util/string.hpp"
#include "btdef/conv/string_traits.hpp"

#include <utility>
#include <clocale>
#include <string>
#include <new>

namespace btdef {
namespace util {

class tm
{
public:
    typedef unsigned short millisecond_t;
    typedef std::pair<std::tm, millisecond_t> tm_t;

protected:
    std::tm tm_;
    millisecond_t millisecond_;

    enum {
        buffer_size = 64,
        yd = 1900,
        md = 1
    };

    tm() BTDEF_NOEXCEPT
        : tm_(time::empty_tm())
        , millisecond_(0)
    {   }

public:
    tm(const std::tm& stdtm, millisecond_t ms) BTDEF_NOEXCEPT
        : tm_(stdtm)
        , millisecond_(ms)
    {  }

    tm(const tm_t& val) BTDEF_NOEXCEPT
        : tm_(val.first)
        , millisecond_(val.second)
    {   }

    std::intptr_t year() const BTDEF_NOEXCEPT
    {
        return tm_.tm_year + yd;
    }

    std::intptr_t mon() const BTDEF_NOEXCEPT
    {
        return tm_.tm_mon + md;
    }

    std::intptr_t mday() const BTDEF_NOEXCEPT
    {
        return tm_.tm_mday;
    }

    std::intptr_t wday() const BTDEF_NOEXCEPT
    {
        return tm_.tm_wday;
    }

    std::intptr_t yday() const BTDEF_NOEXCEPT
    {
        return tm_.tm_yday;
    }

    std::intptr_t hour() const BTDEF_NOEXCEPT
    {
        return tm_.tm_hour;
    }

    std::intptr_t min() const BTDEF_NOEXCEPT
    {
        return tm_.tm_min;
    }

    std::intptr_t sec() const BTDEF_NOEXCEPT
    {
        return tm_.tm_sec;
    }

    millisecond_t msec() const BTDEF_NOEXCEPT
    {
        return millisecond_;
    }

    const std::tm& data() const BTDEF_NOEXCEPT
    {
        return tm_;
    }

    char* put_date_json(char *p) const BTDEF_NOEXCEPT
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

    char* put_time_json(char *p) const BTDEF_NOEXCEPT
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

    char* put_json(char *p) const BTDEF_NOEXCEPT
    {
        p = put_date_json(p);
        *p++ = 'T';
        p = put_time_json(p);
        *p++ = 'Z';

        return p;
    }

    std::string to_json() const
    {
        util::text t = json_text();
        return std::string(t.data(), t.size());
    }

    util::string json() const
    {
        util::string result;
        util::string::runtime_error(result.reserve(32), "bad alloc");

        char* p = result.data();
        result.increase(static_cast<std::size_t>(
            std::distance(p, put_json(p))));

        return result;
    }

    util::text json_text() const BTDEF_NOEXCEPT
    {
        util::text result;
        char* p = result.data();
        result.increase(static_cast<std::size_t>(
            std::distance(p, put_json(p))));
        return result;
    }

    std::string to_str(const char *fmt, std::size_t fmt_len) const
    {
        assert(fmt);
        util::string s = str(fmt, fmt_len);
        return std::string(s.data(), s.size());
    }

    template<std::size_t N>
    std::string to_str(std::reference_wrapper<const char[N]> ref) const
    {
        return to_str(ref.get(), N - 1);
    }

    std::string to_str(const char *fmt) const
    {
        assert(fmt);
        return to_str(fmt, std::strlen(fmt));
    }

    util::string str(const char *fmt, std::size_t fmt_len) const
    {
        assert(fmt);

        util::string result;
        util::string::runtime_error(result.reserve(fmt_len + buffer_size),
                                                                "bad alloc");
        std::size_t count = std::strftime(result.data(),
            result.capacity() + 1, fmt, &tm_);
        if (!count && fmt_len)
            throw std::runtime_error("strftime");

        result.increase(count);
        return result;
    }

    template<std::size_t N>
    std::string str(std::reference_wrapper<const char[N]> ref) const
    {
        return str(ref.get(), N - 1);
    }

    util::string str(const char *fmt) const
    {
        assert(fmt);
        return str(fmt, std::strlen(fmt));
    }

    util::text text(const char *fmt) const BTDEF_NOEXCEPT
    {
        assert(fmt);

        util::text result;
        result.increase(std::strftime(result.data(),
            result.capacity() + 1, fmt, &tm_));

        return result;
    }

    std::string to_string() const
    {
        util::text t = text();
        return std::string(t.data(), t.size());
    }

    util::string string() const
    {
        return str("%a, %d %b %Y %H:%M:%S GMT");
    }

    util::text text() const BTDEF_NOEXCEPT
    {
        return text("%a, %d %b %Y %H:%M:%S GMT");
    }

    util::text millisecond() const BTDEF_NOEXCEPT
    {
        using num::detail::itoa3zf;

        util::text result;
        char* p = result.data();
        result.increase(static_cast<std::size_t>(
            std::distance(p, itoa3zf(millisecond_,p))));
        return result;
    }

    std::string to_locale() const
    {
        util::text t = locale_text();
        return std::string(t.data(), t.size());
    }

    util::text locale_text() const BTDEF_NOEXCEPT
    {
        return text("%x, %X");
    }

    util::string locale() const
    {
        return str("%x, %X");
    }

    util::text zone() const BTDEF_NOEXCEPT
    {
        return util::text(std::cref("+0000"));
    }

    util::text zonename() const BTDEF_NOEXCEPT
    {
        return util::text(std::cref("GMT"));
    }
};

} // namespace util
} // namespace btdef
