// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/allocator/pool_allocator.hpp"

#include <memory>

#ifdef BTDEF_TRACE_WRAPPER
#include <cstdio>
#endif // BTDEF_TRACE_WRAPPER

namespace btdef {
namespace allocator {

template <class T>
class wrapper
{
public:
    using value_type = T;
    using allocator_type = pool_allocator;
    using Traits = std::allocator_traits<wrapper<T>>;
    using pointer = typename std::allocator<T>::pointer;
    using const_pointer = typename std::allocator<T>::const_pointer;
    using reference = typename std::allocator<T>::reference;
    using const_reference = typename std::allocator<T>::const_reference;
    using size_type = typename std::allocator<T>::size_type;
    using difference_type = typename std::allocator<T>::difference_type;

    allocator_type* base_{nullptr};

public:
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
#ifdef BTDEF_TRACE_WRAPPER
        fprintf(stdout, "alloc %d\n", static_cast<int>(n * sizeof(T)));
#endif
        return reinterpret_cast<pointer>(base_->malloc(n * sizeof(T)));
    }

#ifdef BTDEF_TRACE_WRAPPER
    void deallocate(pointer ptr, std::size_t n)
    {
        assert(base_);
        fprintf(stdout, "free %d\n", static_cast<int>(n * sizeof(T)));
        base_->free(ptr);
    }
#else
    void deallocate(pointer ptr,
        std::size_t)
    {
        assert(base_);
        base_->free(ptr);
    }
#endif
    // требуется в VC++ и libstdc++
    template<class U, class... Args>
    void construct(U* p, Args&&... args)
    {
        std::allocator<T>().construct(p, std::forward<Args>(args)...);
    }

    template<class U>
    void destroy(U* p)
    {
        std::allocator<T>().destroy(p);
    }

    template<class U>
    struct rebind
    {
        using other = wrapper<U>;
    };

    template<class U>
    bool operator==(const wrapper<U>& other) const
    {
        return base_ == other.base_;
    }
};

} // namespace allocator
} // namespace btdef
