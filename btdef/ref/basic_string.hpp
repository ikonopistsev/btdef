// Copyright © 2017 igor . ikonopistsev at gmail
// This work is free. You can redistribute it and/or modify it under the
// terms of the Do What The Fuck You Want To Public License, Version 2,
// as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.

#pragma once

#include "btdef/hash/fnv1a.hpp"

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
struct empty_string;

template<>
struct empty_string<char>
{
    static inline const char* value() noexcept
    {
        return "";
    }
};

template<>
struct empty_string<wchar_t>
{
    static inline const wchar_t* value() noexcept
    {
        return L"";
    }
};

} //  namespace detail

template<typename C>
class basic_string
{
public:
    typedef C value_t;
    typedef const value_t* const_pointer;
    typedef const_pointer const_iterator;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    typedef basic_string<C> this_type;

private:
    const_pointer ptr_;
    size_type size_;

    // res::string::с_str() is not allow
    // use data() and size() to get part of string
    const_pointer c_str() const noexcept;

public:

    static const auto npos = static_cast<size_type>(-1);

    basic_string() noexcept
        : ptr_(detail::empty_string<value_t>::value())
        , size_(0)
    {   }

    basic_string(const this_type& other) noexcept
        : ptr_(other.ptr_)
        , size_(other.size_)
    {   }

    basic_string(this_type&& other) noexcept
    {
        swap(other);
    }

    this_type& operator=(const this_type& other) noexcept
    {
        ptr_ = other.ptr_;
        size_ = other.size_;
        return *this;
    }

    this_type& operator=(this_type&& other) noexcept
    {
        swap(other);
        return *this;
    }

    basic_string(const_pointer ptr, size_type size) noexcept
        : ptr_(ptr)
        , size_(size)
    {   }

    // запрещаем инициализцию из константной строки
    // используй string(std::ref("123"));
    basic_string(const_pointer) = delete;

    template<size_type N>
    basic_string(
        const std::reference_wrapper<const value_t[N]>& str) noexcept
        : basic_string(str.get(), N - 1)
    {   }

    template<class T>
    basic_string(const T& str) noexcept
        : basic_string(str.data(), str.size())
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

    const_pointer data() const noexcept
    {
        return ptr_;
    }

    bool empty() const noexcept
    {
        return size_ == 0;
    }

    size_type size() const noexcept
    {
        return size_;
    }

    int compare(const_pointer value, size_type len) const noexcept
    {
        int ret = std::memcmp(ptr_, value, (std::min)(size_, len));
        if (!ret)
        {
            if (size_ < len)
                return -1;
            if (size_ > len)
                return 1;
        }
        return ret;
    }

    int compare(size_type pos, size_type count1,
        const_pointer value, size_type count2) const noexcept
    {
        if (pos < size_)
            count1 = (std::min)(size_ - pos, count1);
        else
            count1 = 0;

        int ret = std::memcmp(ptr_ + pos, value, (std::min)(count1, count2));
        if (!ret)
        {
            if (count1 < count2)
                return -1;
            if (count1 > count2)
                return 1;
        }
        return ret;
    }

    template<class T>
    int compare(const T& other) const noexcept
    {
        return compare(other.data(), other.size());
    }

    template<class T>
    int compare(size_type pos, size_type len, const T& other) const noexcept
    {
        return compare(pos, len, other.data(), other.size());
    }

    int compare(const_pointer value) const noexcept
    {
        assert(value);
        return compare(value, std::strlen(value));
    }

    int compare(size_type pos, size_type len,
        const_pointer value) const noexcept
    {
        assert(value);
        return compare(pos, len, value, std::strlen(value));
    }

    bool operator==(const this_type& other) const noexcept
    {
        return compare(other) == 0;
    }

    bool operator==(const_pointer value) const noexcept
    {
        return compare(value) == 0;
    }

    template<size_type N>
    bool operator==(
        const std::reference_wrapper<const value_t[N]>& ref) const noexcept
    {
        return compare(ref.get(), N - 1) == 0;
    }

    template<class T>
    bool operator==(const T& str) const noexcept
    {
        return compare(str.data(), str.size()) == 0;
    }

    bool operator<(const this_type& other) const noexcept
    {
        return compare(other) < 0;
    }

    bool operator<(const_pointer value) const noexcept
    {
        return compare(value) < 0;
    }

    template<size_type N>
    bool operator<(
        const std::reference_wrapper<const value_t[N]>& ref) const noexcept
    {
        return compare(ref.get(), N - 1) < 0;
    }

    template<class T>
    bool operator<(const T& str) const noexcept
    {
        return compare(str.data(), str.size()) < 0;
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

    size_type find(const this_type& s) const
    {
        auto it = std::search(cbegin(), cend(), s.cbegin(), s.cend());
        return it == cend() ? npos : std::distance(cbegin(), it);
    }

    size_type find(value_t c) const
    {
        auto it = std::find(cbegin(), cend(), c);
        return it == cend() ? npos : std::distance(cbegin(), it);
    }

    size_type rfind(const this_type& s) const
    {
        const_reverse_iterator iter = std::search(crbegin(), crend(),
            s.crbegin(), s.crend());
        return iter == this->crend () ?
            npos : (std::distance(iter, crend()) - s.size());
    }

    size_type rfind(value_t c) const
    {
        const_reverse_iterator iter = std::find_if(crbegin(), crend(),
            [=](value_t val){
                return c == val;
            });
        return iter == crend() ?
            npos : (size() - 1 - std::distance(crbegin(), iter));
    }

    size_type find_first_of(const this_type& s) const
    {
        const_iterator iter = std::find_first_of(cbegin(),
            cend(), s.cbegin(), s.cend());
        return iter == cend() ? npos : std::distance(cbegin(), iter);
    }

    size_type find_first_of(value_t c) const
    {
        return find(c);
    }

    size_type find_last_of(const this_type& s) const
    {
        const_reverse_iterator iter = std::find_first_of(crbegin(),
            crend(), s.cbegin(), s.cend());
        return iter == crend () ?
            npos : (size() - 1 - std::distance(crbegin(), iter));
    }

    size_type find_last_of(value_t c) const
    {
        return rfind(c);
    }

    this_type substr(size_type pos, size_type n = npos) const noexcept
    {
        if (pos > size_)
            throw std::out_of_range("basic_string::substr");

        if ((n == npos) || ((pos + n) > size_))
            n = size_ - pos;

        return this_type(ptr_ + pos, n);
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

    this_type& assign(const_pointer str, size_type size) noexcept
    {
        return *this = this_type(str, size);
    }

    template<class T>
    this_type& assign(const T& str) noexcept
    {
        return *this = this_type(str.data(), str.size());
    }

    const value_t& front() const
    {
        return at(0);
    }

    const value_t& back() const
    {
        return at(size_ - 1);
    }

    const value_t& operator[](size_type pos) const noexcept
    {
        assert(pos < size_);
        return ptr_[pos];
    }

    const value_t& at(size_type pos) const
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
};

} // namespace btref

// ---- btdef::util::basic_string same type

template<class C>
bool operator==(const btref::basic_string<C>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}

template<class C>
bool operator!=(const btref::basic_string<C>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<class C>
bool operator<(const btref::basic_string<C>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<class C>
bool operator>(const btref::basic_string<C>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return rhs < lhs;
}

template<class C>
bool operator<=(const btref::basic_string<C>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return !(rhs < lhs);
}

template<class C>
bool operator>=(const btref::basic_string<C>& lhs,
    const btref::basic_string<C>& rhs) noexcept
{
    return !(lhs < rhs);
}

// ---- btdef::util::basic_string same type

//---

template<class C, class A>
bool operator==(const btdef::util::basic_string<C, A>& lhs,
    const C* rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}

template<class C, class A>
bool operator!=(const btdef::util::basic_string<C, A>& lhs,
    const C* rhs) noexcept
{
    return !(lhs == rhs);
}

template<class C, class A>
bool operator<(const btdef::util::basic_string<C, A>& lhs,
    const C* rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<class C, class A>
bool operator>(const btdef::util::basic_string<C, A>& lhs,
    const C* rhs) noexcept
{
    return lhs.compare(rhs) > 0;
}

template<class C, class A>
bool operator<=(const btdef::util::basic_string<C, A>& lhs,
    const C* rhs) noexcept
{
    return !(rhs < lhs);
}

template<class C, class A>
bool operator>=(const btdef::util::basic_string<C, A>& lhs,
    const C* rhs) noexcept
{
    return !(lhs < rhs);
}

//---

// ---- btdef::util::basic_string other type

template<class C, class F>
bool operator==(const btref::basic_string<C>& lhs,
    const F& rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}

template<class C, class F>
bool operator!=(const btref::basic_string<C>& lhs,
    const F& rhs) noexcept
{
    return !(lhs == rhs);
}

template<class C, class F>
bool operator<(const btref::basic_string<C>& lhs,
    const F& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<class C, class F>
bool operator>(const btref::basic_string<C>& lhs,
    const F& rhs) noexcept
{
    return lhs.compare(rhs) > 0;
}

template<class C, class F>
bool operator<=(const btref::basic_string<C>& lhs,
    const F& rhs) noexcept
{
    return !(rhs < lhs);
}

template<class C, class F>
bool operator>=(const btref::basic_string<C>& lhs,
    const F& rhs) noexcept
{
    return !(lhs < rhs);
}

// ---- btdef::util::basic_string other type

