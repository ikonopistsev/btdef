// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/allocator/module.hpp"

#ifdef BTDEF_TRACE_CRT_MALLOC
#include <cstdio>
#endif // BTDEF_TRACE_CRT_MALLOC

namespace btdef {
namespace allocator {

class crt
{
public:
    void* malloc(std::size_t size) noexcept
    {
#ifdef BTDEF_TRACE_CRT_MALLOC
        fprintf(stdout, "malloc %d\n", static_cast<int>(size));
#endif
        return (size) ? std::malloc(size) : nullptr;
    }

    void* realloc(void* ptr, std::size_t, std::size_t size) noexcept
    {
        if (size == 0)
        {
            std::free(ptr);
            return nullptr;
        }
        return std::realloc(ptr, size);
    }

    static void free(void *ptr) noexcept
    {
        std::free(ptr);
    }
};

} // namespace allocator
} // namespace btdef
