#pragma once

#include <iterator>
#include <cstddef>
#include <cstring>

namespace btdef {
namespace util {

template<class C, std::size_t L>
struct text;

template<std::size_t L>
class text<char, L>
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
    text() = default;

    text(const text& other) noexcept
        : text(other.begin(), other.size())
    {   }

    template<size_type N>
    text(const text<value_type, N>& other) noexcept
        : text(other.begin(), other.size())
    {   }

    text(const_pointer value, size_type len) noexcept
    {
        assign(value, len);
    }

    text(const_pointer value) noexcept
    {
        assign(value);
    }

    text(size_type len, value_type value) noexcept
    {
        assign(len, value);
    }

    template<size_type N>
    text(std::reference_wrapper<const value_type[N]> ref) noexcept
        : text(ref.get(), N - 1)
    {   }

    size_type assign(const text& other) noexcept
    {
        return assign(other.data(), other.size());
    }

    template<value_type N>
    size_type assign(const text<value_type, N>& other) noexcept
    {
        return assign(other.begin(), other.size());
    }

    size_type assign(const_pointer value, size_type len) noexcept
    {
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
        return assign(value, std::strlen(value));
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

    template<value_type N>
    size_type assign(std::reference_wrapper<const value_type[N]> ref) noexcept
    {
        return assign(ref.get(), N - 1);
    }

    size_type operator=(const text& other) noexcept
    {
        return assign(other);
    }

    template<size_type N>
    size_type operator=(const text<value_type, N>& other) noexcept
    {
        return assign(other);
    }

    size_type operator=(const_pointer value) noexcept
    {
        return assign(value);
    }

    template<std::size_t N>
    size_type operator=(std::reference_wrapper<const value_type[N]> r) noexcept
    {
        return assign(r.get(), N - 1);
    }

    long compare(const text& other) const noexcept
    {
        return compare(other.begin(), other.size());
    }

    template<std::size_t N>
    long compare(const text<value_type, N>& other) const noexcept
    {
        return compare(other.begin(), other.size());
    }

    long compare(const_pointer other, size_type len) const noexcept
    {
        size_type sz = size();
        if (sz == len)
            return std::memcmp(begin(), other, sz);

        int ret = std::memcmp(begin(), other, std::min(sz, len));
        if (ret == 0)
            return (sz < len) ? -1 : 1;

        return ret;
    }

    long compare(const_pointer value) const noexcept
    {
        return compare(value, std::strlen(value));
    }

    template<std::size_t N>
    long compare(std::reference_wrapper<const value_type[N]> r) const noexcept
    {
        return compare(r.get(), N - 1);
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
        data_[size_] = '\0';
        return data_;
    }

    pointer data() noexcept
    {
        data_[size_] = '\0';
        return data_;
    }

    const_pointer c_str() const noexcept
    {
        return data();
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

    size_type append(const text& other) noexcept
    {
        return append(other.begin(), other.size());
    }

    template<size_type N>
    size_type append(const text<value_type, N>& other) noexcept
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

    size_type append(const_pointer value, size_type len) noexcept
    {
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

    size_type append(const_pointer value) noexcept
    {
        return append(value, std::strlen(value));
    }

    template<size_type N>
    size_type append(std::reference_wrapper<const value_type[N]> ref) noexcept
    {
        return append(ref.get(), N - 1);
    }

    size_type operator+=(const text& other) noexcept
    {
        return append(other);
    }

    template<size_type N>
    size_type operator+=(const text<value_type, N>& other) noexcept
    {
        return append(other);
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

    void swap(text& other) noexcept
    {
        text t(*this);
        *this = other;
        other = t;
    }
};

} // namespace util
} // namespace btdef

/*
    bool operator==(const vector_cache& other) const noexcept
    {
        return !compare(other);
    }

    bool operator==(const char *value) const noexcept
    {
        return !compare(value);
    }

    template<std::size_t L>
    bool operator==(std::reference_wrapper<const char[L]> ref) const noexcept
    {
        return !compare(std::move(ref));
    }

    template<std::size_t L>
    bool operator==(const vector_cache<char, L>& other) const noexcept
    {
        return !compare(other);
    }

    bool operator!=(const vector_cache& other) const noexcept
    {
        return !(*this == other);
    }

    bool operator!=(const char *value) const noexcept
    {
        return !(*this == value);
    }

    template<std::size_t L>
    bool operator!=(std::reference_wrapper<const char[L]> ref) const noexcept
    {
        return !(*this == std::move(ref));
    }

    template<std::size_t L>
    bool operator!=(const vector_cache<char, L>& other) const noexcept
    {
        return !(*this == other);
    }

    bool operator<(const vector_cache& other) const noexcept
    {
        return compare(other) < 0;
    }

    bool operator<(const char *value) const noexcept
    {
        return compare(value) < 0;
    }

    template<std::size_t L>
    bool operator<(std::reference_wrapper<const char[L]> ref) const noexcept
    {
        return compare(std::move(ref)) < 0;
    }

    template<std::size_t L>
    bool operator<(const vector_cache<char, L>& other) const noexcept
    {
        return compare(other) < 0;
    }

    bool operator>(const vector_cache& other) const noexcept
    {
        return other < *this;
    }

    bool operator>(const char *value) const noexcept
    {
        return compare(value) > 0;
    }

    template<std::size_t L>
    bool operator>(std::reference_wrapper<const char[L]> ref) const noexcept
    {
        return compare(std::move(ref)) > 0;
    }

    template<std::size_t L>
    bool operator>(const vector_cache<char, L>& other) const noexcept
    {
        return other < *this;
    }

    bool operator<=(const vector_cache& other) const noexcept
    {
        return !(other < *this);
    }

    bool operator<=(const char *value) const noexcept
    {
        return !(compare(value) > 0);
    }

    template<std::size_t L>
    bool operator<=(std::reference_wrapper<const char[L]> ref) const noexcept
    {
        return !(compare(std::move(ref)) > 0);
    }

    template<std::size_t L>
    bool operator<=(const vector_cache<char, L>& other) const noexcept
    {
        return !(other < *this);
    }

    bool operator>=(const vector_cache& other) const noexcept
    {
        return !(*this < other);
    }

    bool operator>=(const char *value) const noexcept
    {
        return !(*this < value);
    }

    template<std::size_t L>
    bool operator>=(std::reference_wrapper<const char[L]> ref) const noexcept
    {
        return !(*this < std::move(ref));
    }

    template<std::size_t L>
    bool operator>=(const vector_cache<char, L>& other) const noexcept
    {
        return !(*this < other);
    }
*/

/*

*/

template<std::size_t N>
std::string& operator+=(std::string& lhs,
    const btdef::util::text<char, N>& rhs)
{
    return lhs.append(rhs.data(), rhs.size());
}

template<class C, class T, std::size_t N>
std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& os,
    const btdef::util::text<C, N>& rhs)
{
    return os.write(rhs.data(), rhs.size());
}
