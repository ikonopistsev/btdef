#pragma once

#include <cassert>
#include <cstddef>
#include <cstring>

#include <iterator>
#include <algorithm>
#include <functional>

namespace btdef {
namespace util {

template<class C, std::size_t L>
struct basic_text;

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
        cache_size = L * sizeof(std::uint64_t),
        cache_capacity = cache_size - 1
    };

private:
    mutable value_type data_[cache_size];
    size_type size_{0};

public:
    basic_text() = default;

    basic_text(const_pointer value, size_type len) noexcept
    {
        assign(value, len);
    }

    basic_text(size_type len, value_type value) noexcept
    {
        assign(len, value);
    }

    basic_text(const basic_text& other) noexcept
    {
        assign(other.data(), other.size());
    }

    basic_text(const_pointer value) noexcept
    {
        assign(value);
    }

    template<size_type N>
    basic_text(std::reference_wrapper<const value_type[N]> ref) noexcept
    {
        assign(ref.get(), N - 1);
    }

    template<class T>
    basic_text(const T& other) noexcept
    {
        assign(other.data(), other.size());
    }

    size_type assign(const_pointer value, size_type len) noexcept
    {
        assert(value);
        if (len < cache_size)
        {
            size_ = len;
            if (len)
            {
                if (len == 1)
                    *data_ = *value;
                else
                    std::memcpy(data_, value, len);
            }
            return len;
        }
        return 0;
    }

    size_type assign(const_pointer value) noexcept
    {
        assert(value);
        return assign(value, std::strlen(value));
    }

    template<class T>
    size_type assign(const T& other) noexcept
    {
        return assign(other.begin(), other.size());
    }

    template<value_type N>
    size_type assign(std::reference_wrapper<const value_type[N]> ref) noexcept
    {
        return assign(ref.get(), N - 1);
    }

    size_type assign(size_type n, char value) noexcept
    {
        if (n < cache_size)
        {
            size_ = n;
            if (n)
            {
                if (n == 1)
                    *data_ = value;
                else
                    std::memset(data_, value, n);
            }
            return size_;
        }
        return 0;
    }

    size_type operator=(const basic_text& other) noexcept
    {
        return assign(other.data(), other.size());
    }

    size_type operator=(const_pointer value) noexcept
    {
        return assign(value);
    }

    template<class T>
    size_type operator=(const T& other)
    {
        return assign(other.data(), other.size());
    }

    template<std::size_t N>
    size_type operator=(std::reference_wrapper<const value_type[N]> r) noexcept
    {
        return assign(r.get(), N - 1);
    }

    int compare(const_pointer value, size_type len) const noexcept
    {
        int ret = std::memcmp(data_, value, (std::min)(size_, len));
        if (ret)
            return ret;

        return (size_ < len) ? -1 : 1;
    }

    int compare(size_type pos, size_type count1,
        const_pointer value, size_type count2) const noexcept
    {
        if (pos < size_)
            count1 = (std::min)(size_ - pos, count1);
        else
            count1 = 0;

        int ret = std::memcmp(data_ + pos, value, (std::min)(count1, count2));
        if (ret)
            return ret;

        return (count1 < count2) ? -1 : 1;
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

    template<class T>
    bool starts_with(const T& other) const noexcept
    {
        return (size_ >= other.size()) &&
            (std::memcmp(data_, other.data(), other.size()) == 0);
    }

    template<class T>
    bool ends_with(const T& other) const noexcept
    {
        size_type sz = other.size();
        return (size_ >= sz) &&
           (std::memcmp(data_ + (size_ - sz), other.data(), sz) == 0);
    }

    reference operator[](size_type i) noexcept
    {
        return data_[i];
    }

    const_reference operator[](size_type i) const noexcept
    {
        return data_[i];
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

    size_type resize(size_type len) noexcept
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

    size_type free_size() const noexcept
    {
        return cache_capacity - size_;
    }

    void increase(size_type size) noexcept
    {
        assert(size < free_size());
        size_ = size;
    }

    size_type append(const_pointer value, size_type len) noexcept
    {
        assert(value);

        if (len <= free_size())
        {
            if (len)
            {
                if (len == 1)
                {
                    data_[size_++] = value;
                }
                else
                {
                    std::memcpy(end(), value, len);
                    size_ += len;
                }
            }
            return size_;
        }
        return 0;
    }

    size_type append(const basic_text& other) noexcept
    {
        return append(other.begin(), other.size());
    }

    template<class T>
    size_type append(const T& other) noexcept
    {
        return append(other.begin(), other.size());
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
        if (n <= free_size())
        {
            if (n)
            {
                if (n == 1)
                {
                    data_[size_++] = value;
                }
                else
                {
                    std::memset(end(), value, n);
                    size_ += n;
                }
            }
            return size_;
        }
        return 0;
    }

    size_type append(const_pointer value) noexcept
    {
        return append(value, std::strlen(value));
    }

    template<size_type N>
    size_type append(std::reference_wrapper<const value_type[N]> ref) noexcept
    {
        return append(ref.get(), N - 1);
    }

    size_type operator+=(const basic_text& other) noexcept
    {
        return append(other.data(), other.size());
    }

    template<class T>
    size_type operator+=(const T& other) noexcept
    {
        return append(other.data(), other.size());
    }

    size_type operator+=(value_type value) noexcept
    {
        return append(value);
    }

    size_type operator+=(const_pointer value) noexcept
    {
        return append(value);
    }

    template<size_type N>
    size_type operator+=(std::reference_wrapper<const value_type[N]> r) noexcept
    {
        return append(r.get(), N - 1);
    }

    void swap(basic_text& other) noexcept
    {
        basic_text t(*this);
        *this = other;
        other = t;
    }
};

} // namespace util
} // namespace btdef

template<std::size_t N>
bool operator==(const btdef::util::basic_text<char, N>& lhs,
    const btdef::util::basic_text<char, N>& rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}

template<std::size_t N>
bool operator<(const btdef::util::basic_text<char, N>& lhs,
    const btdef::util::basic_text<char, N>& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<std::size_t N>
bool operator!=(const btdef::util::basic_text<char, N>& lhs,
    const btdef::util::basic_text<char, N>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<std::size_t N>
bool operator<=(const btdef::util::basic_text<char, N>& lhs,
    const btdef::util::basic_text<char, N>& rhs) noexcept
{
    return !(rhs < lhs);
}

template<std::size_t N>
bool operator>=(const btdef::util::basic_text<char, N>& lhs,
    const btdef::util::basic_text<char, N>& rhs) noexcept
{
    return !(lhs < rhs);
}

template<class C, class T, std::size_t N>
std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& os,
    const btdef::util::basic_text<C, N>& rhs)
{
    return os.write(rhs.data(), rhs.size());
}

/*
template<std::size_t N>
std::string& operator+=(std::string& lhs,
    const btdef::util::basic_text<char, N>& rhs)
{
    return lhs.append(rhs.data(), rhs.size());
}
*/