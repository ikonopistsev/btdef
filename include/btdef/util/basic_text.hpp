﻿#pragma once

#include "btdef/config.hpp"
#include "btdef/util/util.hpp"

#include <cstring>

#include <iterator>
#include <algorithm>
#include <functional>
#include <string_view>

namespace btdef {
namespace util {

template<class, std::size_t>
class basic_text;

template<std::size_t L>
class basic_text<char, L>
{
public:
    using value_type = char;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator =  value_type*;
    using const_iterator = const value_type*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using sv_type = std::basic_string_view<value_type>;

    enum {
        cache_size = BTDEF_ALLOCATOR_ALIGN(L),
        cache_capacity = cache_size - 1
    };

private:
    mutable value_type data_[cache_size];
    size_type size_{ };

    static sv_type to_string_view(sv_type text) noexcept
    {
        return text;
    }

    struct sv_wrap
    {
        sv_type text_;
        explicit sv_wrap(sv_type text) noexcept
            : text_{text}
        {   }
    };

    explicit basic_text(sv_wrap svw) noexcept
        : basic_text{svw.text_.data(), svw.text_.size()}
    {   }

public:
    basic_text() = default;
    basic_text(const basic_text&) = default;
    basic_text& operator=(const basic_text&) = default;

    basic_text(const_pointer value, size_type len) noexcept
    {
        assign(value, len);
    }

    template<typename T>
    basic_text(const T& str) noexcept
        : basic_text{sv_wrap{to_string_view(str)}}
    {   }

    basic_text(size_type len, value_type value) noexcept
    {
        assign(len, value);
    }

    basic_text(const_pointer value) noexcept
    {
        assign(value);
    }

    size_type assign(const basic_text& other) noexcept
    {
        size_ = other.size();
        if (size_)
            std::memcpy(data_, other.data(), size_);
        return size_;
    }

    size_type assign(const_pointer value, size_type len) noexcept
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

    size_type assign(value_type value) noexcept
    {
        size_ = 1;
        *data_ = value;
        return size_;
    }

    size_type assign(const_pointer value) noexcept
    {
        assert(value);
        return assign(value, std::strlen(value));
    }

    template<class T>
    size_type assign(const T& other) noexcept
    {
        sv_wrap wr{to_string_view(other)};
        return assign(wr.text_.data(), wr.text_.size());
    }

    size_type assign(size_type n, char value) noexcept
    {
        if (n < cache_size)
        {
            size_ = n;
            std::memset(data_, value, n);
            return n;
        }
        return 0;
    }

    template<class T>
    bool starts_with(const T& other) const noexcept
    {
        sv_wrap wr{to_string_view(other)};
        auto size = wr.text_.size();
        return (size_ >= size) &&
            (std::memcmp(data_, wr.text_.data(), size) == 0);
    }

    template<class T>
    bool ends_with(const T& other) const noexcept
    {
        sv_wrap wr{to_string_view(other)};
        auto sz = wr.text_.size();
        return (size_ >= sz) &&
           (std::memcmp(data_ + (size_ - sz), wr.text_.data(), sz) == 0);
    }

    reference operator[](size_type i) noexcept
    {
        return data_[i];
    }

    const_reference operator[](size_type i) const noexcept
    {
        return data_[i];
    }

    basic_text& operator=(const_pointer value) noexcept
    {
        assign(value);
        return *this;
    }

    operator sv_type() const noexcept
    {
        return sv_type{data(), size()};
    }

    size_type size() const noexcept
    {
        return size_;
    }

    bool empty() const noexcept
    {
        return !size_;
    }

    void clear() noexcept
    {
        size_ = 0;
    }

    reference front() noexcept
    {
        return data_[0];
    }

    const_reference front() const noexcept
    {
        return data_[0];
    }

    reference back() noexcept
    {
        return data_[size_ - 1];
    }

    const_reference back() const noexcept
    {
        return data_[size_ - 1];
    }

    iterator begin() noexcept
    {
        return data_;
    }

    const_iterator begin() const noexcept
    {
        return data_;
    }

    const_iterator cbegin() const noexcept
    {
        return data_;
    }

    iterator end() noexcept
    {
        return data_ + size_;
    }

    reverse_iterator rbegin() noexcept
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return rbegin();
    }

    const_iterator end() const noexcept
    {
        return data_ + size_;
    }

    const_iterator cend() const noexcept
    {
        return end();
    }

    reverse_iterator rend() noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept
    {
        return rend();
    }

    const_pointer data() const noexcept
    {
        return data_;
    }

    pointer data() noexcept
    {
        return data_;
    }

    const_pointer c_str() const noexcept
    {
        data_[size_] = '\0';
        return data_;
    }

    constexpr size_type capacity() const noexcept
    {
        return cache_capacity;
    }

    size_type push_back(value_type value) noexcept
    {
        return append(value);
    }

    void pop_back() noexcept
    {
        if (size_)
            --size_;
    }

    size_type resize(size_type size) noexcept
    {
        assert(size < cache_size);

        if (size < cache_size)
        {
            size_ = size;
            return size;
        }

        return 0;
    }

    void reserve(size_type) noexcept
    {   }

    size_type free_size() const noexcept
    {
        return cache_capacity - size_;
    }

    size_type append(const_pointer value, size_type len) noexcept
    {
        if (len && (len <= free_size()))
        {
            assert(value);
            std::memcpy(end(), value, len);
            size_ += len;
            return size_;
        }

        return 0;
    }

    template<class T>
    size_type append(const T& other) noexcept
    {
        sv_wrap wr{to_string_view(other)};
        return append(wr.text_.data(), wr.text_.size());
    }

    size_type append(value_type value) noexcept
    {
        if (size_ < cache_capacity)
        {
            data_[size_++] = value;
            return size_;
        }
        return 0;
    }

    size_type append(size_type n, value_type value) noexcept
    {
        if (n && (n <= free_size()))
        {
            std::memset(end(), value, n);
            size_ += n;
            return size_;
        }
        return 0;
    }

    template<class T>
    size_type operator+=(const T& other) noexcept
    {
        return append(other);
    }

    void swap(basic_text& other) noexcept
    {
        basic_text t(*this);
        *this = other;
        other = t;
    }
};

template<class C, std::size_t N>
static auto sv(const basic_text<C, N>& val) noexcept
{
    return std::basic_string_view<C>(val.data(), val.size());
}

} // namespace util
} // namespace btdef

// ---- btdef::util::basic_text same type

template<class C, std::size_t N1, std::size_t N2>
bool operator==(const btdef::util::basic_text<C, N1>& lhs,
    const btdef::util::basic_text<C, N2>& rhs) noexcept
{
    using btdef::util::sv;
    return sv(lhs) == sv(rhs);
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator==(const btdef::util::basic_text<C, N>& lhs,
    const basic_other_string<C, O...>& rhs) noexcept
{
    using btdef::util::sv;
    return sv(lhs) == rhs;
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator==(const basic_other_string<C, O...>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    using btdef::util::sv;
    return lhs == sv(rhs);
}

template<class C, std::size_t N>
bool operator==(const btdef::util::basic_text<C, N>& lhs,
    std::basic_string_view<C> rhs) noexcept
{
    using btdef::util::sv;
    return sv(lhs) == rhs;
}

template<class C, std::size_t N>
bool operator==(std::basic_string_view<C> lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    using btdef::util::sv;
    return lhs == sv(rhs);
}

template<class C, std::size_t N1, std::size_t N2>
bool operator!=(const btdef::util::basic_text<C, N1>& lhs,
    const btdef::util::basic_text<C, N2>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator!=(const btdef::util::basic_text<C, N>& lhs,
    const basic_other_string<C, O...>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator!=(const basic_other_string<C, O...>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<class C, std::size_t N>
bool operator!=(const btdef::util::basic_text<C, N>& lhs,
    std::basic_string_view<C> rhs) noexcept
{
    return !(lhs == rhs);
}

template<class C, std::size_t N>
bool operator!=(std::basic_string_view<C> lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<class C, std::size_t N1, std::size_t N2>
bool operator<(const btdef::util::basic_text<C, N1>& lhs,
    const btdef::util::basic_text<C, N2>& rhs) noexcept
{
    using btdef::util::sv;
    return sv(lhs) < sv(rhs);
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator<(const btdef::util::basic_text<C, N>& lhs,
    const basic_other_string<C, O...>& rhs) noexcept
{
    using btdef::util::sv;
    return sv(lhs) < rhs;
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator<(const basic_other_string<C, O...>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    using btdef::util::sv;
    return lhs < sv(rhs);
}

template<class C, std::size_t N>
bool operator<(const btdef::util::basic_text<C, N>& lhs,
    std::basic_string_view<C> rhs) noexcept
{
    using btdef::util::sv;
    return sv(lhs) < rhs;
}

template<class C, std::size_t N>
bool operator<(std::basic_string_view<C> lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    using btdef::util::sv;
    return lhs < sv(rhs);
}

template<class C, std::size_t N1, std::size_t N2>
bool operator>(const btdef::util::basic_text<C, N1>& lhs,
    const btdef::util::basic_text<C, N2>& rhs) noexcept
{
    return rhs < lhs;
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator>(const btdef::util::basic_text<C, N>& lhs,
    const basic_other_string<C, O...>& rhs) noexcept
{
    return rhs < lhs;
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator>(const basic_other_string<C, O...>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return rhs < lhs;
}

template<class C, std::size_t N>
bool operator>(const btdef::util::basic_text<C, N>& lhs,
    std::basic_string_view<C> rhs) noexcept
{
    return rhs < lhs;
}

template<class C, std::size_t N>
bool operator>(std::basic_string_view<C> lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return rhs < lhs;
}

template<class C, std::size_t N1, std::size_t N2>
bool operator<=(const btdef::util::basic_text<C, N1>& lhs,
    const btdef::util::basic_text<C, N2>& rhs) noexcept
{
    return !(lhs > rhs);
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator<=(const btdef::util::basic_text<C, N>& lhs,
    const basic_other_string<C, O...>& rhs) noexcept
{
    return !(lhs > rhs);
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator<=(const basic_other_string<C, O...>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return !(lhs > rhs);
}

template<class C, std::size_t N>
bool operator<=(const btdef::util::basic_text<C, N>& lhs,
    std::basic_string_view<C> rhs) noexcept
{
    return !(lhs > rhs);
}

template<class C, std::size_t N>
bool operator<=(std::basic_string_view<C> lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return !(lhs > rhs);
}

template<class C, std::size_t N1, std::size_t N2>
bool operator>=(const btdef::util::basic_text<C, N1>& lhs,
    const btdef::util::basic_text<C, N2>& rhs) noexcept
{
    return !(lhs < rhs);
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator>=(const btdef::util::basic_text<C, N>& lhs,
    const basic_other_string<C, O...>& rhs) noexcept
{
    return !(lhs < rhs);
}

template<class C, std::size_t N,
         template<class...> class basic_other_string, class ...O>
bool operator>=(const basic_other_string<C, O...>& lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return !(lhs < rhs);
}

template<class C, std::size_t N>
bool operator>=(const btdef::util::basic_text<C, N>& lhs,
    std::basic_string_view<C> rhs) noexcept
{
    return !(lhs < rhs);
}

template<class C, std::size_t N>
bool operator>=(std::basic_string_view<C> lhs,
    const btdef::util::basic_text<C, N>& rhs) noexcept
{
    return !(lhs < rhs);
}


template<class C, std::size_t N, class S>
btdef::util::basic_text<C, N> operator+(const S& lhs, const btdef::util::basic_text<C, N>& rhs)
{
    btdef::util::basic_text<C, N> res;
    res += lhs;
    res += rhs;
    return res;
}

template<class C, std::size_t N, class S>
btdef::util::basic_text<C, N> operator+(const btdef::util::basic_text<C, N>& lhs, const S& rhs)
{
    btdef::util::basic_text<C, N> res;
    res += lhs;
    res += rhs;
    return res;
}

template<class C, std::size_t N, class S>
btdef::util::basic_text<C, N> operator+(std::basic_string_view<C> lhs, 
    const btdef::util::basic_text<C, N>& rhs)
{
    btdef::util::basic_text<C, N> res{lhs};
    return res += rhs;
}

template<class C, std::size_t N, class S>
btdef::util::basic_text<C, N> operator+(const btdef::util::basic_text<C, N>& lhs, 
    std::basic_string_view<C> rhs)
{
    btdef::util::basic_text<C, N> res{lhs};
    return res += rhs;
}

