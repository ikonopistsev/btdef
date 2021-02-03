// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include <utility>

namespace btdef {
namespace conv {

template<class P, int>
class param
{
    P self_{};

public:
    using value_type = P;

    param() = default;
    param(const param&) = default;
    param(param&&) = default;
    param& operator=(const param&) = default;
    param& operator=(param&&) = default;

    explicit param(P&& value) noexcept
        : self_(std::move(value))
    {   }

    explicit param(const P& value)
        : self_(value)
    {   }

    param& operator=(P&& value) noexcept
    {
        self_ = std::move(value);
        return *this;
    }

    param& operator=(const P& value)
    {
        self_ = value;
        return *this;
    }

    operator const P& () const noexcept
    {
        return self_;
    }

    operator P& () noexcept
    {
        return self_;
    }
};

} // namespace conv
} // namespace btdef
