#pragma once

#include "btdef/allocator/basic.hpp"

#include <string>
#include <algorithm>
#include <functional>
#include <cstring>

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
        start_size = 192u,
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

    basic_string(const_pointer value, size_type size) noexcept
    {
        assign(value, size);
    }

    template<size_type N>
    basic_string(std::reference_wrapper<const value_type[N]> ref) noexcept
    {
        assign(ref.get(), N - 1);
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

    size_type concatenate(const_pointer value, size_type len) noexcept
    {
        assert(data_);
        if (len == 1)
            return concatenate(*value);
        std::memcpy(end(), value, len);
        size_ += len;
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
        if (n == 1)
            return concatenate(value);
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
        return swap(other);
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

    basic_string& operator=(const basic_string& other) noexcept
    {
        assign(other);
        return *this;
    }

    basic_string& operator=(basic_string&& other) noexcept
    {
        return assign(std::move(other));
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

    reference operator[](size_type i) noexcept
    {
        assert(i < size_);
        return data_[i];
    }
    const_reference operator[](size_type i) const noexcept
    {
        assert(i < size_);
        return data_[i];
    }

    reference front() noexcept
    {
        assert(size_);
        return *data_;
    }

    const_reference front() const noexcept
    {
        assert(size_);
        return *data_;
    }

    reference back() noexcept
    {
        assert(size_);
        return data_[size_ - 1];
    }

    const_reference back() const noexcept
    {
        assert(size_);
        return data_[size_ - 1];
    }

    const_pointer data() const noexcept
    {
        assert(data_);
        return data_;
    }

    pointer data() noexcept
    {
        assert(data_);
        return data_;
    }

    const_pointer c_str() const noexcept
    {
        if (data_)
        {
            data_[size_] = '\0';
            return data_;
        }

        static const value_type c = '\0';
        return &c;
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
        if (size > capacity())
            return allocate(size);

        return capacity_;
    }

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

    int compare(size_type pos, size_type count1,
        const_pointer value, size_type count2) const noexcept
    {
        assert(data_ && (pos < size_));

        const_pointer p = data_ + pos;
        if (count1 == count2)
            return std::memcmp(p, value, count1);

        int ret = std::memcmp(p, value, std::min(count1, count2));
        if (ret == 0)
            return (count1 < count2) ? -1 : 1;

        return ret;
    }

    int compare(const basic_string& other) const noexcept
    {
        if (empty())
            return other.empty() ? 0 : -1;

        return compare(0, size_, other.data(), other.size());
    }

    int compare(size_type pos, size_type count,
        const basic_string& other) const noexcept
    {
        return compare(pos, count, other.data(), other.size());
    }

    int compare(const_pointer value) const noexcept
    {
        assert(value);
        return compare(0, size_, value, std::strlen(value));
    }

    int compare(size_type pos, size_type count,
        const_pointer value) const noexcept
    {
        assert(value);
        return compare(pos, count, value, std::strlen(value));
    }

    basic_string substr(size_type pos = 0,
                        size_type count = npos)
    {
        if (data_ && (pos < size_))
        {
            count = std::min(size_ - pos, count);
            return basic_string(data_ + pos, count);
        }

        return basic_string();
    }

    void swap(basic_string& other) noexcept
    {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
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
};

typedef basic_string<char, allocator::basic<char>> string;

} // namespace util
} // namespace btdef

template<class T, class A>
bool operator==(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}

template<class T, class A>
bool operator<(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<class T, class A>
bool operator!=(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) noexcept
{
    return !(lhs == rhs);
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

/*
#pragma once

#include "btdef/allocator/wrapper.hpp"

#include <string>

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
        start_size = 192u,
        size_factor = 3u
    };

public:
    static const size_type npos = -1;

    basic_string() noexcept
        : basic_string(allocator_type())
    {   }

    explicit basic_string(const allocator_type& a) noexcept
        : allocator_(a)
    {   }

    basic_string(const basic_string& other)
        : basic_string(other, other.get_allocator())
    {

    }

    basic_string(const basic_string& other, const allocator_type& a) noexcept
    {

    }


    basic_string(const basic_string& other)
    {
        string_.assign(other.cbegin(), other.cend());
    }

    template<class T>
    basic_string(const T& str)
    {
        reserve(cache_reserve);
        string_.assign(str.cbegin(), str.cend());
    }

    basic_string(const char* s)
    {
        reserve(cache_reserve);
        string_.assign(s);
    }

    basic_string(const char* s, std::size_t n)
    {
        reserve(cache_reserve);
        string_.assign(s, n);
    }

    basic_string(std::size_t n, char c)
    {
        reserve(cache_reserve);
        string_.assign(n, c);
    }

    template<class InputIterator>
    basic_string(InputIterator first, InputIterator last)
    {
        reserve(cache_reserve);
        string_.assign(first, last);
    }

    basic_string(std::initializer_list<char> il)
    {
        reserve(cache_reserve);
        string_.assign(std::move(il));
    }

    operator std::string () const
    {
        return { cbegin(), cend() };
    }

    template<class T>
    basic_string& append(const T& str)
    {
        string_.append(str.cbegin(), str.cend());
        return *this;
    }

    basic_string& append(const char* s)
    {
        assert(s);
        string_.append(s);
        return *this;
    }

    basic_string& append(std::size_t n, char c)
    {
        string_.append(n, c);
        return *this;
    }

    template <class InputIterator>
    basic_string& append(InputIterator first, InputIterator last)
    {
        string_.append(first, last);
        return *this;
    }

    basic_string& append(std::initializer_list<char> il)
    {
        string_.append(std::move(il));
        return *this;
    }

    template<class T>
    basic_string& assign(const T& str)
    {
        string_.assign(str.cbegin(), str.cend());
        return *this;
    }

    basic_string& assign(const char* s)
    {
        assert(s);
        string_.assign(s);
        return *this;
    }

    basic_string& assign(std::size_t n, char c)
    {
        string_.assign(n, c);
        return *this;
    }

    template <class InputIterator>
    basic_string& assign(InputIterator first, InputIterator last)
    {
        string_.assign(first, last);
        return *this;
    }

    basic_string& assign(std::initializer_list<char> il)
    {
        string_.assign(std::move(il));
        return *this;
    }

    reference at(std::size_t pos)
    {
        return string_.at(pos);
    }

    const_reference at(std::size_t pos) const
    {
        return string_.at(pos);
    }

    char& back()
    {
        return string_.back();
    }

    const char& back() const
    {
        return string_.back();
    }

    iterator begin() noexcept
    {
        return string_.begin();
    }

    const_iterator begin() const noexcept
    {
        return cbegin();
    }

    std::size_t capacity() const noexcept
    {
        return string_.capacity();
    }

    const_iterator cbegin() const noexcept
    {
        return string_.cbegin();
    }

    const_iterator cend() const noexcept
    {
        return string_.cend();
    }

    void clear(bool memory = true) noexcept
    {
        string_.clear();

        if (memory)
            pool_.clear();
    }

    int compare(const char* s) const
    {
        return string_.compare(s);
    }

    int compare(std::size_t pos, std::size_t len, const char* s) const
    {
        return string_.compare(pos, len, s, size());
    }

    template<class T>
    int compare(const T& other) const noexcept
    {
        return compare(0, other.size(), other.data());
    }

    const char* c_str() const noexcept
    {
        return string_.c_str();
    }

    const char* data() const noexcept
    {
        return string_.data();
    }

    bool empty() const noexcept
    {
        return string_.empty();
    }

    iterator end()
    {
        return string_.end();
    }

    const_iterator end() const noexcept
    {
        return cend();
    }

    template<class T>
    basic_string& operator+=(const T& str)
    {
        return append(str);
    }

    basic_string& operator+=(const char* s)
    {
        return append(s);
    }

    basic_string& operator+=(char c)
    {
        return append(c);
    }

    basic_string& operator+=(std::initializer_list<char> il)
    {
        return append(std::move(il));
    }

    template<class T>
    basic_string& operator=(const T& str)
    {
        return assign(str);
    }

    basic_string& operator=(const char* s)
    {
        return assign(s);
    }

    basic_string& operator=(char c)
    {
        return assign(c);
    }

    basic_string& operator=(std::initializer_list<char> il)
    {
        return assign(std::move(il));
    }

    reference operator[] (std::size_t pos)
    {
        return string_[pos];
    }

    const_reference operator[] (std::size_t pos) const
    {
        return string_[pos];
    }

    void push_back(char c)
    {
        string_.push_back(c);
    }

    void reserve(std::size_t n)
    {
        string_.reserve(n);
    }

    std::size_t size() const noexcept
    {
        return string_.size();
    }
};

#ifndef BTDEF_UTIL_STRING_STACK_SIZE
#define BTDEF_UTIL_STRING_STACK_SIZE 512
#endif // BTDEF_UTIL_STRING_STACK_SIZE


typedef basic_string<BTDEF_UTIL_STRING_STACK_SIZE> string;

} // namespace util
} // namespace btdef

template<std::size_t N>
std::string& operator+=(std::string& lhs,
    const btdef::util::basic_string<N>& rhs)
{
    return lhs.append(rhs.begin(), rhs.end());
}

template<std::size_t N1, std::size_t N2>
btdef::util::basic_string<(N1 > N2) ? N1 : N2> operator+(
    const btdef::util::basic_string<N1>& lhs,
    const btdef::util::basic_string<N2>& rhs)
{
    btdef::util::basic_string<(N1 > N2) ? N1 : N2> result(lhs);
    result.append(rhs);
    return result;
}

template<std::size_t N>
btdef::util::basic_string<N> operator+(const btdef::util::basic_string<N>& lhs,
    const char* rhs)
{
    btdef::util::basic_string<N> result(lhs);
    result.append(rhs);
    return result;
}

template<std::size_t N>
btdef::util::basic_string<N> operator+(const btdef::util::basic_string<N>& lhs,
    char rhs)
{
    btdef::util::basic_string<N> result(lhs);
    result.append(rhs);
    return result;
}

template<class T, std::size_t N>
btdef::util::basic_string<N> operator+(const btdef::util::basic_string<N>& lhs,
    const T& rhs)
{
    btdef::util::basic_string<N> result(lhs);
    result.append(rhs);
    return result;
}

template<class C, class T, std::size_t N>
std::basic_ostream<C, T>& operator<<(std::basic_ostream<C, T>& os,
    const btdef::util::basic_string<N>& str)
{
    return os.write(str.data(), str.size());
}

template<class C, class T, class A, std::size_t N>
std::basic_string<C, T, A> operator<<(std::basic_string<C, T, A>& lhs,
    const btdef::util::basic_string<N>& str)
{
    return lhs.append(str.begin(), str.end());
}

template<std::size_t N>
bool operator==(const char* lhs, const btdef::util::basic_string<N>& rhs)
{
    return rhs.compare(lhs) == 0;
}

template<std::size_t N>
bool operator==(const btdef::util::basic_string<N>& lhs, const char* rhs)
{
    return lhs.compare(rhs) == 0;
}

template<std::size_t N>
bool operator==(const std::string& lhs, const btdef::util::basic_string<N>& rhs)
{
    return rhs.compare(lhs) == 0;
}

template<class T, std::size_t N>
bool operator==(const btdef::util::basic_string<N>& lhs, const T& rhs) noexcept
{
    return lhs.compare(rhs) == 0;
}

template<std::size_t N>
bool operator!=(const char* lhs, const btdef::util::basic_string<N>& rhs)
{
    return !(lhs == rhs);
}

template<std::size_t N>
bool operator!=(const std::string& lhs, const btdef::util::basic_string<N>& rhs)
{
    return !(rhs == lhs);
}

template<class T, std::size_t N>
bool operator!=(const btdef::util::basic_string<N>& lhs, const T& rhs) noexcept
{
    return !(lhs == rhs);
}

//---

template<class T, std::size_t N>
bool operator<(const btdef::util::basic_string<N>& lhs, const T& rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<std::size_t N>
bool operator<(const btdef::util::basic_string<N>& lhs,
    const char* rhs) noexcept
{
    return lhs.compare(rhs) < 0;
}

template<std::size_t N>
bool operator<(const char* lhs, const btdef::util::basic_string<N>& rhs)
{
    return -rhs.compare(lhs) < 0;
}

template<std::size_t N>
bool operator<(const std::string& lhs, const btdef::util::basic_string<N>& rhs)
{
    return -rhs.compare(lhs) < 0;
}

//---

template<class T, std::size_t N>
bool operator<=(const btdef::util::basic_string<N>& lhs, const T& rhs) noexcept
{
    return lhs.compare(rhs) <= 0;
}

template<std::size_t N>
bool operator<=(const btdef::util::basic_string<N>& lhs,
    const char* rhs) noexcept
{
    return lhs.compare(rhs) <= 0;
}

template<std::size_t N>
bool operator<=(const char* lhs, const btdef::util::basic_string<N>& rhs)
{
    return -rhs.compare(lhs) <= 0;
}

template<std::size_t N>
bool operator<=(const std::string& lhs, const btdef::util::basic_string<N>& rhs)
{
    return -rhs.compare(lhs) <= 0;
}

//---

template<class T, std::size_t N>
bool operator>(const btdef::util::basic_string<N>& lhs, const T& rhs) noexcept
{
    return !(lhs <= rhs);
}

template<std::size_t N>
bool operator>(const btdef::util::basic_string<N>& lhs,
    const char* rhs) noexcept
{
    return !(lhs <= rhs);
}

template<std::size_t N>
bool operator>(const char* lhs, const btdef::util::basic_string<N>& rhs)
{
    return !(lhs <= rhs);
}

template<std::size_t N>
bool operator>(const std::string& lhs, const btdef::util::basic_string<N>& rhs)
{
    return !(lhs <= rhs);
}

//---

template<class T, std::size_t N>
bool operator>=(const btdef::util::basic_string<N>& lhs, const T& rhs) noexcept
{
    return !(lhs < rhs);
}

template<std::size_t N>
bool operator>=(const btdef::util::basic_string<N>& lhs,
    const char* rhs) noexcept
{
    return !(lhs < rhs);
}

template<std::size_t N>
bool operator>=(const char* lhs, const btdef::util::basic_string<N>& rhs)
{
    return !(lhs < rhs);
}

template<std::size_t N>
bool operator>=(const std::string& lhs, const btdef::util::basic_string<N>& rhs)
{
    return !(lhs < rhs);
}

*/
