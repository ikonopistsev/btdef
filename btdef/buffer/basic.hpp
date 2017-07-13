#pragma once

#include <cassert>
#include <cstdint>
#include <sys/uio.h>

#include "btdef/allocator/hider.hpp"

namespace btdef {
namespace buffer {
namespace detail {

typedef iovec iovec_type;

#ifndef BTDEF_BUFFER_CHUNK_COUNT
#define BTDEF_BUFFER_CHUNK_COUNT 9
#endif // BTDEF_BUFFER_CHUNK_COUNT

struct cache_meta
{
    // вместимость кэша
    std::size_t capacity_{0};
    // размер кэша
    std::size_t size_{0};
};

struct chunk_meta
{
    // количество кусков
    std::size_t chunk_count_{0};
    // номер крайнего кусока
    std::size_t chunk_{0};
};

struct header
{
    // общий размер памяти (то что записано)
    allocator allocator_{};
    std::size_t size_{0};
    iovec *data_{nullptr};
};

struct chunk_meta
{
    typedef void(*cleanup_type)(const void *data, std::size_t len, void *that);
    // сколько памяти выделяли
    std::size_t chunk_capacity_{0};
    // метод очистки памяти
    cleanup_type clean_fn_{nullptr};
    // указатель передаваемый в callback
    void *clean_param_{nullptr};
};

} // namespace detail

template<class T, class A>
class basic
{
public:
    typedef std::size_t size_type;
    typedef A allocator_type;
    typedef T iovec_type;
    typedef iovec_type* iovec_pointer;
    typedef const iovec_type* const_iovec_pointer;
    typedef detail::header* handle_type;
    typedef const detail::header* const_handle_type;
    typedef detail::chunk_meta* meta_pointer;
    typedef const detail::chunk_meta* const_meta_pointer;

private:
    allocator::hider<allocator_type, handle_type> data_;

    allocator_type& get_allocator() noexcept
    {
        return data_;
    }

    handle_type get_handle() const noexcept
    {
        return data_.get();
    }

    size_type get_memory_size() const noexcept
    {
        handle_type handle = get_handle();
        assert(handle);
        return handle->memory_size_;
    }

    size_type reserve_first_chunk(size_type size, size_type count) noexcept
    {
        // первая цепочка состоит из размера первой цепочки плюс :
        // память под объект
        size += sizeof(detail::header);
        // массив iovec
        size += sizeof(iovec_type) * count;
        // массив калбеков
        size += sizeof(detail::chunk_meta) * count;

        auto handle = data_.create(size);
        if (handle)
        {
            auto ivp = get_iovec(handle);
            // зануляем информацию в первном куске
            ivp->iov_base = nullptr;
            ivp->iov_len = 0;

            // сохраняем размер выделенной памяти в куске
            get_meta(ivp, count)->chunk_capacity_ = size;
            // сохраняем количество кусков
            handle->chunk_count_ = count;

            return size;
        }

        return 0;
    }

    size_type reserve_chunk(size_type size, size_type i) noexcept
    {
        return 0;
    }

    static inline iovec_pointer get_iovec(handle_type handle) noexcept
    {
        assert(handle);
        return static_cast<iovec_pointer>(static_cast<void*>(handle + 1));
    }

    static inline meta_pointer get_meta(iovec_pointer ivp,
        size_type chunk_count) noexcept
    {
        assert(ivp);
        return static_cast<meta_pointer>(static_cast<void*>(ivp + chunk_count));
    }

    static inline meta_pointer get_meta(const handle_type handle,
        size_type chunk_count) noexcept
    {
        assert(handle);
        return get_meta(get_iovec(handle));
    }

    static inline bool get_is_cached(handle_type handle,
        size_type chunk_count) noexcept
    {
        assert(handle);
        auto i = get_iovec(handle);
        auto p = static_cast<void*>(get_meta(i, chunk_count) + chunk_count);
        return &i->iov_base == &p;
    }

    static inline bool get_is_cached(handle_type handle) noexcept
    {
        assert(handle);
        return get_is_cached(handle, handle->chunk_count_);
    }

public:
    basic(const allocator_type& a = allocator_type()) noexcept
        : data_(a, nullptr)
    {   }

    basic(size_type size_reserverd, const allocator_type& a = allocator_type())
        : data_(a, nullptr)
    {   }

    basic(size_type size_reserverd, size_type chunk_conut,
        const allocator_type& a = allocator_type())
        : data_(a, nullptr)
    {   }

    basic(basic&& other) noexcept
        : data_(std::move(other.data_))
    {   }

    size_type reserve(size_type size_reserverd, size_type chunk_count) noexcept
    {
        if (size_reserverd && chunk_count)
        {
            if (!reserve_first_chunk(size_reserverd, chunk_count))
                return 0;
        }

        return size();
    }

    size_type reserve(size_type size_reserverd) noexcept
    {
        return reserve(size_reserverd, detail::header::initial_chunk_count);
    }

    size_type size() const noexcept
    {
        auto handle = get_handle();
        if (handle)
            return handle->memory_size_;
        return 0;
    }

    bool empty() const noexcept
    {
        return size() == 0;
    }

    bool is_cached() const noexcept
    {
        return get_is_cached(get_handle());
    }
};

} // namespace buffer
} // namespace btdef
