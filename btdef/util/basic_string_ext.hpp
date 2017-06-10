#pragma once

#include "btdef/util/basic_string.hpp"
#include "btdef/hash/fnv1a.hpp"

#include <string>

namespace std {

template<class A>
struct hash<btdef::util::basic_string<char, A>>
{
    size_t
    operator()(const btdef::util::basic_string<char, A>& str) const noexcept
    {
        btdef::hash::fnv1a fn;
        return static_cast<size_t>(fn(str.data(), str.size()));
    }
};

} // namespace std

template<typename C, typename T, typename A1, typename A2>
inline std::basic_string<C, T, A1>&
    operator+=(std::basic_string<C, T, A1>& lhs,
        const btdef::util::basic_string<C, A2>& rhs)
{
    return lhs.append(rhs.data(), rhs.size());
}

template<typename C, typename T, typename A1, typename A2>
inline std::basic_string<C, T, A1>
    operator+(const std::basic_string<C, T, A1>& lhs,
        const btdef::util::basic_string<C, A2>& rhs)
{
    std::basic_string<C, T, A1> result(lhs);
    result.append(rhs.data(), rhs.size());
    return result;
}

template<typename C, typename T, typename A1, typename A2>
inline std::basic_string<C, T, A1>
    operator+(std::basic_string<C, T, A1>&& lhs,
        const btdef::util::basic_string<C, A2>& rhs)
{
    return std::move(lhs.append(rhs.data(), rhs.size()));
}

template<class C, class T, class A>
std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& os,
    const btdef::util::basic_string<C, A>& rhs)
{
    return os.write(rhs.data(), rhs.size());
}

// ----

template<typename C, typename T, typename A1, typename A2>
bool operator==(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) noexcept
{
    return rhs.compare(lhs) == 0;
}

template<typename C, typename T, typename A1, typename A2>
bool operator!=(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<typename C, typename T, typename A1, typename A2>
bool operator<(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) noexcept
{
    return rhs > lhs;
}

template<typename C, typename T, typename A1, typename A2>
bool operator>(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) noexcept
{
    return rhs < lhs;
}

template<typename C, typename T, typename A1, typename A2>
bool operator<=(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) noexcept
{
    return !(rhs < lhs);
}

template<typename C, typename T, typename A1, typename A2>
bool operator>=(const std::basic_string<C, T, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) noexcept
{
    return !(lhs < rhs);
}
