#pragma once

#include "btdef/allocator/module.hpp"

namespace btdef {
namespace allocator {

template<class T>
struct basic
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef std::size_t size_type;

    pointer allocate(size_type size, pointer hint = nullptr) noexcept
    {
        size *= sizeof(value_type);
        return static_cast<pointer>(std::realloc(hint, size));
    }

    void deallocate(pointer p, size_type) noexcept
    {
        std::free(p);
    }
};

} // namespace allocator
} // namespace btdef
