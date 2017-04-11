// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/util/util.hpp"
#include <cstdlib>

namespace btdef {
namespace util {

class time_zone
{
    std::string prev_;

public:
    /*
        Windows TZ data
        http://science.ksc.nasa.gov/software/winvn/userguide/3_1_4.htm
        example EET : "EET-2"
    */

    static inline std::string create()
    {
#if defined(WIN32) || defined(_WIN32)
        _tzset();
        size_t size = 0;
        char buffer[16];
        if (getenv_s(&size, buffer, sizeof(buffer), "TZ") == 0)
            return std::string(buffer, size);
#else
        auto env = ::getenv("TZ");
        if (env)
            return std::string(env);
#endif
        return std::string();
    }

    time_zone(const std::string& tz)
        : prev_(create())
    {
#if defined(WIN32) || defined(_WIN32)
        _putenv_s("TZ", tz.c_str());
        _tzset();
#else
        ::setenv("TZ", tz.c_str(), 1);
        ::tzset();
#endif //
    }

    time_zone(const char* tz)
        : prev_(create())
    {
#if defined(WIN32) || defined(_WIN32)
        _putenv_s("TZ", tz);
        _tzset();
#else
        ::setenv("TZ", tz, 1);
        ::tzset();
#endif //
    }

    ~time_zone()
    {
	    if (prev_.empty()) return;

#if defined(WIN32) || defined(_WIN32)
        _putenv_s("TZ", prev_.c_str());
        _tzset();
#else
        if (prev_.empty())
            ::unsetenv("TZ");
        else
            ::setenv("TZ", prev_.c_str(), 1);

        ::tzset();
#endif //
    }
};

} // namespace util
} // namespace btdef
