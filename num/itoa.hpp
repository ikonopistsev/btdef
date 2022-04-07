#pragma once

#include "btdef/config.hpp"

namespace btdef {
namespace num {
namespace detail {

static inline const char* ch_arr() noexcept
{
    static const char res[200] = {
        '0','0','0','1','0','2','0','3','0','4',
        '0','5','0','6','0','7','0','8','0','9',
        '1','0','1','1','1','2','1','3','1','4',
        '1','5','1','6','1','7','1','8','1','9',
        '2','0','2','1','2','2','2','3','2','4',
        '2','5','2','6','2','7','2','8','2','9',
        '3','0','3','1','3','2','3','3','3','4',
        '3','5','3','6','3','7','3','8','3','9',
        '4','0','4','1','4','2','4','3','4','4',
        '4','5','4','6','4','7','4','8','4','9',
        '5','0','5','1','5','2','5','3','5','4',
        '5','5','5','6','5','7','5','8','5','9',
        '6','0','6','1','6','2','6','3','6','4',
        '6','5','6','6','6','7','6','8','6','9',
        '7','0','7','1','7','2','7','3','7','4',
        '7','5','7','6','7','7','7','8','7','9',
        '8','0','8','1','8','2','8','3','8','4',
        '8','5','8','6','8','7','8','8','8','9',
        '9','0','9','1','9','2','9','3','9','4',
        '9','5','9','6','9','7','9','8','9','9'
    };
    return res;
}

static inline char* itoa4zf(std::uint32_t value, char *ptr) noexcept
{
    if (!value)
    {
        *ptr++ = '0';
        *ptr++ = '0';
        *ptr++ = '0';
        *ptr++ = '0';
        return ptr;
    }

    const char* carr = ch_arr();

    const std::uint32_t d1 = (value / 100) << 1;
    const std::uint32_t d2 = (value % 100) << 1;

    if (value >= 1000)
        *ptr++ = carr[d1];
    else
        *ptr++ = '0';

    if (value >= 100)
        *ptr++ = carr[d1 + 1];
    else
        *ptr++ = '0';

    if (value >= 10)
        *ptr++ = carr[d2];
    else
        *ptr++ = '0';

    *ptr++ = carr[d2 + 1];

    return ptr;
}

static inline char* itoa3zf(std::uint32_t value, char *ptr) noexcept
{
    if (!value)
    {
        *ptr++ = '0';
        *ptr++ = '0';
        *ptr++ = '0';
        return ptr;
    }

    const char* carr = ch_arr();

    const std::uint32_t d1 = (value / 100) << 1;
    const std::uint32_t d2 = (value % 100) << 1;

    if (value >= 100)
        *ptr++ = carr[d1 + 1];
    else
        *ptr++ = '0';

    if (value >= 10)
        *ptr++ = carr[d2];
    else
        *ptr++ = '0';

    *ptr++ = carr[d2 + 1];

    return ptr;
}

static inline char* itoa2zf(std::uint32_t value, char *ptr) noexcept
{
    if (!value)
    {
        *ptr++ = '0';
        *ptr++ = '0';
        return ptr;
    }

    const char* carr = ch_arr();
    const std::uint32_t d2 = (value % 100) << 1;

    if (value >= 10)
        *ptr++ = carr[d2];
    else
        *ptr++ = '0';

    *ptr++ = carr[d2 + 1];

    return ptr;
}

static inline char* itoa(std::uint32_t value, char *ptr) noexcept
{
    if (!value)
    {
        *ptr++ = '0';
        return ptr;
    }

    const char* carr = ch_arr();

    if (value < 10000)
    {
        const std::uint32_t d1 = (value / 100) << 1;
        const std::uint32_t d2 = (value % 100) << 1;

        if (value >= 1000)
            *ptr++ = carr[d1];
        if (value >= 100)
            *ptr++ = carr[d1 + 1];
        if (value >= 10)
            *ptr++ = carr[d2];
        *ptr++ = carr[d2 + 1];
    }
    else if (value < 100000000)
    {
        // value = bbbbcccc
        const std::uint32_t b = value / 10000;
        const std::uint32_t c = value % 10000;

        const std::uint32_t d1 = (b / 100) << 1;
        const std::uint32_t d2 = (b % 100) << 1;

        const std::uint32_t d3 = (c / 100) << 1;
        const std::uint32_t d4 = (c % 100) << 1;

        if (value >= 10000000)
            *ptr++ = carr[d1];
        if (value >= 1000000)
            *ptr++ = carr[d1 + 1];
        if (value >= 100000)
            *ptr++ = carr[d2];
        *ptr++ = carr[d2 + 1];

        *ptr++ = carr[d3];
        *ptr++ = carr[d3 + 1];
        *ptr++ = carr[d4];
        *ptr++ = carr[d4 + 1];
    }
    else
    {
        // value = aabbbbcccc in decimal

        const std::uint32_t a = value / 100000000; // 1 to 42
        value %= 100000000;

        if (a >= 10) {
            const unsigned i = a << 1;
            *ptr++ = carr[i];
            *ptr++ = carr[i + 1];
        }
        else
            *ptr++ = static_cast<char>('0' + static_cast<char>(a));

        const std::uint32_t b = value / 10000; // 0 to 9999
        const std::uint32_t c = value % 10000; // 0 to 9999

        const std::uint32_t d1 = (b / 100) << 1;
        const std::uint32_t d2 = (b % 100) << 1;

        const std::uint32_t d3 = (c / 100) << 1;
        const std::uint32_t d4 = (c % 100) << 1;

        *ptr++ = carr[d1];
        *ptr++ = carr[d1 + 1];
        *ptr++ = carr[d2];
        *ptr++ = carr[d2 + 1];
        *ptr++ = carr[d3];
        *ptr++ = carr[d3 + 1];
        *ptr++ = carr[d4];
        *ptr++ = carr[d4 + 1];
    }
    return ptr;
}

static inline char* itoa(std::int32_t value, char *ptr) noexcept
{
    std::uint32_t u = static_cast<std::uint32_t>(value);
    if (value < 0)
    {
        *ptr++ = '-';
        u = ~u + 1;
    }

    return itoa(u, ptr);
}

static inline char* itoa(std::uint64_t value, char* ptr) noexcept
{
    if (!value)
    {
        *ptr++ = '0';
        return ptr;
    }

    const char* carr = ch_arr();

    static const uint64_t kTen8 = 100000000;
    static const uint64_t kTen9 = kTen8 * 10;
    static const uint64_t kTen10 = kTen8 * 100;
    static const uint64_t kTen11 = kTen8 * 1000;
    static const uint64_t kTen12 = kTen8 * 10000;
    static const uint64_t kTen13 = kTen8 * 100000;
    static const uint64_t kTen14 = kTen8 * 1000000;
    static const uint64_t kTen15 = kTen8 * 10000000;
    static const uint64_t kTen16 = kTen8 * kTen8;

    if (value < kTen8)
    {
        const std::uint32_t v = static_cast<std::uint32_t>(value);
        if (v < 10000)
        {
            const std::uint32_t d1 = (v / 100) << 1;
            const std::uint32_t d2 = (v % 100) << 1;

            if (v >= 1000)
                *ptr++ = carr[d1];
            if (v >= 100)
                *ptr++ = carr[d1 + 1];
            if (v >= 10)
                *ptr++ = carr[d2];
            *ptr++ = carr[d2 + 1];
        }
        else
        {
            // value = bbbbcccc
            const std::uint32_t b = v / 10000;
            const std::uint32_t c = v % 10000;

            const std::uint32_t d1 = (b / 100) << 1;
            const std::uint32_t d2 = (b % 100) << 1;

            const std::uint32_t d3 = (c / 100) << 1;
            const std::uint32_t d4 = (c % 100) << 1;

            if (value >= 10000000)
                *ptr++ = carr[d1];
            if (value >= 1000000)
                *ptr++ = carr[d1 + 1];
            if (value >= 100000)
                *ptr++ = carr[d2];
            *ptr++ = carr[d2 + 1];

            *ptr++ = carr[d3];
            *ptr++ = carr[d3 + 1];
            *ptr++ = carr[d4];
            *ptr++ = carr[d4 + 1];
        }
    }
    else if (value < kTen16)
    {
        const std::uint32_t v0 = static_cast<std::uint32_t>(value / kTen8);
        const std::uint32_t v1 = static_cast<std::uint32_t>(value % kTen8);

        const std::uint32_t b0 = v0 / 10000;
        const std::uint32_t c0 = v0 % 10000;

        const std::uint32_t d1 = (b0 / 100) << 1;
        const std::uint32_t d2 = (b0 % 100) << 1;

        const std::uint32_t d3 = (c0 / 100) << 1;
        const std::uint32_t d4 = (c0 % 100) << 1;

        const std::uint32_t b1 = v1 / 10000;
        const std::uint32_t c1 = v1 % 10000;

        const std::uint32_t d5 = (b1 / 100) << 1;
        const std::uint32_t d6 = (b1 % 100) << 1;

        const std::uint32_t d7 = (c1 / 100) << 1;
        const std::uint32_t d8 = (c1 % 100) << 1;

        if (value >= kTen15)
            *ptr++ = carr[d1];
        if (value >= kTen14)
            *ptr++ = carr[d1 + 1];
        if (value >= kTen13)
            *ptr++ = carr[d2];
        if (value >= kTen12)
            *ptr++ = carr[d2 + 1];
        if (value >= kTen11)
            *ptr++ = carr[d3];
        if (value >= kTen10)
            *ptr++ = carr[d3 + 1];
        if (value >= kTen9)
            *ptr++ = carr[d4];
        if (value >= kTen8)
            *ptr++ = carr[d4 + 1];

        *ptr++ = carr[d5];
        *ptr++ = carr[d5 + 1];
        *ptr++ = carr[d6];
        *ptr++ = carr[d6 + 1];
        *ptr++ = carr[d7];
        *ptr++ = carr[d7 + 1];
        *ptr++ = carr[d8];
        *ptr++ = carr[d8 + 1];
    }
    else
    {
        const uint32_t a = static_cast<uint32_t>(value / kTen16); // 1 to 1844
        value %= kTen16;

        if (a < 10)
            *ptr++ = static_cast<char>('0' + static_cast<char>(a));
        else if (a < 100)
        {
            const std::uint32_t i = a << 1;
            *ptr++ = carr[i];
            *ptr++ = carr[i + 1];
        }
        else if (a < 1000)
        {
            *ptr++ = static_cast<char>('0' + static_cast<char>(a / 100));

            const std::uint32_t i = (a % 100) << 1;
            *ptr++ = carr[i];
            *ptr++ = carr[i + 1];
        }
        else
        {
            const std::uint32_t i = (a / 100) << 1;
            const std::uint32_t j = (a % 100) << 1;
            *ptr++ = carr[i];
            *ptr++ = carr[i + 1];
            *ptr++ = carr[j];
            *ptr++ = carr[j + 1];
        }

        const std::uint32_t v0 = static_cast<std::uint32_t>(value / kTen8);
        const std::uint32_t v1 = static_cast<std::uint32_t>(value % kTen8);

        const std::uint32_t b0 = v0 / 10000;
        const std::uint32_t c0 = v0 % 10000;

        const std::uint32_t d1 = (b0 / 100) << 1;
        const std::uint32_t d2 = (b0 % 100) << 1;

        const std::uint32_t d3 = (c0 / 100) << 1;
        const std::uint32_t d4 = (c0 % 100) << 1;

        const std::uint32_t b1 = v1 / 10000;
        const std::uint32_t c1 = v1 % 10000;

        const std::uint32_t d5 = (b1 / 100) << 1;
        const std::uint32_t d6 = (b1 % 100) << 1;

        const std::uint32_t d7 = (c1 / 100) << 1;
        const std::uint32_t d8 = (c1 % 100) << 1;

        *ptr++ = carr[d1];
        *ptr++ = carr[d1 + 1];
        *ptr++ = carr[d2];
        *ptr++ = carr[d2 + 1];
        *ptr++ = carr[d3];
        *ptr++ = carr[d3 + 1];
        *ptr++ = carr[d4];
        *ptr++ = carr[d4 + 1];
        *ptr++ = carr[d5];
        *ptr++ = carr[d5 + 1];
        *ptr++ = carr[d6];
        *ptr++ = carr[d6 + 1];
        *ptr++ = carr[d7];
        *ptr++ = carr[d7 + 1];
        *ptr++ = carr[d8];
        *ptr++ = carr[d8 + 1];
    }

    return ptr;
}

static inline char* itoa(std::int64_t value, char *ptr) noexcept
{
    std::uint64_t u = static_cast<std::uint64_t>(value);
    if (value < 0)
    {
        *ptr++ = '-';
        u = ~u + 1;
    }

    return itoa(u, ptr);
}

} // namespace detail

template<typename T>
char* itoa(T value, char *ptr) noexcept
{
    return detail::itoa(value, ptr);
}

static inline char* int32toa(std::int32_t value, char *ptr) noexcept
{
    return detail::itoa(value, ptr);
}

static inline char* int64toa(std::int64_t value, char *ptr) noexcept
{
    return detail::itoa(value, ptr);
}

static inline char* uint32toa(std::uint32_t value, char *ptr) noexcept
{
    return detail::itoa(value, ptr);
}

static inline char* uint64toa(std::uint64_t value, char *ptr) noexcept
{
    return detail::itoa(value, ptr);
}

} // namespace num
} // namespace btdef
