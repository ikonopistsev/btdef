// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "bthash.hpp"

#include <iosfwd>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <functional>
#include <stdexcept>
#include <cassert>
#include <cstring>

namespace btref {

template<typename T>
class basic_string;

namespace detail {

template<typename T>
struct default_string;

template<>
struct default_string<char>
{
    static inline const char* value() noexcept
    {
        return "";
    }
};

template<>
struct default_string<wchar_t>
{
    static inline const wchar_t* value() noexcept
    {
        return L"";
    }
};

} //  namespace detail

template<typename T>
class basic_string
{
public:
    typedef T value_t;
    typedef const value_t* pointer;
    typedef pointer const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef basic_string<T> this_type;

private:
    pointer ptr_;
    std::size_t size_;

    // res::string::с_str() is not allow
    // use data() and size() to get part of string
    pointer c_str() const noexcept
    {
        return nullptr;
    }

public:

    static const auto npos = static_cast<std::size_t>(-1);

    basic_string() noexcept
        : ptr_(detail::default_string<value_t>::value())
        , size_(0)
    {   }

    basic_string(const this_type& other) noexcept
        : ptr_(other.ptr_)
        , size_(other.size_)
    {   }

    this_type& operator=(const this_type& other) noexcept
    {
        ptr_ = other.ptr_;
        size_ = other.size_;
        return *this;
    }

    basic_string(pointer ptr, std::size_t size) noexcept
        : ptr_(ptr)
        , size_(size)
    {
        assert(ptr_);
    }

    // запрещаем инициализцию из константной строки
    // используй string(std::ref("123"));
    basic_string(pointer) = delete;

    template<std::size_t N>
    basic_string(
        const std::reference_wrapper<const value_t[N]>& str) noexcept
        : basic_string(str.get(), N - 1)
    {   }

    basic_string(const std::basic_string<value_t>& str) noexcept
        : basic_string(str.c_str(), str.size())
    {   }

    const_iterator cbegin() const noexcept
    {
        return ptr_;
    }

    const_iterator cend() const noexcept
    {
        return ptr_ + size_;
    }

    const_iterator begin() const noexcept
    {
        return cbegin();
    }

    const_iterator end() const noexcept
    {
        return cend();
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator(begin());
    }


    pointer data() const noexcept
    {
        return ptr_;
    }


    bool empty() const noexcept
    {
        return size_ == 0;
    }

    std::size_t size() const noexcept
    {
        return size_;
    }

    std::size_t length() const noexcept
    {
        return size_;
    }

    std::size_t max_size() const noexcept
    {
        return size_;
    }

    int compare(const this_type& x) const noexcept
    {
        int res = std::memcmp(ptr_, x.ptr_, (std::min)(size_, x.size_));
        if (res)
            return res;
        return (size_ < x.size_) ? -1 : 1;
    }

    bool starts_with(const this_type& x) const noexcept
    {
        return size_ >= x.size_ && (std::memcmp(ptr_, x.ptr_, x.size_) == 0);
    }

    bool ends_with(const this_type& x) const noexcept
    {
        return size_ >= x.size_ &&
           (std::memcmp(ptr_ + (size_ - x.size_), x.ptr_, x.size_) == 0);
    }

    std::size_t find(const this_type& s) const
    {
        auto it = std::search(cbegin(), cend(), s.cbegin(), s.cend());
        return it == cend() ? npos : std::distance(cbegin(), it);
    }

    std::size_t find(value_t c) const
    {
        auto it = std::find(cbegin(), cend(), c);
        return it == cend() ? npos : std::distance(cbegin(), it);
    }

    std::size_t rfind(const this_type& s) const
    {
        const_reverse_iterator iter = std::search(crbegin(), crend(),
            s.crbegin(), s.crend());
        return iter == this->crend () ?
            npos : (std::distance(iter, crend()) - s.size());
    }

    std::size_t rfind(value_t c) const
    {
        const_reverse_iterator iter = std::find_if(crbegin(), crend(),
            [=](value_t val){
                return c == val;
            });
        return iter == crend() ?
            npos : (size() - 1 - std::distance(crbegin(), iter));
    }

    std::size_t find_first_of(const this_type& s) const
    {
        const_iterator iter = std::find_first_of(cbegin(),
            cend(), s.cbegin(), s.cend());
        return iter == cend() ? npos : std::distance(cbegin(), iter);
    }

    std::size_t find_first_of(value_t c) const
    {
        return find(c);
    }

    std::size_t find_last_of(const this_type& s) const
    {
        const_reverse_iterator iter = std::find_first_of(crbegin(),
            crend(), s.cbegin(), s.cend());
        return iter == crend () ?
            npos : (size() - 1 - std::distance(crbegin(), iter));
    }

    std::size_t find_last_of(value_t c) const
    {
        return rfind(c);
    }

    this_type substr(size_t pos, size_t n = npos) const noexcept
    {
        if (pos > size())
            throw std::out_of_range("basic_string::substr");

        if (n == npos || pos + n > size())
            n = size () - pos;

        return this_type(data() + pos, n);
    }

    this_type& assign(const this_type& other) noexcept
    {
        return *this = other;
    }

    template<std::size_t N>
    this_type& assign(
        const std::reference_wrapper<const value_t[N]>& str_ref) noexcept
    {
        return *this = this_type(str_ref);
    }

    this_type& assign(pointer str, std::size_t size) noexcept
    {
        return *this = this_type(str, size);
    }

    this_type& assign(const std::basic_string<value_t>& str) noexcept
    {
        return *this = this_type(str.c_str(), str.size());
    }

    const value_t& front() const noexcept
    {
        return at(0);
    }

    const value_t& back() const noexcept
    {
        assert(size_);
        return ptr_[size_ - 1];
    }

    const value_t& operator[](std::size_t pos) const noexcept
    {
        assert(pos < size_);
        return ptr_[pos];
    }

    const value_t& at(std::size_t pos) const noexcept
    {
        if (pos >= size_)
            throw std::out_of_range("basic_string::at");

        return ptr_[pos];
    }

    void swap(this_type& other) noexcept
    {
        this_type t(*this);
        *this = other;
        other = t;
    }

    std::basic_string<value_t> str() const
    {
        assert(ptr_);
        return std::basic_string<value_t>(ptr_, size_);
    }
};

template<typename T, typename R>
std::basic_ostream<T, R>& operator<<(std::basic_ostream<T, R>& os,
    const basic_string<T>& str)
{
    return os.write(str.data(), str.size());
}

template<typename T>
bool operator==(const basic_string<T>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}

template<typename T, typename R, typename A>
bool operator==(const basic_string<T>& lhs,
    const std::basic_string<T, R, A>& rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}

template<typename T, typename R, typename A>
bool operator==(const std::basic_string<T, R, A>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) == 0;
}

template<typename T>
bool operator!=(const basic_string<T>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return !(lhs.compare(rhs) == 0);
}

template<typename T, typename R, typename A>
bool operator!=(const basic_string<T>& lhs,
    const std::basic_string<T, R, A>& rhs) noexcept
{
    return !(lhs.compare(rhs) == 0);
}

template<typename T, typename R, typename A>
bool operator!=(const std::basic_string<T, R, A>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return !(lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) == 0);
}

template<typename T>
bool operator<(const basic_string<T>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<typename T, typename R, typename A>
bool operator<(const basic_string<T>& lhs,
    const std::basic_string<T, R, A>& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<typename T, typename R, typename A>
bool operator<(const std::basic_string<T, R, A>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) < 0;
}

template<typename T>
bool operator>(const basic_string<T>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return lhs.compare(rhs) > 0;
}

template<typename T, typename R, typename A>
bool operator>(const basic_string<T>& lhs,
    const std::basic_string<T, R, A>& rhs) noexcept
{
    return lhs.compare(rhs) > 0;
}

template<typename T, typename R, typename A>
bool operator>(const std::basic_string<T, R, A>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) > 0;
}

template<typename T>
bool operator<=(const basic_string<T>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return lhs.compare(rhs) <= 0;
}

template<typename T, typename R, typename A>
bool operator<=(const basic_string<T>& lhs,
    const std::basic_string<T, R, A>& rhs) noexcept
{
    return lhs.compare(rhs) <= 0;
}

template<typename T, typename R, typename A>
bool operator<=(const std::basic_string<T, R, A>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) <= 0;
}

template<typename T>
bool operator>=(const basic_string<T>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return lhs.compare(rhs) >= 0;
}

template<typename T, typename R, typename A>
bool operator>=(const basic_string<T>& lhs,
    const std::basic_string<T, R, A>& rhs) noexcept
{
    return lhs.compare(rhs) >= 0;
}

template<typename T, typename R, typename A>
bool operator>=(const std::basic_string<T, R, A>& lhs,
    const basic_string<T>& rhs) noexcept
{
    return lhs.compare(0, lhs.size(), rhs.data(), rhs.size()) >= 0;
}

} // namespace btref


namespace std {

template<>
struct hash<btref::basic_string<char>>
{
    size_t operator()(const btref::basic_string<char>& str) const noexcept
    {
        bthash::hash::fnv1a fn;
        return static_cast<size_t>(fn(str.data(), str.size()));
    }
};

} // namespace std
