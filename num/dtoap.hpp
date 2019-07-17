// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/num/itoa.hpp"

#include <stdexcept>
#include <limits>
#include <cstring>
#include <cmath>

namespace btdef {
namespace num {
namespace detail {

static inline std::uint64_t get_exp(std::size_t precision)
{
    static const std::size_t max_precision = 16;
    static const std::uint64_t exp[max_precision] = {
        1ull, 
        10ull, 
        100ull, 
        1000ull, 
        10000ull, 
        100000ull, 
        1000000ull,
        10000000ull, 
        100000000ull, 
        1000000000ull, 
        10000000000ull,
        100000000000ull, 
        1000000000000ull, 
        10000000000000ull,
        100000000000000ull, 
        1000000000000000ull
    };

    if (precision >= max_precision)
        throw std::invalid_argument("too large precision");

    return exp[precision];
}

static inline std::pair<std::uint64_t, std::uint64_t>
split(double val, std::size_t precision)
{
    static const double max_val =
        static_cast<double>(std::numeric_limits<long long>::max());
    const std::uint64_t exp = get_exp(precision);

    val = std::fma(val, exp, 0.5555555555555555);
    if (val > max_val)
        throw std::range_error("too large real");

    const std::uint64_t ull = static_cast<std::uint64_t>(val);
    const std::uint64_t rval = ull % exp;
    return std::make_pair((ull - rval) / exp, rval);
}

static inline char* print(double val, std::size_t exp, char *text)
{
    const bool negative = std::signbit(val);
    if (negative)
        *text++ = '-';

    const std::pair<std::uint64_t, std::uint64_t> llv = 
        split(negative ? -val : val, exp);

    text = num::itoa(llv.first, text);

    if (!exp)
        return text;

    *text++ = '.';

    char* res = text + exp;

    if (llv.second)
    {
        char t[24];
        std::size_t l = static_cast<std::size_t>(num::itoa(llv.second, t) - t);
        std::memcpy(res - l, t, l);
    }

    return res;
}

} // namespace detail

static inline std::size_t dtoap(double val, std::size_t exp, char *dest)
{
    return static_cast<std::size_t>(detail::print(val, exp, dest) - dest);
}

} // namespace num
} // namespace btdef
