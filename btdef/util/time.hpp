// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include <ctime>
#include <cstring>
#include <chrono>

#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#include <Winsock2.h>
#else
#include <sys/time.h>
#endif

namespace btdef {
namespace util {
namespace time {

typedef std::chrono::system_clock::time_point time_point_t;
typedef std::chrono::steady_clock::time_point steady_point_t;
typedef timeval timeval_t;

#if defined(WIN32) || defined(_WIN32)
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define UTIL_U64(x) x##Ui64
#else
#define UTIL_U64(x) x##ULL
#endif

static inline ULONGLONG to_quad_100ns(FILETIME ft) noexcept
{
    ULARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    return li.QuadPart;
}

static inline time_point_t to_time_point_from_100ns(ULONGLONG ns100) noexcept
{
    using std::chrono::time_point;
    using std::chrono::nanoseconds;
    using std::chrono::system_clock;
    return std::chrono::time_point_cast<system_clock::duration>(
        time_point<system_clock, nanoseconds>(nanoseconds(ns100 * 100)));
}

static inline time_point_t now(FILETIME ft) noexcept
{
    static constexpr auto delta = UTIL_U64(116444736000000000);
    auto time = to_quad_100ns(ft);
    time -= delta;
    return to_time_point_from_100ns(time);
}
#endif

static inline time_point_t now() noexcept
{
    return std::chrono::system_clock::now();
}

static inline steady_point_t steady() noexcept
{
    return std::chrono::steady_clock::now();
}

static inline std::tm create_tm() noexcept
{
    std::tm result;
    std::memset(&result, 0, sizeof(result));
    return result;
}

static inline std::tm empty_tm() noexcept
{
    static std::tm empty = create_tm();
    return empty;
}

static inline std::tm empty_tm_dst() noexcept
{
    std::tm tmdst = create_tm();
    // вообще выглядит не очень
    // но реальность она такая
    // для mktime, на момент генерации таймстампа,
    // нужен флаг учета летнего времени
    // его можно узнать только запросив этот фалг
    // перед генерацией таймстампа
    time_t t = std::time(nullptr);
    // такая вот грабелька
    tmdst.tm_isdst = localtime(&t)->tm_isdst;
    return tmdst;
}


} // namespace time
} // namespace util
} // namespace btdef
