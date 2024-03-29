// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/allocator/basic_pool.hpp"

#include <memory>

namespace btdef {
namespace allocator {

template <class T>
class wrapper
{
public:
    using value_type = T;
    using allocator_type = pool;
    using Traits = std::allocator_traits<wrapper<T>>;
    using pointer = typename std::allocator<T>::pointer;
    using const_pointer = typename std::allocator<T>::const_pointer;
    using reference = typename std::allocator<T>::reference;
    using const_reference = typename std::allocator<T>::const_reference;
    using size_type = typename std::allocator<T>::size_type;
    using difference_type = typename std::allocator<T>::difference_type;

    allocator_type* base_{nullptr};

    wrapper() = default;

    explicit wrapper(allocator_type& base)
        : base_(&base)
    {   }

    template<class U>
    wrapper(const wrapper<U>& other)
        : base_(other.base_)
    {   }

    pointer allocate(std::size_t n)
    {
        assert(base_);
        return reinterpret_cast<pointer>(base_->malloc(n * sizeof(T)));
    }
    void deallocate(pointer ptr, std::size_t)
    {
        assert(base_);
        base_->free(ptr);
    }

    template<class U, class... Args>
    void construct(U* p, Args&&... args)
    {
        std::allocator<U>().construct(p, std::forward<Args>(args)...);
    }

    template<class U>
    void destroy(U* p)
    {
        std::allocator<U>().destroy(p);
    }

    template<class U>
    struct rebind
    {
        using other = wrapper<U>;
    };

    template<class U>
    bool operator==(const wrapper<U>& other) const noexcept
    {
        return base_ == other.base_;
    }

    template<class U>
    bool operator!=(const wrapper<U>& other) const noexcept
    {
        return !(*this == other);
    }
};

} // namespace allocator
} // namespace btdef
