// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/num/itoa.hpp"

#include <stdexcept>

#include <cmath>

namespace btdef {
namespace num {
namespace dtoap {
namespace detail {

template<typename T>
class real_impl;

template<>
class real_impl<double>
{
    char str_[24];
    char *end_;

    static std::uint64_t get_exp(std::size_t precision)
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

    static std::pair<std::uint64_t, std::uint64_t>
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

    static char* print(double val, std::size_t exp, char *text)
    {
        const bool negative = std::signbit(val);

        if (negative)
            *text++ = '-';

        const auto llv = split(negative ? -val : val, exp);

        if (llv.first)
            text = num::itoa(llv.first, text);
        else
            ++text;

        *text++ = '.';

        char* res = text + exp;

        if (llv.second)
            num::itoa(llv.second, text);

        return res;
    }

public:
    real_impl(double val, std::size_t exp)
        : str_{"00000000000000000000000"}
        , end_(print(val, exp, str_))
    {   }

    const char* cbegin() const noexcept
    {
        return str_;
    }

    const char* cend() const noexcept
    {
        return end_;
    }
};

} // namespace detail

template<typename T>
class real;

template<typename V>
class real
{
    detail::real_impl<V> real_;

public:
    real(V val, std::size_t exp)
        : real_(val, exp)
    {   }

    std::string str() const
    {
        return { real_.cbegin(), real_.cend() };
    }

    template<typename T, typename P>
    std::basic_ostream<T, P>& dump(std::basic_ostream<T, P>& os) const
    {
        return os.write(real_.cbegin(),
            std::distance(real_.cbegin(), real_.cend()));
    }
};

template<typename T, typename P, typename V>
std::basic_ostream<T, P>& operator<<(std::basic_ostream<T, P>& os,
    const real<V>& r)
{
    return r.dump(os);
}

template<typename T>
real<T> make_real(T num, std::size_t exp)
{
    return real<T>(num, exp);
}

template<typename T>
std::string to_string(T num, std::size_t exp)
{
    return make_real(num, exp).str();
}

} // namespace dtoap
} // namespace num
} // namespace btdef
