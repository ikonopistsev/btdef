// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/config.hpp"

#include <cstdlib>

namespace btdef {
namespace allocator {

template<class T>
struct basic
{
    typedef T* pointer;
    typedef T value_type;
    typedef std::size_t size_type;

    pointer allocate(size_type size, pointer h = nullptr) const BTDEF_NOEXCEPT
    {
        return (size) ?
            static_cast<pointer>(std::realloc(h, size * sizeof(T))) : nullptr;
    }

    void deallocate(pointer p, size_type) const BTDEF_NOEXCEPT
    {
        std::free(p);
    }
};

} // namespace allocator
} // namespace btdef
