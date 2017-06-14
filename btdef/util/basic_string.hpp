#pragma once

#include "btdef/util/text.hpp"

#include <stdexcept>

namespace btdef {
namespace util {

template<class T, class A>
class basic_string;

template<class A>
class basic_string<char, A>
{
public:
    typedef A allocator_type;
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
    allocator_type allocator_{};

    enum {
        start_size = 320u,
        size_factor = 3u
    };

public:
    static const size_type npos = -1;

    basic_string() = default;

    explicit basic_string(const allocator_type& a) noexcept
        : allocator_(a)
    {   }

    basic_string(const basic_string& other) noexcept
        : allocator_(other.ocator())
    {
        assign(other);
    }

    basic_string(const basic_string& other, const allocator_type& a) noexcept
        : allocator_(a)
    {
        assign(other);
    }

    basic_string(basic_string&& other) noexcept
    {
        swap(other);
    }

    basic_string(const_pointer value) noexcept
    {
        assign(value);
    }

    basic_string(size_type size, value_type value) noexcept
    {
        assign(size, value);
    }

    basic_string(const_pointer value, size_type size) noexcept
    {
        assign(value, size);
    }

    template<size_type N>
    basic_string(std::reference_wrapper<const value_type[N]> ref) noexcept
    {
        assign(ref.get(), N - 1);
    }

    template<class T>
    basic_string(const T& other) noexcept
    {
        assign(other.data(), other.size());
    }

    ~basic_string() noexcept
    {
        allocator_.deallocate(data_, capacity_);
    }

    allocator_type get_allocator() const noexcept
    {
        return allocator_;
    }

    size_type free_size() const noexcept
    {
        return capacity_ - size_;
    }

    void increase(size_type size) noexcept
    {
        assert(data_ && (size < free_size()));
        // в проверках особо смысла нет
        // тк предполагается что впечатывали в буфер через memcpy(data(), ...)
        size_ = size;
    }

    size_type concatenate(const_pointer value, size_type len) noexcept
    {
        if (len)
        {
            assert(data_ && value);
            std::memcpy(end(), value, len);
            size_ += len;
        }
        return size_;
    }

    size_type concatenate(value_type value) noexcept
    {
        assert(data_);
        data_[size_++] = value;
        return size_;
    }

    size_type concatenate(size_type n, value_type value) noexcept
    {
        assert(data_);
        std::memset(end(), value, n);
        size_ += n;
        return size_;
    }

    size_type allocate_concatenate(const_pointer value, size_type len) noexcept
    {
        size_type new_capacity = (capacity_) ?
            capacity_ * size_factor : size_type(start_size);

        if (new_capacity <= len)
            new_capacity = len * size_factor;

        pointer ptr = allocator_.allocate(new_capacity, data_);
        if (ptr)
        {
            data_ = ptr;
            capacity_ = new_capacity;
            return concatenate(value, len);
        }
        return 0;
    }

    size_type allocate_concatenate(value_type value) noexcept
    {
        size_type new_capacity = (capacity_) ?
            capacity_ * size_factor : size_type(start_size);

        pointer ptr = allocator_.allocate(new_capacity, data_);
        if (ptr)
        {
            data_ = ptr;
            capacity_ = new_capacity;
            return concatenate(value);
        }
        return 0;
    }

    size_type allocate_concatenate(size_type n, value_type value) noexcept
    {
        size_type new_capacity = (capacity_) ?
            capacity_ * size_factor : size_type(start_size);

        if (new_capacity <= n)
            new_capacity = n * size_factor;

        pointer ptr = allocator_.allocate(new_capacity, data_);
        if (ptr)
        {
            data_ = ptr;
            capacity_ = new_capacity;
            return concatenate(n, value);
        }
        return 0;
    }

    size_type allocate(size_type len) noexcept
    {
        size_type new_capacity = (capacity_) ?
            capacity_ * size_factor : size_type(start_size);

        if (new_capacity <= len)
            new_capacity = len * size_factor;

        pointer ptr = allocator_.allocate(new_capacity, data_);
        if (ptr)
        {
            data_ = ptr;
            capacity_ = new_capacity;
            return new_capacity;
        }
        return 0;
    }

    size_type assign(const basic_string& other) noexcept
    {
        return assign(other.begin(), other.size());
    }

    basic_string& assign(basic_string&& other) noexcept
    {
        swap(other);
        return *this;
    }

    size_type assign(value_type value) noexcept
    {
        size_ = 0;
        return append(value);
    }

    size_type assign(size_type n, value_type value) noexcept
    {
        size_ = 0;
        return append(n, value);
    }

    size_type assign(const_pointer value) noexcept
    {
        size_ = 0;
        return append(value, std::strlen(value));
    }

    size_type assign(const_pointer value, size_type len) noexcept
    {
        size_ = 0;
        return append(value, len);
    }

    template<size_type N>
    size_type assign(std::reference_wrapper<const value_type[N]> ref) noexcept
    {
        size_ = 0;
        return append(ref.get(), N - 1);
    }

    template<class T>
    size_type assign(const T& other) noexcept
    {
        size_ = 0;
        return append(other.data(), other.size());
    }

    basic_string& operator=(const basic_string& other) noexcept
    {
        assign(other);
        return *this;
    }

    basic_string& operator=(basic_string&& other) noexcept
    {
        assign(std::move(other));
        return *this;
    }

    basic_string& operator=(const_pointer value) noexcept
    {
        assign(value);
        return *this;
    }

    basic_string& operator=(value_type value) noexcept
    {
        assign(value);
        return *this;
    }

    template<std::size_t N>
    basic_string& operator=(std::reference_wrapper<const value_type[N]> r) noexcept
    {
        assign(r.get(), N - 1);
        return *this;
    }

    template<class T>
    basic_string& operator=(const T& other) noexcept
    {
        assign(other.data(), other.size());
        return *this;
    }

    reference operator[](size_type i) noexcept
    {
        assert(i < size_);
        return data_[i];
    }
    const_reference operator[](size_type i) const noexcept
    {
        assert(data_);
        assert(i < size_);
        return data_[i];
    }

    reference front() noexcept
    {
        assert(data_);
        return *data_;
    }

    const_reference front() const noexcept
    {
        assert(data_);
        return *data_;
    }

    reference back() noexcept
    {
        assert(data_ && size_);
        return data_[size_ - 1];
    }

    const_reference back() const noexcept
    {
        assert(data_ && size_);
        return data_[size_ - 1];
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
        if (data_)
        {
            data_[size_] = '\0';
            return data_;
        }

        static const value_type empty[] = "";
        return empty;
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
        return begin();
    }

    iterator end() noexcept
    {
        return data_ + size_;
    }

    const_iterator end() const noexcept
    {
        return data_ + size_;
    }

    const_iterator cend() const noexcept
    {
        return end();
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

    bool empty() const noexcept
    {
        return size_ == 0;
    }

    size_type size() const noexcept
    {
        return size_;
    }

    size_type reserve(size_type size) noexcept
    {
        if (size >= capacity_)
            return allocate(size);

        return capacity_;
    }

    // интерфейсный метод возвращает -1 элеметнт под '\0'
    size_type capacity() const noexcept
    {
        return capacity_ - 1;
    }

    void clear() noexcept
    {
        size_ = 0;
    }

    size_type insert(size_type index, size_type count, value_type value);
    size_type insert(size_type index, const_pointer value);
    size_type insert(size_type index, const basic_string& other);
    size_type insert(size_type index, const_pointer value, size_type count);

    size_type append(value_type value) noexcept
    {
        if (size_ < capacity_)
            return concatenate(value);

        return allocate_concatenate(value);
    }

    size_type append(size_type n, value_type value) noexcept
    {
        if (n < free_size())
            return concatenate(n, value);

        return allocate_concatenate(n, value);
    }

    size_type append(const_pointer value, size_type len) noexcept
    {
        if (len < free_size())
            return concatenate(value, len);

        return allocate_concatenate(value, len);
    }

    template<size_type N>
    size_type append(std::reference_wrapper<const value_type[N]> ref) noexcept
    {
        return append(ref.get(), N - 1);
    }

    template<class T>
    size_type append(const T& other) noexcept
    {
        return append(other.data(), other.size());
    }

    size_type operator+=(const basic_string& other) noexcept
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
    size_type operator+=(std::reference_wrapper<const value_type[N]> ref) noexcept
    {
        return append(ref.get(), N - 1);
    }

    template<class T>
    size_type operator+=(const T& other) noexcept
    {
        return append(other.data(), other.size());
    }

    int compare(const_pointer value, size_type len) const noexcept
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
        const_pointer value, size_type count2) const noexcept
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

    bool operator==(const basic_string& rhs) const noexcept
    {
        return compare(rhs) == 0;
    }

    bool operator<(const basic_string& rhs) const noexcept
    {
        return compare(rhs) < 0;
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

    basic_string substr(size_type pos = 0,
                        size_type count = npos)
    {
        if (data_ && (pos < size_))
        {
            count = (std::min)(size_ - pos, count);
            return basic_string(data_ + pos, count);
        }

        return basic_string();
    }

    void swap(basic_string& other) noexcept
    {
        std::swap(data_, other.data_),
        std::swap(size_, other.size_),
        std::swap(capacity_, other.capacity_),
        std::swap(allocator_, other.allocator_);
    }

    size_type find(const_pointer value,
        size_type pos, size_type count) const noexcept
    {
        auto it = std::search(cbegin(), cend(),
            value + pos, value + pos + count);
        return it == cend() ? npos : std::distance(cbegin(), it);
    }

    size_type find(const basic_string& other, size_type pos = 0) const noexcept
    {
        // FIXME TEST
        return find(other.data(), pos, other.size() - pos);
    }

    size_type find(const_pointer value, size_type pos = 0) const noexcept
    {
        return find(value, pos, std::strlen(value) - pos);
    }

    size_type find(value_type value, size_type pos = 0) const noexcept
    {
        return find(&value, pos, 1);
    }

    static inline void runtime_error(size_type result, const char *text)
    {
        if (!result)
            throw std::runtime_error(text);
    }
};

typedef basic_string<char, allocator::basic<char>> string;

} // namespace util
} // namespace btdef

// ---- btdef::util::basic_string same type

template<class C, class A1, class A2>
bool operator==(const btdef::util::basic_string<C, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}

template<class T, class A>
bool operator!=(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) noexcept
{
    return !(lhs == rhs);
}

template<class T, class A>
bool operator<(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<class T, class A>
bool operator>(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) noexcept
{
    return rhs < lhs;
}

template<class T, class A>
bool operator<=(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) noexcept
{
    return !(rhs < lhs);
}

template<class T, class A>
bool operator>=(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) noexcept
{
    return !(lhs < rhs);
}

// ---- btdef::util::basic_string same type
// ---- btdef::util::basic_string other type

template<class T, class A, class F>
bool operator==(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}

template<class T, class A, class F>
bool operator!=(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) noexcept
{
    return !(lhs == rhs);
}

template<class T, class A, class F>
bool operator<(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<class T, class A, class F>
bool operator>(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) noexcept
{
    return lhs.compare(rhs) > 0;
}

template<class T, class A, class F>
bool operator<=(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) noexcept
{
    return !(rhs < lhs);
}

template<class T, class A, class F>
bool operator>=(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) noexcept
{
    return !(lhs < rhs);
}

// ---- btdef::util::basic_string other type
