// Copyright © 2017 alexander . krakhmal at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include <algorithm>
#include <cmath>
#include <iterator>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace btdef {
namespace num {
namespace ionum {
namespace detail {

template <typename T>
struct uint_impl
{
    static char* to_string_back(char* pb, T val) noexcept
    {
        static constexpr T base = static_cast<T>(10);
        do *--pb = '0' + val % base;
        while (val /= base);
        return pb;
    }
};

struct unsigned_integer
{
    template <typename T>
    static char* to_string_back(char* pb, T val) noexcept
    {
        using impl_type = uint_impl<typename std::conditional<
            sizeof(T) < sizeof(std::size_t), std::size_t, T>::type>;
        return impl_type::to_string_back(pb, val);
    }
};

struct signed_integer
{
    template <typename T>
    static char* to_string_back(char* pb, T val) noexcept
    {
        using uT = typename std::make_unsigned<T>::type;
        if (val >= 0)
            return unsigned_integer::to_string_back(pb, static_cast<uT>(val));
        pb = unsigned_integer::to_string_back(pb, uT(0) - val);
        *--pb = '-';
        return pb;
    }
};

template <typename T>
struct real_impl;

template <>
struct real_impl<double>
{
    static unsigned long long get_exp(std::size_t precision)
    {
        static constexpr std::size_t max_precision = 16;
        static const unsigned long long exp[max_precision] = {
            1ull, 10ull, 100ull, 1000ull, 10000ull, 100000ull, 1000000ull,
            10000000ull, 100000000ull, 1000000000ull, 10000000000ull,
            100000000000ull, 1000000000000ull, 10000000000000ull,
            100000000000000ull, 1000000000000000ull
        };
        if (precision >= max_precision)
            throw std::invalid_argument("too large precision");
        return exp[precision];
    }

    static std::pair<unsigned long long, unsigned long long>
    split(double val, std::size_t precision)
    {
        static const double max_val = 
            static_cast<double>(std::numeric_limits<long long>::max());
        const auto exp = get_exp(precision);

        val = std::fma(val, exp, 0.5);
        if (val > max_val)
            throw std::range_error("too large real");

        const auto ull = static_cast<unsigned long long>(val);
        const auto rval = ull % exp;
        return std::make_pair((ull - rval) / exp, rval);
    }

    static char* to_string_back(char* pb, double val, std::size_t precision)
    {
        const bool negative = std::signbit(val);
        const auto llv = split(negative ? -val : val, precision);

        unsigned_integer::to_string_back(pb, llv.second);
        pb = pb - (llv.second ? precision : 1);
        *--pb = '.';
        pb = unsigned_integer::to_string_back(pb, llv.first);
        if (negative) *--pb = '-';

        return pb;
    }
};

template <typename T>
struct real_impl : real_impl<double> {};

} //namespace detail

template <typename T, typename = typename
    std::enable_if<std::is_integral<T>::value, T>::type>
class integer
    : private std::conditional<
        std::is_signed<T>::value,
        detail::signed_integer,
        detail::unsigned_integer
        >::type
{
    char text_[24];
    char* str_;

public:

    explicit integer(T val) noexcept
        : str_(this->to_string_back(std::end(text_), val))
    {   }

    const char* begin() const noexcept
    {
        return str_;
    }

    const char* end() const noexcept
    {
        return std::end(text_);
    }
};

template <typename T>
inline integer<T> to_integer(T val) noexcept
{
    integer<T> i(val);
    return i;
}

template <typename T>
inline std::string to_string(T val)
{
    auto i = to_integer(val);
    return {i.begin(), i.end()};
}

template <typename T, typename = typename
    std::enable_if<std::is_floating_point<T>::value, T>::type>
class real
    : private detail::real_impl<T>
{
    char text_[24];
    char* str_;

public:

    real(T val, std::size_t precision)
        : text_{"00000000000000000000000"}
        , str_(this->to_string_back(std::end(text_), val, precision))
    {   }

    const char* begin() const noexcept
    {
        return str_;
    }

    const char* end() const noexcept
    {
        return std::end(text_);
    }
};

template <typename T>
inline real<T> to_real(T val, std::size_t precision)
{
    real<T> r(val, precision);
    return r;
}

template <typename T>
inline std::string to_string(T val, std::size_t precision)
{
    auto r = to_real(val, precision);
    return {r.begin(), r.end()};
}

template <typename CharT, typename Traits, typename T>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& out,
    const integer<T>& i)
{
    out.write(i.begin(), std::distance(i.begin(), i.end()));
    return out;
}

template <typename CharT, typename Traits, typename T>
inline std::basic_ostream<CharT, Traits>& operator<<(
    std::basic_ostream<CharT, Traits>& out,
    const real<T>& r)
{
    out.write(r.begin(), std::distance(r.begin(), r.end()));
    return out;
}

} // namespace ionum
} // namespace num
} // namespace btdef
