// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

/*
    Date object javascript analog;
    https://msdn.microsoft.com/en-us/library/jj155284(v=vs.94).aspx
*/

#include "btdef/util/tm.hpp"

#include <algorithm>
#include <stdexcept>

namespace btdef {
namespace util {

class date
{
public:
    typedef std::int64_t value_t;
    typedef time::time_point_t time_point_t;
    typedef time::timeval_t timeval_t;
    typedef tm::millisecond_t millisecond_t;
    typedef std::intptr_t minuteswest_t;
    typedef std::intptr_t dstflag_t;

    enum {
        k_msec = 1000u,
        k_usec = 1000000u
    };

private:

    time_point_t time_point_{};

    template<typename S, typename M>
    static time_point_t create(S sec, M usec) noexcept
    {
        using std::chrono::time_point;
        using std::chrono::microseconds;
        using std::chrono::system_clock;

        const value_t time = static_cast<value_t>(sec) * k_usec + usec;
        return std::chrono::time_point_cast<system_clock::duration>(
                time_point<system_clock, microseconds>(microseconds(time)));
    }

    static inline std::time_t to_time(const std::tm& tm,
        minuteswest_t minuteswest) noexcept
    {
        static const std::intptr_t mdays[] = {
            0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334
        };

        std::intptr_t year = tm.tm_year - 70;
        std::intptr_t month = tm.tm_mon;
        std::intptr_t day = tm.tm_mday;

        if ((year < 0) || (year > 129)) /* algo only works for 1970-2099 */
            return -1;

        if ((month < 0) || (month > 11)) /* array bounds */
            return -1;

        if (day < 0)
            return -1;

        if ((month < 2) || ((year + 2) % 4))
            --day;

        if ((tm.tm_hour < 0) || (tm.tm_min < 0) || (tm.tm_sec < 0))
            return -1;

        std::time_t rc = tm.tm_hour * 3600 + tm.tm_min * 60 +
                tm.tm_sec + minuteswest * 60;

        rc += (year * 365 + (year + 1) / 4 + mdays[month] + day) * 86400u;

        return rc;
    }

    static inline date make(const std::tm& tms,
        millisecond_t millisecond, minuteswest_t minuteswest)
    {
        std::time_t second = to_time(tms, minuteswest);
        if (second < 0)
            throw std::runtime_error("parse date");

        return date(second, millisecond);
    }

    template<class T>
    static bool out_of_range(T from, T value, T to) noexcept
    {
        return (value < from) || (value > to);
    }

    template<class T>
    static bool eq(T value, T n1) noexcept
    {
        return value == n1;
    }

    template<class T, class... A>
    static bool eq(T value, T n1, A... nn) noexcept
    {
        return (value == n1) || eq(value, nn...);
    }

public:

    date() = default;

    explicit date(time_point_t tp) noexcept
        : time_point_(tp)
    {   }

    explicit date(value_t val) noexcept
        : time_point_(create(val / k_msec, (val % k_msec) * k_msec))
    {   }

    /*
    для сторонней инициализации
    например для gettimeofday() или evnet::queue::gettimeofday_cached() const
    */
    explicit date(timeval_t tv) noexcept
        : time_point_(create(tv.tv_sec, tv.tv_usec))
    {   }

#if defined(WIN32) || defined(_WIN32)
    explicit date(FILETIME ft) noexcept
        : time_point_(time::now(ft))
    {   }
#endif

    explicit date(const std::tm& tm, millisecond_t ms = 0)
    {
        std::tm t = tm;
        time_t sec = mktime(&t);
        if (sec == static_cast<time_t>(-1))
            throw std::runtime_error("invalid date");

        time_point_ = create(sec, ms * k_msec);
    }

    explicit date(const std::basic_string_view<char>& text)
    {
        if (!text.empty())
            *this = parse(text.data());
    }

    // ultrafast parser :)
    explicit date(const char *ptr)
    {
        *this = parse(ptr);
    }

    date(std::time_t sec, millisecond_t msec) noexcept
        : time_point_(create(sec, k_msec * msec))
    {   }

    static inline date parse(const char *ptr)
    {
        assert(ptr);

        std::tm tms{};
        millisecond_t msec = 0;
        minuteswest_t minwest = 0;

        using conv::antoutp;
        ptr = antoutp<4>(ptr, -1900, tms.tm_year);
        if (!eq(*ptr++, '-'))
            return make(tms, msec, minwest);

        ptr = antoutp<2>(ptr, -1, tms.tm_mon);
        if (!eq(*ptr++, '-'))
            return make(tms, msec, minwest);

        ptr = antoutp<2>(ptr, tms.tm_mday);
        if (!eq(*ptr++, 'T', 't'))
            return make(tms, msec, minwest);

        ptr = antoutp<2>(ptr, tms.tm_hour);
        if (!eq(*ptr++, ':'))
            return make(tms, msec, minwest);

        ptr = antoutp<2>(ptr, tms.tm_min);
        if (!eq(*ptr++, ':'))
            return make(tms, msec, minwest);

        ptr = antoutp<2>(ptr, tms.tm_sec);
        if (!eq(*ptr++, '.'))
            return make(tms, msec, minwest);

        ptr = antoutp<3>(ptr, msec);

        if (!eq(*ptr, 'Z', 'z'))
        {
            if (eq(*ptr++, '-'))
            {
                ptr = antoutp<2>(ptr, minwest);
                minwest *= 60;

                if (eq(*ptr, ':'))
                    ++ptr;

                if (!out_of_range('0', *ptr, '9'))
                    antoutp<2>(ptr, minwest, minwest);
            }
            else
            {
                ptr = antoutp<2>(ptr, minwest);
                minwest *= 60;

                if (eq(*ptr, ':'))
                    ++ptr;

                if (!out_of_range('0', *ptr, '9'))
                    antoutp<2>(ptr, minwest, minwest);

                minwest = -minwest;
            }
        }

        return make(tms, msec, minwest);
    }

    static inline date now() noexcept
    {
        return date(time::now());
    }

    static inline date from_time_t(std::time_t t) noexcept
    {
        return date(time::from_time_t(t));
    }

    // Gets the time value in milliseconds.
    value_t time() const noexcept
    {
        using std::chrono::milliseconds;
        auto time = time_point_.time_since_epoch();
        return std::chrono::duration_cast<milliseconds>(time).count();
    }

    std::time_t unix_time() const noexcept
    {
        using std::chrono::system_clock;
        return static_cast<std::time_t>(system_clock::to_time_t(time_point_));
    }

    time_point_t time_point() const noexcept
    {
        return time_point_;
    }

    timeval_t sys_time() const noexcept
    {
        auto t = time();
        return {
            static_cast<decltype(timeval_t::tv_sec)>(t / k_msec),
            static_cast<decltype(timeval_t::tv_usec)>((t % k_msec) * k_msec)
        };
    }

    millisecond_t millisecond() const noexcept
    {
        return static_cast<millisecond_t>(time() % k_msec);
    }

    bool operator==(date other) const noexcept
    {
        return time_point_ == other.time_point_;
    }

    bool operator<(date other) const noexcept
    {
        return time_point_ < other.time_point_;
    }

    date operator+(value_t ms) const noexcept
    {
        return date(time_point_ + std::chrono::milliseconds(ms));
    }

    template<class Rep, class Period>
    date operator+(std::chrono::duration<Rep, Period> d) const noexcept
    {
        return date(time_point_ + d);
    }

    date operator-(value_t ms) const noexcept
    {
        return date(time_point_ - std::chrono::milliseconds(ms));
    }

    template<class Rep, class Period>
    date operator-(std::chrono::duration<Rep, Period> d) const noexcept
    {
        return date(time_point_ - d);
    }

    value_t operator-(date other) const noexcept
    {
        return diff(*this, other);
    }

    static inline value_t diff(date a, date b) noexcept
    {
        return a.time() - b.time();
    }

    static inline double ddiff(date a, date b, double d = 1000.0) noexcept
    {
        return diff(a, b) / d;
    }

    static inline value_t diff_abs(date a, date b) noexcept
    {
        value_t abs = diff(a, b);
        return (abs < 0) ? -abs : abs;
    }

    static inline double ddiff_abs(date a, date b, double d = 1000.0) noexcept
    {
        return diff_abs(a, b) / d;
    }

    date& operator+=(value_t ms) noexcept
    {
        return *this += std::chrono::milliseconds(ms);
    }

    template<class Rep, class Period>
    date& operator+=(std::chrono::duration<Rep, Period> d) noexcept
    {
        time_point_ += d;
        return *this;
    }

    date& operator-=(value_t ms) noexcept
    {
        return *this -= std::chrono::milliseconds(ms);
    }

    template<class Rep, class Period>
    date& operator-=(std::chrono::duration<Rep, Period> d) noexcept
    {
        time_point_ -= d;
        return *this;
    }

    auto local_time() const noexcept
    {
        std::tm tms;
        value_t val = time();
        std::time_t sec = static_cast<std::time_t>(val / k_msec);
#if defined(WIN32) || defined(_WIN32)
        ::localtime_s(&tms, &sec);
#else
        ::localtime_r(&sec, &tms);
#endif //
        return std::make_pair(std::move(tms),
            static_cast<millisecond_t>(val % k_msec));
    }

    auto utc_time() const noexcept
    {
        std::tm tms;
        value_t val = time();
        std::time_t sec = static_cast<std::time_t>(val / k_msec);
#if defined(WIN32) || defined(_WIN32)
        ::gmtime_s(&tms, &sec);
#else
        ::gmtime_r(&sec, &tms);
#endif //
        return std::make_pair(std::move(tms),
            static_cast<millisecond_t>(val % k_msec));
    }

    class local final
        : tm
    {
#if defined(WIN32) || defined(_WIN32)
        minuteswest_t minuteswest_ = 0;

        static inline minuteswest_t timezone() noexcept
        {
            long second;
            _get_timezone(&second);
            return second / 60;
        }
#endif
    public:
        local(date d) noexcept
            : tm(d.local_time())
#if defined(WIN32) || defined(_WIN32)
            , minuteswest_(timezone())
#endif
        {   }

        local& operator=(date d)
        {
            local l(d);
            *this = l;
            return *this;
        }

        minuteswest_t minuteswest() const noexcept
        {
#if defined(WIN32) || defined(_WIN32)
            return minuteswest_;
#else
            return -tm_.tm_gmtoff / 60;
#endif
        }

        dstflag_t dstflag() const noexcept
        {
            return tm_.tm_isdst;
        }

        minuteswest_t timezone_offset() const noexcept
        {
            return (tm_.tm_isdst != 0) ? minuteswest() - 60 : minuteswest();
        }

        char* put_json(char *p, minuteswest_t tz) const noexcept
        {
            p = put_date_json(p);
            *p++ = 'T';
            p = put_time_json(p);

            if (tz < 0)
            {
                tz = -tz;
                *p++ = '+';
            }
            else
                *p++ = '-';

            using num::detail::itoa2zf;
            p = itoa2zf(static_cast<std::uint32_t>(tz / 60), p);
            p = itoa2zf(static_cast<std::uint32_t>(tz % 60), p);

            return p;
        }

        util::text to_json() const noexcept
        {
            minuteswest_t tz = timezone_offset();
            if (tz)
            {
                util::text result;
                char* p = result.data();
                result.resize(static_cast<std::size_t>(
                    std::distance(p, put_json(p, tz))));
                return result;
            }
            return tm::to_json();
        }

        util::text zone() const noexcept
        {
            return tm::text("%z");
        }

        util::text zonename() const noexcept
        {
            return tm::text("%Z");
        }

        util::text text() const noexcept
        {
            return text("%a %b %d %Y %H:%M:%S GMT%z (%Z)");
        }

        util::text text(const char *fmt) const noexcept
        {
            return tm::text(fmt);
        }

        util::text date_text() const noexcept
        {
            return tm::text("%a %b %d %Y");
        }

        util::text time_text() const noexcept
        {
            return tm::text("%H:%M:%S GMT%z (%Z)");
        }

        using tm::data;
        using tm::msec;
        using tm::wday;
    };


    explicit date(const local& l)
    {
        std::tm t{l.data()};
        time_t sec = std::mktime(&t);
        if (sec == static_cast<time_t>(-1))
            throw std::runtime_error("invalid date");

        time_point_ = create(sec, l.msec() * k_msec);
    }

    class utc final
        : public tm
    {
    public:
        explicit utc(date d) noexcept
            : tm(d.utc_time())
        {   }

        util::text date_json() const noexcept
        {
            util::text result;
            char* p = result.data();
            result.resize(static_cast<std::size_t>(
                std::distance(p, put_date_json(p))));
            return result;
        }

        util::text time_json() const noexcept
        {
            util::text result;
            char* p = result.data();
            result.resize(static_cast<std::size_t>(
                std::distance(p, put_time_json(p))));
            return result;
        }
    };

    typedef utc gmt;

    // Returns a date converted to a string using Greenwich Mean Time(GMT).
    // example : "Fri, 27 Nov 2015 19:16:51 GMT"
    util::text utc_text() const
    {
        return utc(*this).text();
    }

    // Returns a time as a string value.
    // ex: "Sat Nov 28 2015 00:05:36 GMT+0300 (RTZ 2 (зима))" using local time
    util::text text() const
    {
        return local(*this).text();
    }

    util::text date_text() const
    {
        return local(*this).date_text();
    }

    util::text time_text() const
    {
        return local(*this).time_text();
    }

    util::text to_json() const noexcept
    {
        return utc(*this).to_json();
    }

    util::text date_json() const
    {
        return utc(*this).date_json();
    }

    util::text time_json() const
    {
        return utc(*this).time_json();
    }

    util::text zone() const noexcept
    {
        return local(*this).zone();
    }

    util::text zonename() const
    {
        return local(*this).zonename();
    }

    static inline util::text log_time()
    {
        return local(now()).to_json();
    }
};

} // namespace util
} // namespace btdef

bool inline operator!=(btdef::util::date a, btdef::util::date b) noexcept
{
    return !(a == b);
}

bool inline operator>(btdef::util::date a, btdef::util::date b) noexcept
{
    return b < a;
}

bool inline operator<=(btdef::util::date a, btdef::util::date b) noexcept
{
    return !(b < a);
}

bool inline operator>=(btdef::util::date a, btdef::util::date b) noexcept
{
    return !(a < b);
}

template<typename T, typename P>
inline std::basic_ostream<T, P>& 
    operator<<(std::basic_ostream<T, P>& os, btdef::util::date dt) noexcept
{
    btdef::util::text t = dt.text();
    return os.write(t.data(), t.size());
}

template<typename T, typename P>
inline std::basic_ostream<T, P>& 
    operator<<(std::basic_ostream<T, P>& os,
        const btdef::util::date::local& loc) noexcept
{
    btdef::util::text t = loc.text();
    return os.write(t.data(), t.size());
}

template<typename T, typename P>
inline std::basic_ostream<T, P>& 
    operator<<(std::basic_ostream<T, P>& os,
        const btdef::util::date::utc& utc) noexcept
{
    btdef::util::text t = utc.text();
    return os.write(t.data(), t.size());
}
