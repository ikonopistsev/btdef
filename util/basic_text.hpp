#pragma once

#include "btdef/config.hpp"
#include "btdef/util/util.hpp"
#include "btdef/config.hpp"

#include <cstring>

#include <iterator>
#include <algorithm>
#include <functional>

namespace btdef {
namespace util {

template<class C, std::size_t L>
class basic_text;

template<std::size_t L>
class basic_text<char, L>
{
public:
    typedef char value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    enum {
        cache_size = BTDEF_ALLOCATOR_ALIGN(L),
        cache_capacity = cache_size - 1
    };

private:
    mutable value_type data_[cache_size];
    size_type size_{0};

public:
    basic_text() = default;

    basic_text(const_pointer value, size_type len) BTDEF_NOEXCEPT
    {
        assign(value, len);
    }

    basic_text(size_type len, value_type value) BTDEF_NOEXCEPT
    {
        assign(len, value);
    }

    basic_text(const basic_text& other) BTDEF_NOEXCEPT
    {
        assign(other.data(), other.size());
    }

    basic_text(const_pointer value) BTDEF_NOEXCEPT
    {
        assign(value);
    }

    template<size_type N>
    basic_text(std::reference_wrapper<const value_type[N]> ref) BTDEF_NOEXCEPT
    {
        assign(ref.get(), N - 1);
    }

    template<class T>
    basic_text(const T& other) BTDEF_NOEXCEPT
    {
        assign(other.data(), other.size());
    }

    size_type assign(const_pointer value, size_type len) BTDEF_NOEXCEPT
    {
        if (len < cache_size)
        {
            size_ = len;
            if (len)
            {
                assert(value);
                std::memcpy(data_, value, len);
            }
            return len;
        }
        return 0;
    }

    size_type assign(value_type value) BTDEF_NOEXCEPT
    {
        size_ = 1;
        *data_ = value;
        return size_;
    }

    size_type assign(const_pointer value) BTDEF_NOEXCEPT
    {
        assert(value);
        return assign(value, std::strlen(value));
    }

    template<value_type N>
    size_type assign(std::reference_wrapper<const value_type[N]> ref) BTDEF_NOEXCEPT
    {
        return assign(ref.get(), N - 1);
    }

    template<class T>
    size_type assign(const T& other) BTDEF_NOEXCEPT
    {
        return assign(other.begin(), other.size());
    }

    size_type assign(size_type n, char value) BTDEF_NOEXCEPT
    {
        if (n < cache_size)
        {
            size_ = n;
            std::memset(data_, value, n);
            return n;
        }
        return 0;
    }

    size_type operator=(const basic_text& other) BTDEF_NOEXCEPT
    {
        return assign(other.data(), other.size());
    }

    size_type operator=(const_pointer value) BTDEF_NOEXCEPT
    {
        return assign(value);
    }

    template<class T>
    size_type operator=(const T& other) BTDEF_NOEXCEPT
    {
        return assign(other.data(), other.size());
    }

    template<std::size_t N>
    size_type operator=(std::reference_wrapper<const value_type[N]> r) BTDEF_NOEXCEPT
    {
        return assign(r.get(), N - 1);
    }

    int compare(const_pointer value, size_type len) const BTDEF_NOEXCEPT
    {
        int ret = std::memcmp(data_, value, (std::min)(size_, len));
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
        const_pointer value, size_type count2) const BTDEF_NOEXCEPT
    {
        if (pos < size_)
            count1 = (std::min)(size_ - pos, count1);
        else
            count1 = 0;

        int ret = std::memcmp(data_ + pos, value, (std::min)(count1, count2));
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
    int compare(const T& other) const BTDEF_NOEXCEPT
    {
        return compare(other.data(), other.size());
    }

    template<class T>
    int compare(size_type pos, size_type len, const T& other) const BTDEF_NOEXCEPT
    {
        return compare(pos, len, other.data(), other.size());
    }

    int compare(const_pointer value) const BTDEF_NOEXCEPT
    {
        assert(value);
        return compare(value, std::strlen(value));
    }

    int compare(size_type pos, size_type len,
        const_pointer value) const BTDEF_NOEXCEPT
    {
        assert(value);
        return compare(pos, len, value, std::strlen(value));
    }

    bool operator==(const basic_text& rhs) const BTDEF_NOEXCEPT
    {
        return compare(rhs) == 0;
    }

    bool operator<(const basic_text& rhs) const BTDEF_NOEXCEPT
    {
        return compare(rhs) < 0;
    }

    template<class T>
    bool starts_with(const T& other) const BTDEF_NOEXCEPT
    {
        return (size_ >= other.size()) &&
            (std::memcmp(data_, other.data(), other.size()) == 0);
    }

    template<class T>
    bool ends_with(const T& other) const BTDEF_NOEXCEPT
    {
        size_type sz = other.size();
        return (size_ >= sz) &&
           (std::memcmp(data_ + (size_ - sz), other.data(), sz) == 0);
    }

    reference operator[](size_type i) BTDEF_NOEXCEPT
    {
        return data_[i];
    }

    const_reference operator[](size_type i) const BTDEF_NOEXCEPT
    {
        return data_[i];
    }

    size_type size() const BTDEF_NOEXCEPT
    {
        return size_;
    }

    bool empty() const BTDEF_NOEXCEPT
    {
        return !size_;
    }

    void clear() BTDEF_NOEXCEPT
    {
        size_ = 0;
    }

    reference front() BTDEF_NOEXCEPT
    {
        return data_[0];
    }

    const_reference front() const BTDEF_NOEXCEPT
    {
        return data_[0];
    }

    reference back() BTDEF_NOEXCEPT
    {
        return data_[size_ - 1];
    }

    const_reference back() const BTDEF_NOEXCEPT
    {
        return data_[size_ - 1];
    }

    iterator begin() BTDEF_NOEXCEPT
    {
        return data_;
    }

    const_iterator begin() const BTDEF_NOEXCEPT
    {
        return data_;
    }

    const_iterator cbegin() const BTDEF_NOEXCEPT
    {
        return data_;
    }

    iterator end() BTDEF_NOEXCEPT
    {
        return data_ + size_;
    }

    reverse_iterator rbegin() BTDEF_NOEXCEPT
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const BTDEF_NOEXCEPT
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const BTDEF_NOEXCEPT
    {
        return rbegin();
    }

    const_iterator end() const BTDEF_NOEXCEPT
    {
        return data_ + size_;
    }

    const_iterator cend() const BTDEF_NOEXCEPT
    {
        return end();
    }

    reverse_iterator rend() BTDEF_NOEXCEPT
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const BTDEF_NOEXCEPT
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const BTDEF_NOEXCEPT
    {
        return rend();
    }

    const_pointer data() const BTDEF_NOEXCEPT
    {
        return data_;
    }

    pointer data() BTDEF_NOEXCEPT
    {
        return data_;
    }

    const_pointer c_str() const BTDEF_NOEXCEPT
    {
        data_[size_] = '\0';
        return data_;
    }

    constexpr size_type capacity() const BTDEF_NOEXCEPT
    {
        return cache_capacity;
    }

    size_type push_back(value_type value) BTDEF_NOEXCEPT
    {
        return append(value);
    }

    void pop_back() BTDEF_NOEXCEPT
    {
        if (size_)
            --size_;
    }

    size_type resize(size_type len) BTDEF_NOEXCEPT
    {
        if (len < cache_size)
        {
            if (len < size_)
            {
                size_ = len;
                return len;
            }
            return size_;
        }
        return 0;
    }

    void reserve(size_type) BTDEF_NOEXCEPT
    {   }

    size_type free_size() const BTDEF_NOEXCEPT
    {
        return cache_capacity - size_;
    }

    void increase(size_type size) BTDEF_NOEXCEPT
    {
        assert(size < free_size());
        size_ = size;
    }

    size_type append(const_pointer value, size_type len) BTDEF_NOEXCEPT
    {
        if (len <= free_size())
        {
            if (len)
            {
                assert(value);
                std::memcpy(end(), value, len);
                size_ += len;
            }
            return size_;
        }
        return 0;
    }

    size_type append(const basic_text& other) BTDEF_NOEXCEPT
    {
        return append(other.begin(), other.size());
    }

    template<class T>
    size_type append(const T& other) BTDEF_NOEXCEPT
    {
        return append(other.begin(), other.size());
    }

    size_type append(value_type value) BTDEF_NOEXCEPT
    {
        if (size_ < cache_capacity)
        {
            data_[size_++] = value;
            return size_;
        }
        return 0;
    }

    size_type append(size_type n, value_type value) BTDEF_NOEXCEPT
    {
        if (n <= free_size())
        {
            if (n)
            {
                std::memset(end(), value, n);
                size_ += n;
            }
            return size_;
        }
        return 0;
    }

    size_type append(const_pointer value) BTDEF_NOEXCEPT
    {
        return append(value, std::strlen(value));
    }

    template<size_type N>
    size_type append(std::reference_wrapper<const value_type[N]> ref) BTDEF_NOEXCEPT
    {
        return append(ref.get(), N - 1);
    }

    size_type operator+=(const basic_text& other) BTDEF_NOEXCEPT
    {
        return append(other.data(), other.size());
    }

    template<class T>
    size_type operator+=(const T& other) BTDEF_NOEXCEPT
    {
        return append(other.data(), other.size());
    }

    size_type operator+=(value_type value) BTDEF_NOEXCEPT
    {
        return append(value);
    }

    size_type operator+=(const_pointer value) BTDEF_NOEXCEPT
    {
        return append(value);
    }

    template<size_type N>
    size_type operator+=(std::reference_wrapper<const value_type[N]> r) BTDEF_NOEXCEPT
    {
        return append(r.get(), N - 1);
    }

    void swap(basic_text& other) BTDEF_NOEXCEPT
    {
        basic_text t(*this);
        *this = other;
        other = t;
    }
};

} // namespace util
} // namespace btdef

// ---- btdef::util::basic_text same type

template<class C, std::size_t N>
bool operator==(const btdef::util::basic_text<C, N>& lhs,
    const btdef::util::basic_text<C, N>& rhs) BTDEF_NOEXCEPT
{
    return lhs.compare(rhs) == 0;
}

template<class C, std::size_t N>
bool operator<(const btdef::util::basic_text<C, N>& lhs,
    const btdef::util::basic_text<C, N>& rhs) BTDEF_NOEXCEPT
{
    return lhs.compare(rhs) < 0;
}

template<class C, std::size_t N>
bool operator!=(const btdef::util::basic_text<C, N>& lhs,
    const btdef::util::basic_text<C, N>& rhs) BTDEF_NOEXCEPT
{
    return !(lhs == rhs);
}

template<class C, std::size_t N>
bool operator<=(const btdef::util::basic_text<C, N>& lhs,
    const btdef::util::basic_text<C, N>& rhs) BTDEF_NOEXCEPT
{
    return !(rhs < lhs);
}

template<class C, std::size_t N>
bool operator>=(const btdef::util::basic_text<C, N>& lhs,
    const btdef::util::basic_text<C, N>& rhs) BTDEF_NOEXCEPT
{
    return !(lhs < rhs);
}

// ---- btdef::util::basic_text same type
// ---- btdef::util::basic_text other type

template<class C, class F, std::size_t N>
bool operator==(const btdef::util::basic_text<C, N>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return lhs.compare(rhs) == 0;
}

template<class C, class F, std::size_t N>
bool operator!=(const btdef::util::basic_text<C, N>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return !(lhs == rhs);
}

template<class C, class F, std::size_t N>
bool operator<(const btdef::util::basic_text<C, N>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return lhs.compare(rhs) < 0;
}

template<class C, class F, std::size_t N>
bool operator>(const btdef::util::basic_text<C, N>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return lhs.compare(rhs) > 0;
}

template<class C, class F, std::size_t N>
bool operator<=(const btdef::util::basic_text<C, N>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return !(rhs < lhs);
}

template<class C, class F, std::size_t N>
bool operator>=(const btdef::util::basic_text<C, N>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return !(lhs < rhs);
}

// ---- btdef::util::basic_text other type
