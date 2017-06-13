// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/num/itoa.hpp"
#include "btdef/util/string.hpp"

#include <stdexcept>

#include <cmath>

namespace btdef {
namespace num {
namespace detail {

static inline std::uint64_t get_exp(std::size_t precision)
{
    static constexpr std::size_t max_precision = 16;
    static const std::uint64_t exp[max_precision] = {
        1ull, 10ull, 100ull, 1000ull, 10000ull, 100000ull, 1000000ull,
        10000000ull, 100000000ull, 1000000000ull, 10000000000ull,
        100000000000ull, 1000000000000ull, 10000000000000ull,
        100000000000000ull, 1000000000000000ull
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
    const auto exp = get_exp(precision);

    val = std::fma(val, exp, 0.5);
    if (val > max_val)
        throw std::range_error("too large real");

    const auto ull = static_cast<std::uint64_t>(val);
    const auto rval = ull % exp;
    return std::make_pair((ull - rval) / exp, rval);
}

static inline char* print(double val, std::size_t exp, char *text)
{
    const bool negative = std::signbit(val);

    if (negative)
        *text++ = '-';

    const auto llv = split(negative ? -val : val, exp);
    text = num::itoa(llv.first, text);
    *text++ = '.';

    char* res = text + exp;

    char t[24];
    auto l = num::itoa(llv.second, t) - t;
    std::memcpy(res - l, t, l);

    return res;
}

} // namespace detail

static inline std::size_t dtoap(double d, std::size_t exp, char *dest)
{
    return std::distance(dest,
        detail::print(d, exp, dest));
}

} // namespace num
} // namespace btdef
