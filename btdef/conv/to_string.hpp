// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/num/itoa.hpp"
#include "btdef/num/dtoap.hpp"
#include "btdef/num/fpconv.hpp"

namespace btdef {
namespace conv {

template <typename T>
inline std::string to_string(T val)
{
    char buf[24];
    using num::itoa;
    return { std::begin(buf), itoa(val, buf) };
}

inline std::string to_string(float value)
{
    using num::fpconv::dtofp;
    return dtofp(static_cast<double>(value)).str();
}

inline std::string to_string(double value)
{
    using num::fpconv::dtofp;
    return dtofp(value).str();
}

inline std::string to_string(float val, std::size_t exp)
{
    using num::dtoap::make_real;
    return make_real(static_cast<double>(val), exp).str();
}

inline std::string to_string(double val, std::size_t exp)
{
    using num::dtoap::make_real;
    return make_real(val, exp).str();
}

template <typename CharT>
inline std::basic_string<CharT> to_string(const CharT *ptr)
{
    std::basic_string<CharT> str;
    if (ptr != nullptr) str.assign(ptr);
    return str;
}

template <typename CharT>
inline std::basic_string<CharT> to_string(const CharT *ptr, std::size_t len)
{
    std::basic_string<CharT> str;
    if (ptr != nullptr && len != 0)
        str.assign(ptr, len);
    return str;
}

template <typename CharT, std::size_t N>
inline std::basic_string<CharT> to_string(const CharT (&ptr)[N])
{
    std::basic_string<CharT> str(ptr, N);
    return str;
}

} // namespace conv
} // namespace btdef
