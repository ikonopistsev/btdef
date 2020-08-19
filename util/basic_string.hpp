#pragma once

#include "btdef/util/text.hpp"
#include "btdef/allocator/basic.hpp"

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
    };

    static constexpr double size_factor = 1.61803;

    class bad_alloc
        : public std::bad_alloc
    {
        text what_;

    public:
        template<std::size_t N>
        explicit bad_alloc(const char (&text)[N]) noexcept
            : what_(text, N - 1)
        {
            what_ += std::cref(": bad alloc");
        }

        virtual const char* what() const noexcept override
        {
            return what_.c_str();
        }
    };

public:
    static const size_type npos = static_cast<size_type>(-1);

    basic_string() = default;

    explicit basic_string(const allocator_type& a) BTDEF_NOEXCEPT
        : allocator_(a)
    {   }

    basic_string(const basic_string& other) BTDEF_NOEXCEPT
        : allocator_(other.get_allocator())
    {
        assign(other);
    }

    basic_string(const basic_string& other,
        const allocator_type& a) BTDEF_NOEXCEPT
        : allocator_(a)
    {
        assign(other);
    }

    basic_string(basic_string&& other) BTDEF_NOEXCEPT
    {
        swap(other);
    }

    basic_string(const_pointer value) BTDEF_NOEXCEPT
    {
        assign(value);
    }

    basic_string(size_type size, value_type value) BTDEF_NOEXCEPT
    {
        assign(size, value);
    }

    basic_string(const_pointer value, size_type size) BTDEF_NOEXCEPT
    {
        assign(value, size);
    }

    template<size_type N>
    basic_string(std::reference_wrapper<const value_type[N]> ref)
        BTDEF_NOEXCEPT
    {
        assign(ref.get(), N - 1);
    }

    template<class T>
    basic_string(const T& other) BTDEF_NOEXCEPT
    {
        assign(other.data(), other.size());
    }

    ~basic_string() BTDEF_NOEXCEPT
    {
        allocator_.deallocate(data_, capacity_);
    }

    allocator_type get_allocator() const BTDEF_NOEXCEPT
    {
        return allocator_;
    }

    size_type free_size() const BTDEF_NOEXCEPT
    {
        return capacity_ - size_;
    }

    void increase(size_type size) BTDEF_NOEXCEPT
    {
        assert(data_ && (size < free_size()));
        // в проверках особо смысла нет
        // тк предполагается что впечатывали в буфер через memcpy(data(), ...)
        size_ = size;
    }

    size_type concatenate(const_pointer value, size_type len) BTDEF_NOEXCEPT
    {
        if (len)
        {
            assert(data_ && value);
            std::memcpy(end(), value, len);
            size_ += len;
        }
        return size_;
    }

    size_type concatenate(value_type value) BTDEF_NOEXCEPT
    {
        assert(data_);
        data_[size_++] = value;
        return size_;
    }

    size_type concatenate(size_type n, value_type value) BTDEF_NOEXCEPT
    {
        assert(data_);
        std::memset(end(), value, n);
        size_ += n;
        return size_;
    }

    size_type allocate_concatenate(const_pointer value, size_type len)
        BTDEF_NOEXCEPT
    {
        size_type new_capacity = (capacity_) ?
            static_cast<size_type>(capacity_ * size_factor)
                  : size_type(start_size);

        size_type need_size = size() + len + 1;
        if (new_capacity <= need_size)
            new_capacity = need_size;

        pointer ptr = allocator_.allocate(new_capacity, data_);
        if (ptr)
        {
            data_ = ptr;
            capacity_ = new_capacity;
            return concatenate(value, len);
        }
        return 0;
    }

    size_type allocate_concatenate(value_type value) BTDEF_NOEXCEPT
    {
        size_type new_capacity = (capacity_) ?
            static_cast<size_type>(capacity_ * size_factor)
                  : size_type(start_size);

        size_type need_size = size() + 2;
        if (new_capacity <= need_size)
            new_capacity = need_size;

        pointer ptr = allocator_.allocate(new_capacity, data_);
        if (ptr)
        {
            data_ = ptr;
            capacity_ = new_capacity;
            return concatenate(value);
        }
        return 0;
    }

    size_type allocate_concatenate(size_type n,
        value_type value) BTDEF_NOEXCEPT
    {
        size_type new_capacity = (capacity_) ?
            static_cast<size_type>(capacity_ * size_factor)
                  : size_type(start_size);

        size_type need_size = size() + n + 1;
        if (new_capacity <= need_size)
            new_capacity = need_size;

        pointer ptr = allocator_.allocate(new_capacity, data_);
        if (ptr)
        {
            data_ = ptr;
            capacity_ = new_capacity;
            return concatenate(n, value);
        }
        return 0;
    }

    size_type allocate(size_type len) BTDEF_NOEXCEPT
    {
        size_type new_capacity = (capacity_) ?
            static_cast<size_type>(capacity_ * size_factor)
                  : size_type(start_size);

        if (new_capacity <= len)
            new_capacity = len;

        pointer ptr = allocator_.allocate(new_capacity, data_);
        if (ptr)
        {
            data_ = ptr;
            capacity_ = new_capacity;
            return new_capacity;
        }
        return 0;
    }

    size_type assign(const basic_string& other) BTDEF_NOEXCEPT
    {
        return assign(other.begin(), other.size());
    }

    basic_string& assign(basic_string&& other) BTDEF_NOEXCEPT
    {
        swap(other);
        return *this;
    }

    size_type assign(value_type value) BTDEF_NOEXCEPT
    {
        size_ = 0;
        return append(value);
    }

    size_type assign(size_type n, value_type value) BTDEF_NOEXCEPT
    {
        size_ = 0;
        return append(n, value);
    }

    size_type assign(const_pointer value) BTDEF_NOEXCEPT
    {
        size_ = 0;
        return append(value, std::strlen(value));
    }

    size_type assign(const_pointer value, size_type len) BTDEF_NOEXCEPT
    {
        size_ = 0;
        return append(value, len);
    }

    template<size_type N>
    size_type assign(std::reference_wrapper<const value_type[N]> ref)
        BTDEF_NOEXCEPT
    {
        size_ = 0;
        return append(ref.get(), N - 1);
    }

    template<class T>
    size_type assign(const T& other) BTDEF_NOEXCEPT
    {
        size_ = 0;
        return append(other.data(), other.size());
    }

    basic_string& operator=(const basic_string& other) BTDEF_NOEXCEPT
    {
        assign(other);
        return *this;
    }

    basic_string& operator=(basic_string&& other) BTDEF_NOEXCEPT
    {
        assign(std::move(other));
        return *this;
    }

    basic_string& operator=(const_pointer value) BTDEF_NOEXCEPT
    {
        assign(value);
        return *this;
    }

    basic_string& operator=(value_type value) BTDEF_NOEXCEPT
    {
        assign(value);
        return *this;
    }

    template<std::size_t N>
    basic_string& operator=(
        std::reference_wrapper<const value_type[N]> r) BTDEF_NOEXCEPT
    {
        assign(r.get(), N - 1);
        return *this;
    }

    template<class T>
    basic_string& operator=(const T& other) BTDEF_NOEXCEPT
    {
        assign(other.data(), other.size());
        return *this;
    }

    reference operator[](size_type i) BTDEF_NOEXCEPT
    {
        assert(i < size_);
        return data_[i];
    }
    const_reference operator[](size_type i) const BTDEF_NOEXCEPT
    {
        assert(data_);
        assert(i < size_);
        return data_[i];
    }

    reference front() BTDEF_NOEXCEPT
    {
        assert(data_);
        return *data_;
    }

    const_reference front() const BTDEF_NOEXCEPT
    {
        assert(data_);
        return *data_;
    }

    reference back() BTDEF_NOEXCEPT
    {
        assert(data_ && size_);
        return data_[size_ - 1];
    }

    const_reference back() const BTDEF_NOEXCEPT
    {
        assert(data_ && size_);
        return data_[size_ - 1];
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
        if (data_)
        {
            data_[size_] = '\0';
            return data_;
        }

        static const value_type empty[] = "";
        return empty;
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
        return begin();
    }

    iterator end() BTDEF_NOEXCEPT
    {
        return data_ + size_;
    }

    const_iterator end() const BTDEF_NOEXCEPT
    {
        return data_ + size_;
    }

    const_iterator cend() const BTDEF_NOEXCEPT
    {
        return end();
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

    bool empty() const BTDEF_NOEXCEPT
    {
        return size_ == 0;
    }

    size_type size() const BTDEF_NOEXCEPT
    {
        return size_;
    }

    size_type reserve(size_type size) BTDEF_NOEXCEPT
    {
        if (size >= capacity_)
            return allocate(size);

        return capacity_;
    }

    void reserve_ex(size_type size)
    {
        if (reserve(size) < size)
            throw bad_alloc("reserve_ex");
    }

    // интерфейсный метод возвращает -1 элеметнт под '\0'
    size_type capacity() const BTDEF_NOEXCEPT
    {
        return capacity_ - 1;
    }

    void clear() BTDEF_NOEXCEPT
    {
        size_ = 0;
    }

    size_type insert(size_type index, size_type count, value_type value);
    size_type insert(size_type index, const_pointer value);
    size_type insert(size_type index, const basic_string& other);
    size_type insert(size_type index, const_pointer value, size_type count);

    size_type append(value_type value) BTDEF_NOEXCEPT
    {
        if (size_ < capacity_)
            return concatenate(value);

        return allocate_concatenate(value);
    }

    size_type append(size_type n, value_type value) BTDEF_NOEXCEPT
    {
        if (n < free_size())
            return concatenate(n, value);

        return allocate_concatenate(n, value);
    }

    size_type append(const_pointer value, size_type len) BTDEF_NOEXCEPT
    {
        if (len < free_size())
            return concatenate(value, len);

        return allocate_concatenate(value, len);
    }

    template<size_type N>
    size_type append(std::reference_wrapper<const value_type[N]> ref)
        BTDEF_NOEXCEPT
    {
        return append(ref.get(), N - 1);
    }

    template<class T>
    size_type append(const T& other) BTDEF_NOEXCEPT
    {
        return append(other.data(), other.size());
    }

    size_type append(const_pointer value) BTDEF_NOEXCEPT
    {
        assert(value);
        return append(value, std::strlen(value));
    }

    size_type operator+=(const basic_string& other) BTDEF_NOEXCEPT
    {
        return append(other);
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
    size_type operator+=(
        std::reference_wrapper<const value_type[N]> ref) BTDEF_NOEXCEPT
    {
        return append(ref.get(), N - 1);
    }

    template<class T>
    size_type operator+=(const T& other) BTDEF_NOEXCEPT
    {
        return append(other.data(), other.size());
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
    int compare(size_type pos, size_type len, const T& other) const
        BTDEF_NOEXCEPT
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

    bool operator==(const basic_string& rhs) const BTDEF_NOEXCEPT
    {
        return compare(rhs) == 0;
    }

    bool operator<(const basic_string& rhs) const BTDEF_NOEXCEPT
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

    void swap(basic_string& other) BTDEF_NOEXCEPT
    {
        std::swap(data_, other.data_),
        std::swap(size_, other.size_),
        std::swap(capacity_, other.capacity_),
        std::swap(allocator_, other.allocator_);
    }

    size_type find(const_pointer value,
        size_type pos, size_type count) const BTDEF_NOEXCEPT
    {
        const_iterator it = std::search(cbegin(), cend(),
            value + pos, value + pos + count);
        return it == cend() ? npos : std::distance(cbegin(), it);
    }

    size_type find(const basic_string& other,
        size_type pos = 0) const BTDEF_NOEXCEPT
    {
        // FIXME TEST
        return find(other.data(), pos, other.size() - pos);
    }

    size_type find(const_pointer value, size_type pos = 0) const BTDEF_NOEXCEPT
    {
        return find(value, pos, std::strlen(value) - pos);
    }

    size_type find(value_type value, size_type pos = 0) const BTDEF_NOEXCEPT
    {
        return find(&value, pos, 1);
    }
};

typedef basic_string<char, allocator::basic<char>> string;

} // namespace util
} // namespace btdef

// ---- btdef::util::basic_string same type

template<class C, class A1, class A2>
bool operator==(const btdef::util::basic_string<C, A1>& lhs,
    const btdef::util::basic_string<C, A2>& rhs) BTDEF_NOEXCEPT
{
    return lhs.compare(rhs) == 0;
}

template<class T, class A>
bool operator!=(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) BTDEF_NOEXCEPT
{
    return !(lhs == rhs);
}

template<class T, class A>
bool operator<(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) BTDEF_NOEXCEPT
{
    return lhs.compare(rhs) < 0;
}

template<class T, class A>
bool operator>(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) BTDEF_NOEXCEPT
{
    return rhs < lhs;
}

template<class T, class A>
bool operator<=(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) BTDEF_NOEXCEPT
{
    return !(rhs < lhs);
}

template<class T, class A>
bool operator>=(const btdef::util::basic_string<T, A>& lhs,
    const btdef::util::basic_string<T, A>& rhs) BTDEF_NOEXCEPT
{
    return !(lhs < rhs);
}

// ---- btdef::util::basic_string same type
// ---- btdef::util::basic_string other type

template<class T, class A, class F>
bool operator==(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return lhs.compare(rhs) == 0;
}

template<class T, class A, class F>
bool operator!=(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return !(lhs == rhs);
}

template<class T, class A, class F>
bool operator<(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return lhs.compare(rhs) < 0;
}

template<class T, class A, class F>
bool operator>(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return lhs.compare(rhs) > 0;
}

template<class T, class A, class F>
bool operator<=(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return !(rhs < lhs);
}

template<class T, class A, class F>
bool operator>=(const btdef::util::basic_string<T, A>& lhs,
    const F& rhs) BTDEF_NOEXCEPT
{
    return !(lhs < rhs);
}

// ---- btdef::util::basic_string other type
