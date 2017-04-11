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
    typedef date this_type;

    enum {
        k_msec = 1000u,
        k_usec = 1000000u
    };

private:

    time_point_t time_point_{ };

    template<typename S, typename M>
    static time_point_t create(S sec, M usec) noexcept
    {
        using std::chrono::time_point;
        using std::chrono::microseconds;
        using std::chrono::system_clock;

        const auto time = static_cast<value_t>(sec) * k_usec + usec;
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

        return (year * 365 + (year + 1) / 4 + mdays[month] + day) * 86400UL +
            tm.tm_hour * 3600 + tm.tm_min * 60 + tm.tm_sec + minuteswest * 60;
    }

    static inline date make(const std::tm& tms,
        millisecond_t millisecond, minuteswest_t minuteswest)
    {
        auto second = to_time(tms, minuteswest);
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

    date(const date& other) noexcept
        : time_point_(other.time_point_)
    {   }

    explicit date(const time_point_t& timepoint) noexcept
        : time_point_(timepoint)
    {   }

    explicit date(value_t val) noexcept
        : time_point_(create(val / k_msec, (val % k_msec) * k_msec))
    {   }

    /*
    для сторонней инициализации
    например для gettimeofday() или evnet::queue::gettimeofday_cached() const
    */
    explicit date(const timeval_t& tv) noexcept
        : time_point_(create(tv.tv_sec, tv.tv_usec))
    {   }

#if defined(WIN32) || defined(_WIN32)
    explicit date(FILETIME ft) noexcept
        : time_point_(time::now(ft))
    {   }
#endif

    // ultrafast parser :)
    explicit date(const std::string& str)
    {
        *this = parse(str.c_str());
    }

    explicit date(const char *ptr)
    {
        *this = parse(ptr);
    }

    date(std::time_t sec, millisecond_t msec) noexcept
        : time_point_(create(sec, k_msec * msec))
    {   }

    static inline date parse(const char *ptr)
    {
        auto tms = time::empty_tm();
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

    // Gets the time value in milliseconds.
    value_t time() const noexcept
    {
        using std::chrono::milliseconds;
        auto time = time_point_.time_since_epoch();
        return std::chrono::duration_cast<milliseconds>(time).count();
    }

    std::time_t unix_time() const noexcept
    {
        using std::chrono::seconds;
        auto time = time_point_.time_since_epoch();
        auto result = std::chrono::duration_cast<seconds>(time).count();
        return static_cast<std::time_t>(result);
    }

    const time_point_t& time_point() const noexcept
    {
        return time_point_;
    }

    millisecond_t millisecond() const noexcept
    {
        return static_cast<millisecond_t>(time() % k_msec);
    }

    bool operator<(const date& other) const noexcept
    {
        return time_point_ < other.time_point_;
    }

    bool operator>(const date& other) const noexcept
    {
        return time_point_ > other.time_point_;
    }

    bool operator<=(const date& other) const noexcept
    {
        return time_point_ <= other.time_point_;
    }

    bool operator>=(const date& other) const noexcept
    {
        return time_point_ >= other.time_point_;
    }

    bool operator==(const date& other) const noexcept
    {
        return time_point_ == other.time_point_;
    }

    bool operator!=(const date& other) const noexcept
    {
        return time_point_ != other.time_point_;
    }

    date operator+(value_t ms) const noexcept
    {
        return date(time_point_ + std::chrono::milliseconds(ms));
    }

    template<class Rep, class Period>
    date operator+(const std::chrono::duration<Rep, Period>& d) const noexcept
    {
        return date(time_point_ + d);
    }

    date operator-(value_t ms) const noexcept
    {
        return date(time_point_ - std::chrono::milliseconds(ms));
    }

    template<class Rep, class Period>
    date operator-(const std::chrono::duration<Rep, Period>& d) const noexcept
    {
        return date(time_point_ - d);
    }

    value_t operator-(const date& other) const noexcept
    {
        return diff(*this, other);
    }

    static inline value_t diff(const date& a, const date& b) noexcept
    {
        return a.time() - b.time();
    }

    static inline double ddiff(const date& a, const date& b,
                               double d = 1000.0) noexcept
    {
        return diff(a, b) / d;
    }

    static inline value_t diff_abs(const date& a, const date& b) noexcept
    {
        auto abs = diff(a, b);
        return (abs < 0) ? -abs : abs;
    }

    static inline double ddiff_abs(const date& a, const date& b,
                                   double d = 1000.0) noexcept
    {
        return diff_abs(a, b) / d;
    }

    date& operator+=(value_t ms) noexcept
    {
        return *this += std::chrono::milliseconds(ms);
    }

    template<class Rep, class Period>
    date& operator+=(const std::chrono::duration<Rep, Period>& d) noexcept
    {
        time_point_ += d;
        return *this;
    }

    date& operator-=(value_t ms) noexcept
    {
        return *this -= std::chrono::milliseconds(ms);
    }

    template<class Rep, class Period>
    date& operator-=(const std::chrono::duration<Rep, Period>& d) noexcept
    {
        time_point_ -= d;
        return *this;
    }


    tm::tm_t local_time() const noexcept
    {
        std::tm tms;
        auto val = time();
        auto sec = static_cast<std::time_t>(val / k_msec);
#if defined(WIN32) || defined(_WIN32)
        ::localtime_s(&tms, &sec);
#else
        ::localtime_r(&sec, &tms);
#endif //
        return std::make_pair(std::move(tms),
            static_cast<millisecond_t>(val % k_msec));
    }

    tm::tm_t utc_time() const noexcept
    {
        std::tm tms;
        auto val = time();
        auto sec = static_cast<std::time_t>(val / k_msec);
#if defined(WIN32) || defined(_WIN32)
        ::gmtime_s(&tms, &sec);
#else
        ::gmtime_r(&sec, &tms);
#endif //
        return std::make_pair(std::move(tms),
            static_cast<millisecond_t>(val % k_msec));
    }

    class local
        : public tm
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

        local() = default;

        local(const std::tm& stdtm, millisecond_t millisecond)
            : tm(stdtm, millisecond)
        {   }

        explicit local(const this_type& d) noexcept
            : tm(d.local_time())
#if defined(WIN32) || defined(_WIN32)
            , minuteswest_(timezone())
#endif
        {   }

        minuteswest_t minuteswest() const noexcept
        {
#if defined(WIN32) || defined(_WIN32)
            return minuteswest_;
#else
            return -tm_.tm_gmtoff / 60;
#endif
        }

        local& set(const tm::year& val) noexcept
        {
            using type = decltype(tm_.tm_year);
            tm_.tm_year = static_cast<type>(val.get() - tm::yd);
            return *this;
        }

        local& set(const tm::mon& val) noexcept
        {
            using type = decltype(tm_.tm_mon);
            tm_.tm_mon = static_cast<type>(val.get() - tm::md);
            return *this;
        }

        local& set(const tm::mday& val) noexcept
        {
            using type = decltype(tm_.tm_mday);
            tm_.tm_mday = static_cast<type>(val.get());
            return *this;
        }

        local& set(const tm::hour& val) noexcept
        {
            using type = decltype(tm_.tm_hour);
            tm_.tm_hour = static_cast<type>(val.get());
            return *this;
        }

        local& set(const tm::min& val) noexcept
        {
            using type = decltype(tm_.tm_min);
            tm_.tm_min = static_cast<type>(val.get());
            return *this;
        }

        local& set(const tm::sec& val) noexcept
        {
            using type = decltype(tm_.tm_sec);
            tm_.tm_sec = static_cast<type>(val.get());
            return *this;
        }

        local& set(const std::tm& tm) noexcept
        {
            tm_ = tm;
            return *this;
        }

        dstflag_t dstflag() const noexcept
        {
            return tm_.tm_isdst;
        }

        minuteswest_t timezone_offset() const noexcept
        {
            return (tm_.tm_isdst != 0) ? minuteswest() - 60 : minuteswest();
        }

        template<std::size_t N>
        void put_json(minuteswest_t tz, char (&fmt)[N]) const noexcept
        {
            typedef std::uint32_t type;

            using num::detail::itoa4zf;
            itoa4zf(static_cast<type>(tm_.tm_year + yd), fmt);

            using num::detail::itoa2zf;
            itoa2zf(static_cast<type>(tm_.tm_mon + md), fmt + 5);
            itoa2zf(static_cast<type>(tm_.tm_mday), fmt + 8);
            itoa2zf(static_cast<type>(tm_.tm_hour), fmt + 11);
            itoa2zf(static_cast<type>(tm_.tm_min), fmt + 14);
            itoa2zf(static_cast<type>(tm_.tm_sec), fmt + 17);

            using num::detail::itoa3zf;
            itoa3zf(millisecond_, fmt + 20);

            if (tz < 0)
                tz = -tz;
            else
                fmt[23] = '-';

            using num::detail::itoa2zf;
            itoa2zf(static_cast<type>(tz / 60), fmt + 24);
            itoa2zf(static_cast<type>(tz % 60), fmt + 26);
        }

        std::string to_json() const
        {
            auto tz = timezone_offset();
            if (tz == 0)
                return tm::to_json();

            char fmt[] = "0000-00-00T00:00:00.000+0000";
            put_json(tz, fmt);

            return std::string(fmt, sizeof(fmt) - 1);
        }

        template<typename T, typename P>
        std::basic_ostream<T, P>& dump_json(std::basic_ostream<T, P>& os) const
            noexcept
        {
            auto tz = timezone_offset();
            if (tz == 0)
                return tm::dump_json(os);

            char fmt[] = "0000-00-00T00:00:00.000+0000";
            put_json(tz, fmt);

            return os.write(fmt, sizeof(fmt) - 1);
        }

        class mwest
            : public mut
        {
        public:
            explicit mwest(const local& l) noexcept
                : mut(l.minuteswest())
            {   }

            explicit mwest(int val) noexcept
                : mut(val)
            {   }
        };

        class dst
            : public mut
        {
        public:
            explicit dst(const local& l) noexcept
                : mut(l.dstflag())
            {   }

            explicit dst(std::intptr_t val) noexcept
                : mut(val)
            {   }
        };

        std::string to_zone_string() const
        {
            return put_time(tm_, "%z");
        }

        std::string to_zonename_string() const
        {
            return put_time(tm_, "%Z");
        }

        std::string to_string() const
        {
            return put_time(tm_, "%a %b %d %Y %H:%M:%S GMT%z (%Z)");
        }

        std::string to_date_string() const
        {
            return put_time(tm_, "%a %b %d %Y");
        }

        /*
        Returns a time as a string value.
        example : "00:05:36 GMT+0300 (RTZ 2 (зима))" using local time
        */
        std::string to_time_string() const
        {
            return put_time(tm_, "%H:%M:%S GMT%z (%Z)");
        }

        template<typename T, typename P>
        std::basic_ostream<T, P>& dump(std::basic_ostream<T, P>& os) const
            noexcept
        {
            static const char fmt[] = "%a %b %d %Y %H:%M:%S GMT%z (%Z)";
            char buffer[buffer_size + sizeof(fmt)];
            auto size = write_time(fmt, buffer);
            return os.write(buffer, size);
        }
    };

    explicit date(const local& l)
    {
        auto tm = l.get();
        auto sec = mktime(&tm);
        if (sec == static_cast<time_t>(-1))
            throw std::runtime_error("invalid date");
        time_point_ = create(sec, k_msec * tm::msec(l).get());
    }

    date& operator=(const local& l)
    {
        auto tm = l.get();
        auto sec = mktime(&tm);
        if (sec == static_cast<time_t>(-1))
            throw std::runtime_error("invalid date");

        time_point_ = create(sec, k_msec * tm::msec(l).get());

        return *this;
    }

    operator local() const noexcept
    {
        return local(*this);
    }

    class utc
        : public tm
    {
    public:
        explicit utc(const this_type& d) noexcept
            : tm(d.utc_time())
        {   }
    };

    operator utc() const noexcept
    {
        return utc(*this);
    }

    typedef utc gmt;

    /*
    Returns a date converted to a string using Greenwich Mean Time(GMT).
    example : "Fri, 27 Nov 2015 19:16:51 GMT"
    */

    std::string to_utc_string() const
    {
        return utc(*this).to_string();
    }

    std::string to_gmt_string() const
    {
        return to_utc_string();
    }

    /*
    Returns a time as a string value.
    example: "Sat Nov 28 2015 00:05:36 GMT+0300 (RTZ 2 (зима))" using local time
    */

    std::string to_string() const
    {
        return local(*this).to_string();
    }

    std::string to_date_string() const
    {
        return local(*this).to_date_string();
    }

    std::string to_time_string() const
    {
        return local(*this).to_time_string();
    }

    /*
    The ISO format is a simplification of the ISO 8601 format.
    For more information, see Date and Time Strings (JavaScript).
    The time zone is always UTC, denoted by the suffix Z in the output.
    */

    std::string to_iso_string() const
    {
        return utc(*this).to_json();
    }

    std::string to_json() const
    {
        return to_iso_string();
    }

    template<typename T, typename P>
    std::basic_ostream<T, P>& dump_json(std::basic_ostream<T, P>& os) const
        noexcept
    {
        return utc(*this).dump_json(os);
    }

    std::string to_date_json() const
    {
        return utc(*this).to_date_json();
    }

    std::string to_time_json() const
    {
        return utc(*this).to_time_json();
    }

    std::string to_millisecond_string() const
    {
        char buffer[] = "000";
        using num::detail::itoa3zf;
        itoa3zf(static_cast<std::uint32_t>(millisecond()), buffer);
        return std::string(buffer, sizeof(buffer) - 1);
    }

    std::string to_zone_string() const
    {
        return local(*this).to_zone_string();
    }

    std::string to_zonename_string() const
    {
        return local(*this).to_zonename_string();
    }

    template<typename T, typename P>
    std::basic_ostream<T, P>& dump(std::basic_ostream<T, P>& os) const noexcept
    {
        return local(*this).dump(os);
    }

    static inline std::string to_log_time()
    {
        return local(now()).to_json();
    }

    template<typename T, typename P>
    static std::basic_ostream<T, P>& dump_log_time(std::basic_ostream<T, P>& os)
        noexcept
    {
        return local(now()).dump_json(os);
    }
};

template<typename T, typename P>
std::basic_ostream<T, P>& operator<<(std::basic_ostream<T, P>& os,
const date& dt) noexcept
{
    return dt.dump(os);
}

template<typename T, typename P>
std::basic_ostream<T, P>& operator<<(std::basic_ostream<T, P>& os,
const date::local& loc) noexcept
{
    return loc.dump(os);
}

template<typename T, typename P>
std::basic_ostream<T, P>& operator<<(std::basic_ostream<T, P>& os,
const date::utc& utc) noexcept
{
    return utc.dump(os);
}

} // namespace util
} // namespace btdef
