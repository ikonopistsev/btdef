// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include <utility>

namespace btdef {
namespace allocator {

template<class allocator_type, class value_type>
class hider
    : public allocator_type
{
public:
    typedef std::size_t size_type;
    typedef typename allocator_type::pointer allocator_pointer;

private:
    value_type ptr_{nullptr};

    hider(const hider&) = delete;
    hider& operator=(const hider&) = delete;

public:
    explicit hider(const allocator_type& a) noexcept
        : allocator_type(a)
    {   }

    hider(const allocator_type& a, const value_type val) noexcept
        : allocator_type(a)
        , ptr_(val)
    {   }

    hider(hider&& other) noexcept
        : allocator_type(std::move(other))
    {
        std::swap(ptr_, other.ptr_);
    }

    value_type create(size_type size) noexcept
    {
        ptr_ = reinterpret_cast<value_type>(allocator_type::allocate(size,
            reinterpret_cast<allocator_pointer>(ptr_)));
        return ptr_;
    }

    value_type get() const noexcept
    {
        return ptr_;
    }
};

} // namespace allocator
} // namespace btdef
