// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/num/itoa.hpp"
#include "btdef/num/dtoap.hpp"
#include "btdef/num/fpconv.hpp"
#include "btdef/util/text.hpp"

#include <type_traits>

namespace btdef {
namespace conv {

template<typename T>
util::text to_text(T val) noexcept
{
    util::text result;
    using num::itoa;
    result.increase(std::distance(result.begin(), itoa(val, result.data())));
    return result;
}

static inline util::text to_text(float value) noexcept
{
    using num::fpconv::dtoa;
    util::text result;
    result.increase(dtoa(static_cast<double>(value), result.data()));
    return result;
}

static inline util::text to_text(double value) noexcept
{
    using num::fpconv::dtoa;
    util::text result;
    result.increase(dtoa(static_cast<double>(value), result.data()));
    return result;
}

static inline util::text to_text(float val, std::size_t exp) noexcept
{
    using num::dtoap::make_real;
    return util::text(make_real(static_cast<double>(val), exp));
}

static inline util::text to_text(double val, std::size_t exp) noexcept
{
    using num::dtoap::make_real;
    return util::text(make_real(val, exp));
}

} // namespace conv
} // namespace btdef
