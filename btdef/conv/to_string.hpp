// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/num/itoa.hpp"
#include "btdef/num/dtoap.hpp"
#include "btdef/num/fpconv.hpp"
#include "btdef/util/string.hpp"

namespace btdef {
namespace conv {

template <typename T>
util::string to_string(T val)
{
    char buf[24];
    using num::itoa;
    return { std::begin(buf), itoa(val, buf) };
}

static inline util::string to_string(float value)
{
    using num::fpconv::dtofp;
    auto res = dtofp(static_cast<double>(value));
    return { res.cbegin(), res.cend() };
}

static inline util::string to_string(double value)
{
    using num::fpconv::dtofp;
    auto res = dtofp(value);
    return { res.cbegin(), res.cend() };
}

static inline util::string to_string(float val, std::size_t exp)
{
    using num::dtoap::make_real;
    auto res = make_real(static_cast<double>(val), exp);
    return { res.cbegin(), res.cend() };
}

static inline util::string to_string(double val, std::size_t exp)
{
    using num::dtoap::make_real;
    auto res = make_real(val, exp);
    return { res.cbegin(), res.cend() };
}

template <std::size_t N>
std::string to_string(const util::basic_string<N>& str)
{
    return std::string(str.data(), str.size());
}

} // namespace conv
} // namespace btdef
