// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/config.hpp"

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
    constexpr static auto delta = UTIL_U64(116444736000000000);
    auto time = to_quad_100ns(ft);
    time -= delta;
    return to_time_point_from_100ns(time);
}
#endif

static inline time_point_t from_time_t(std::time_t t) noexcept
{
    return std::chrono::system_clock::from_time_t(t);
}

static inline time_point_t now() noexcept
{
    return std::chrono::system_clock::now();
}

static inline steady_point_t steady() noexcept
{
    return std::chrono::steady_clock::now();
}

static inline std::tm empty_tm_dst() noexcept
{
    std::tm tmdst{};
    // вообще выглядит не очень
    // но реальность она такая
    // для mktime, на момент генерации таймстампа,
    // нужен флаг учета летнего времени
    // его можно узнать только запросив этот фалг
    // перед генерацией таймстампа
    std::tm tmst;
    time_t t = std::time(nullptr);
#if defined(WIN32) || defined(_WIN32)
    localtime_s(&tmst, &t);
#else
    localtime_r(&t, &tmst);
#endif
    // такая вот грабелька
    tmdst.tm_isdst = tmst.tm_isdst;
    return tmdst;
}


} // namespace time
} // namespace util
} // namespace btdef
