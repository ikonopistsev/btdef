// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include <cstdint>

#if !defined(BTDEF_HAS_NOEXCEPT)
#if defined(__clang__)
#if __has_feature(cxx_noexcept)
#define BTDEF_HAS_NOEXCEPT
#endif
#else
#if defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || \
    defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026
#define BTDEF_HAS_NOEXCEPT
#endif
#endif
#endif

#ifdef BTDEF_HAS_NOEXCEPT
#define BTDEF_NOEXCEPT noexcept
#else
#define BTDEF_NOEXCEPT
#endif

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

