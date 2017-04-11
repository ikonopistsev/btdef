// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/util/time.hpp"
#include "btdef/num/itoa.hpp"
#include "btdef/conv/string_traits.hpp"

#include <vector>
#include <utility>
#include <clocale>

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
        buffer_size = 64u
    };

    std::tm tm_ = time::empty_tm();
    millisecond_t millisecond_{ 0 };

    class mut
    {
    protected:
        std::intptr_t val_;

    public:
        explicit mut(std::intptr_t val) noexcept
            : val_(val)
        {   }

        std::intptr_t get() const noexcept
        {
            return val_;
        }

        operator std::intptr_t() const noexcept
        {
            return val_;
        }
    };

    static inline std::size_t strfmt(const std::tm& tms,
        const char *fmt, char *buffer, std::size_t buffer_len) noexcept
    {
        return std::strftime(buffer, buffer_len, fmt, &tms);
    }

    template<std::size_t buffer_len>
    static inline std::size_t strfmt(const std::tm& tms,
        const char *fmt, char(&buffer)[buffer_len]) noexcept
    {
        return strfmt(tms, fmt, buffer, buffer_len);
    }

    template<std::size_t fmt_len>
    static inline std::string put_time(const std::tm& tms,
        const char(&fmt)[fmt_len]) noexcept
    {
        char buffer[buffer_size + fmt_len];
        auto size = strfmt(tms, fmt, buffer);
        return std::string(buffer, size);
    }

    static inline std::string put_time(const std::tm& tms,
        const char *fmt, std::size_t fmt_len) noexcept
    {
        std::vector<char> buffer;
        buffer.resize(buffer_size + fmt_len);
        auto size = strfmt(tms, fmt, &buffer[0], buffer.size());
        return std::string(&buffer[0], size);
    }

public:

    class year
        : public mut
    {
    public:
        explicit year(const tm& t) noexcept
            : mut(t.tm_.tm_year + yd)
        {   }

        explicit year(std::intptr_t val) noexcept
            : mut(val)
        {   }
    };

    /* Month (1-12) */
    class mon
        : public mut
    {
    public:
        explicit mon(const tm& t) noexcept
            : mut(t.tm_.tm_mon + md)
        {   }

        explicit mon(std::intptr_t val) noexcept
            : mut(val)
        {   }
    };

    /* Day of the month (1-31) */
    class mday
        : public mut
    {
    public:
        explicit mday(const tm& t) noexcept
            : mut(t.tm_.tm_mday)
        {   }

        explicit mday(std::intptr_t val) noexcept
            : mut(val)
        {   }
    };

    typedef mday day;

    /* Day of the week (0-6, Sunday = 0) */
    class wday
        : public mut
    {
    public:
        explicit wday(const tm& t) noexcept
            : mut(t.tm_.tm_wday)
        {   }

        explicit wday(std::intptr_t val) noexcept
            : mut(val)
        {   }
    };

    /* Day in the year (0-365, 1 Jan = 0) */
    class yday
        : public mut
    {
    public:
        explicit yday(const tm& t) noexcept
            : mut(t.tm_.tm_yday)
        {   }

        explicit yday(std::intptr_t val) noexcept
            : mut(val)
        {   }
    };

    /* Daylight saving time */
    class isdst
        : public mut
    {
    public:
        explicit isdst(const tm& t) noexcept
            : mut(t.tm_.tm_isdst)
        {   }

        explicit isdst(std::intptr_t val) noexcept
            : mut(val)
        {   }
    };

    /* Hours (0-23) */
    class hour
        : public mut
    {
    public:
        explicit hour(const tm& t) noexcept
            : mut(t.tm_.tm_hour)
        {   }

        explicit hour(std::intptr_t val) noexcept
            : mut(val)
        {   }
    };

    class min
        : public mut
    {
    public:
        explicit min(const tm& t) noexcept
            : mut(t.tm_.tm_min)
        {   }

        explicit min(std::intptr_t val) noexcept
            : mut(val)
        {   }
    };

    class sec
        : public mut
    {
    public:
        explicit sec(const tm& t) noexcept
            : mut(t.tm_.tm_sec)
        {   }

        explicit sec(std::intptr_t val) noexcept
            : mut(val)
        {   }
    };

    class msec
        : public mut
    {
    public:
        explicit msec(const tm& t) noexcept
            : mut(t.millisecond_)
        {   }

        explicit msec(std::intptr_t val) noexcept
            : mut(val)
        {   }
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

    operator year() const noexcept
    {
        return year(*this);
    }

    operator mon() const noexcept
    {
        return mon(*this);
    }

    operator mday() const noexcept
    {
        return mday(*this);
    }

    operator wday() const noexcept
    {
        return wday(*this);
    }

    operator yday() const noexcept
    {
        return yday(*this);
    }

    operator hour() const noexcept
    {
        return hour(*this);
    }

    operator min() const noexcept
    {
        return min(*this);
    }

    operator sec() const noexcept
    {
        return sec(*this);
    }

    operator msec() const noexcept
    {
        return msec(*this);
    }

    operator std::tm() const noexcept
    {
        return get();
    }

    std::tm get() const noexcept
    {
        return tm_;
    }

    template<std::size_t N>
    void put_json(char (&fmt)[N]) const noexcept
    {
        typedef std::uint32_t type;
        num::detail::itoa4zf(static_cast<type>(tm_.tm_year + yd), fmt);
        num::detail::itoa2zf(static_cast<type>(tm_.tm_mon + md), fmt + 5);
        num::detail::itoa2zf(static_cast<type>(tm_.tm_mday), fmt + 8);
        num::detail::itoa2zf(static_cast<type>(tm_.tm_hour), fmt + 11);
        num::detail::itoa2zf(static_cast<type>(tm_.tm_min), fmt + 14);
        num::detail::itoa2zf(static_cast<type>(tm_.tm_sec), fmt + 17);
        num::detail::itoa3zf(millisecond_, fmt + 20);
    }

    std::string to_json() const
    {
        char fmt[] = "0000-00-00T00:00:00.000Z";
        put_json(fmt);
        return std::string(fmt, sizeof(fmt) - 1);
    }

    template<typename T, typename P>
    std::basic_ostream<T, P>& dump_json(
        std::basic_ostream<T, P>& os) const noexcept
    {
        char fmt[] = "0000-00-00T00:00:00.000Z";
        put_json(fmt);
        return os.write(fmt, sizeof(fmt) - 1);
    }

    template<std::size_t N>
    void put_date_json(char (&fmt)[N]) const noexcept
    {
        typedef std::uint32_t type;
        num::detail::itoa4zf(static_cast<type>(tm_.tm_year + yd), fmt);
        num::detail::itoa2zf(static_cast<type>(tm_.tm_mon + md), fmt + 5);
        num::detail::itoa2zf(static_cast<type>(tm_.tm_mday), fmt + 8);
    }

    std::string to_date_json() const
    {
        char fmt[] = "0000-00-00";
        put_date_json(fmt);
        return std::string(fmt, sizeof(fmt) - 1);
    }

    template<typename T, typename P>
    std::basic_ostream<T, P>& dump_date_json(
        std::basic_ostream<T, P>& os) const noexcept
    {
        char fmt[] = "0000-00-00";
        put_date_json(fmt);
        return os.write(fmt, sizeof(fmt) - 1);
    }

    template<std::size_t N>
    void put_time_json(char (&fmt)[N]) const noexcept
    {
        typedef std::uint32_t type;
        num::detail::itoa2zf(static_cast<type>(tm_.tm_hour), fmt);
        num::detail::itoa2zf(static_cast<type>(tm_.tm_min), fmt + 3);
        num::detail::itoa2zf(static_cast<type>(tm_.tm_sec), fmt + 6);
        num::detail::itoa3zf(millisecond_, fmt + 9);
    }

    std::string to_time_json() const
    {
        char fmt[] = "00:00:00.000";
        put_time_json(fmt);
        return std::string(fmt, sizeof(fmt) - 1);
    }

    template<typename T, typename P>
    std::basic_ostream<T, P>& dump_time_json(
        std::basic_ostream<T, P>& os) const noexcept
    {
        char fmt[] = "00:00:00.000";
        put_time_json(fmt);
        return os.write(fmt, sizeof(fmt) - 1);
    }

    template<std::size_t fmt_len>
    std::string str(const char(&fmt)[fmt_len]) const
    {
        return put_time(tm_, fmt);
    }

    std::string str(const std::string& fmt) const
    {
        return put_time(tm_, fmt.c_str(), fmt.size());
    }

    std::string to_millisecond_string() const
    {
        char buffer[] = "000";
        num::detail::itoa3zf(millisecond_, buffer);
        return std::string(buffer, sizeof(buffer) - 1);
    }

    std::size_t write_time(const char *fmt,
        char *buffer, std::size_t buffer_len) const noexcept
    {
        return strfmt(tm_, fmt, buffer, buffer_len);
    }

    template<std::size_t buffer_len>
    std::size_t write_time(const char *fmt,
        char(&buffer)[buffer_len]) const noexcept
    {
        return write_time(fmt, buffer, buffer_len);
    }

    class locale
    {
        std::string prev_;

        static inline std::string create()
        {
            auto time_locale = std::setlocale(LC_TIME, nullptr);
            if (time_locale)
                return std::string(time_locale);

            return std::string("C");
        }

    public:

        locale(const std::string& loc)
            : prev_(create())
        {
            std::setlocale(LC_TIME, loc.c_str());
        }

        ~locale()
        {
            std::setlocale(LC_TIME, prev_.c_str());
        }
    };

    // Converts a date to a string by using the current or specified locale.
    std::string to_locale_string() const
    {
        return put_time(tm_, "%x, %X");
    }

    // Converts a date to a string by using the current or specified locale.
    std::string to_locale_date_string() const
    {
        return put_time(tm_, "%x");
    }

    // Converts a date to a string by using the current or specified locale.
    std::string to_locale_time_string() const
    {
        return put_time(tm_, "%X");
    }

    // Converts a date to a string by using the current or specified locale.
    std::string to_locale_string(const std::string& loc) const
    {
        locale l(loc);
        return put_time(tm_, "%x, %X");
    }

    // Converts a date to a string by using the current or specified locale.
    std::string to_locale_date_string(const std::string& loc) const
    {
        locale l(loc);
        return put_time(tm_, "%x");
    }

    // Converts a date to a string by using the current or specified locale.
    std::string to_locale_time_string(const std::string& loc) const
    {
        locale l(loc);
        return put_time(tm_, "%X");
    }

    template<typename T, typename P>
    std::basic_ostream<T, P>& dump(std::basic_ostream<T, P>& os) const
        noexcept
    {
        static const char fmt[] = "%a, %d %b %Y %H:%M:%S GMT";
        char buffer[buffer_size + sizeof(fmt)];
        auto size = write_time(fmt, buffer);
        return os.write(buffer, size);
    }

    /*
    Returns a date converted to a string using Greenwich Mean Time(GMT).
    example : "Fri, 27 Nov 2015 19:16:51 GMT"
    */
    std::string to_string() const
    {
        return put_time(tm_, "%a, %d %b %Y %H:%M:%S GMT");
    }

    std::string to_zone_string() const
    {
        return std::string("+0000");
    }

    std::string to_zonename_string() const
    {
        return std::string("GMT");
    }
};

} // namespace util
} // namespace btdef
