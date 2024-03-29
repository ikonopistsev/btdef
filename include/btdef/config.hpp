// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include <cstdint>

#ifndef BTDEF_UTIL_TEXT_SIZE
#define BTDEF_UTIL_TEXT_SIZE 320
#endif // BTDEF_UTIL_TEXT_SIZE

/*
 *  from rapidjson (http://rapidjson.org/)
 */

#ifndef BTDEF_ALLOCATOR_64BIT
#if defined(__LP64__) || (defined(__x86_64__) && \
    defined(__ILP32__)) || defined(_WIN64)
#define BTDEF_ALLOCATOR_64BIT 1
#else
#define BTDEF_ALLOCATOR_64BIT 0
#endif
#endif // BTDEF_ALLOCATOR_64BIT

#ifndef BTDEF_ALLOCATOR_ALIGN
#if BTDEF_ALLOCATOR_64BIT == 1
#define BTDEF_ALLOCATOR_ALIGN(x) \
            (((x) + static_cast<uint64_t>(7u)) & ~static_cast<uint64_t>(7u))
#else
#define BTDEF_ALLOCATOR_ALIGN(x) (((x) + 3u) & ~3u)
#endif // BTDEF_ALLOCATOR_64BIT
#endif // BTDEF_ALLOCATOR_ALIGN



