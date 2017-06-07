// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/num/itoa.hpp"
#include "btdef/num/dtoap.hpp"
#include "btdef/num/fpconv.hpp"
#include "btdef/util/string.hpp"

#include <type_traits>

namespace btdef {
namespace conv {

template<typename T>
util::string to_string(T val)
{
    char buf[24];
    using num::itoa;
    return util::string(std::begin(buf), std::distance(buf, itoa(val, buf)));
}

static inline util::string to_string(float value)
{
    util::string result;
    result.reserve(24);
    using num::fpconv::dtoa;
    result.increase(dtoa(static_cast<double>(value), result.data()));
    return result;
}

static inline util::string to_string(double value)
{
    util::string result;
    result.reserve(32);
    using num::fpconv::dtoa;
    result.increase(dtoa(value, result.data()));
    return result;
}

static inline util::string to_string(float val, std::size_t exp)
{
    using num::dtoap::make_real;
    return util::string(make_real(static_cast<double>(val), exp));
}

static inline util::string to_string(double val, std::size_t exp)
{
    using num::dtoap::make_real;
    return util::string(make_real(val, exp));
}
/*
template<class A>
std::string to_string(const util::basic_string<char, A>& str)
{
    return std::string(str.data(), str.size());
}

template<std::size_t N>
util::text to_text(std::reference_wrapper<const char[N]> ref)
{
    static_assert(util::text::cache_size > N, "string size");
    return util::text(ref);
}

namespace stdex {

template<std::size_t N>
std::string to_string(std::reference_wrapper<const char[N]> ref)
{
    return std::string(ref.get(), N - 1);
}

} // namespace stdex
*/
} // namespace conv
} // namespace btdef

