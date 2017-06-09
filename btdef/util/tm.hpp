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
#include <new>

namespace btdef {
namespace util {

class tm
{
public:
    typedef unsigned short millisecond_t;
    typedef std::pair<std::tm, millisecond_t> tm_t;

protected:

    enum {
        md = 1,
        yd = 1900,
        buffer_size = 160,
    };

    std::tm tm_ = time::empty_tm();
    millisecond_t millisecond_{ 0 };

public:
    enum part
    {
        Y,
        M,
        D,
        h,
        m,
        s,
        ms
    };

    tm() = default;

    tm(const std::tm& stdtm, millisecond_t millisecond) noexcept
        : tm_(stdtm)
        , millisecond_(millisecond)
    {  }

    tm(const tm_t& tmt) noexcept
        : tm_(tmt.first)
        , millisecond_(tmt.second)
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

    std::intptr_t msec() const noexcept
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

    std::string to_json() const
    {
        util::text t = json_text();
        return std::string(t.data(), t.size());
    }

    util::string json() const
    {
        util::string result;

        if (!result.reserve(32))
            throw std::bad_alloc();

        char* p = result.data();
        result.increase(std::distance(p, put_json(p)));

        return result;
    }

    util::text json_text() const noexcept
    {
        util::text result;
        char* p = result.data();
        result.increase(std::distance(p, put_json(p)));
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

        if (!result.reserve(fmt_len + buffer_size))
            throw std::bad_alloc();

        result.increase(std::strftime(result.data(),
            result.capacity() + 1, fmt, &tm_));

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

    util::text text(const char *fmt) const noexcept
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

    util::text text() const noexcept
    {
        return text("%a, %d %b %Y %H:%M:%S GMT");
    }

    util::text millisecond() const noexcept
    {
        using num::detail::itoa3zf;

        util::text result;
        char* p = result.data();
        result.increase(std::distance(p, itoa3zf(millisecond_,p)));
        return result;
    }

    class lc_time
    {
        util::text prev_;

        static inline util::text create() noexcept
        {
            const char *time_locale = std::setlocale(LC_TIME, nullptr);
            if (time_locale)
                return util::text(time_locale);

            return util::text("C");
        }

    public:

        lc_time(const util::text& l)
            : prev_(create())
        {
            std::setlocale(LC_TIME, l.c_str());
        }

        ~lc_time()
        {
            std::setlocale(LC_TIME, prev_.c_str());
        }
    };

    std::string to_locale() const
    {
        util::text t = locale_text();
        return std::string(t.data(), t.size());
    }

    util::text locale_text() const noexcept
    {
        return text("%x, %X");
    }

    util::string locale() const
    {
        return str("%x, %X");
    }

    util::text zone() const noexcept
    {
        return util::text(std::cref("+0000"));
    }

    util::text zonename() const noexcept
    {
        return util::text(std::cref("GMT"));
    }
};

} // namespace util
} // namespace btdef
