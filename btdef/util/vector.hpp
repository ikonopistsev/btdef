#pragma once

#include <iterator>
#include <cstddef>
#include <cstring>
#include <cstdlib>

namespace btdef {
namespace util {

template<class T>
class vector;

template<class T, std::size_t N>
class vector_cache;

template<>
class vector<char>
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

private:
    mutable pointer data_{nullptr};
    size_type size_{0};
    size_type capacity_{0};

    enum {
        start_size = 192u,
        size_factor = 3u
    };

    size_type reserve_push_back(value_type value) noexcept
    {
        size_type size = (capacity_) ?
            capacity_ * size_factor : size_type(start_size);

        pointer ptr = static_cast<pointer>(std::realloc(data_, size));
        if (ptr)
        {
            ptr[size_++] = value;
            data_ = ptr;
            capacity_ = size - 1;
            return size_;
        }
        return 0;
    }

public:
    vector() = default;

    vector(vector&& other) noexcept
    {
        swap(other);
    }

    ~vector() noexcept
    {
        free(data_);
    }

    size_type size() const noexcept
    {
        return size_;
    }

    size_type capacity() const noexcept
    {
        return capacity_ + 1;
    }

    void clear() noexcept
    {
        size_ = 0;
    }

    pointer data() noexcept
    {
        if (data_)
            data_[size_] = '\0';
        return data_;
    }

    const_pointer data() const noexcept
    {
        if (data_)
            data_[size_] = '\0';
        return data_;
    }

    size_type push_back(value_type value) noexcept
    {
        if (size_ < capacity_)
        {
            data_[size_++] = value;
            return size_;
        }
        return reserve_push_back(value);
    }

    void swap(vector& other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }
};

} // namespace util
} // namespace btdef
