#pragma once

#include "btdef/allocator/basic.hpp"

#include <cstring>
#include <cassert>

namespace btdef {
namespace allocator {

/*
 *  from rapidjson (http://rapidjson.org/)
 */

template<typename T>
class basic_pool
{
public:
    static const std::size_t chunk_capacity = std::size_t(64 * 1024);

private:
    struct chunk_header
    {
        std::size_t capacity_;
        std::size_t size_;
        chunk_header *next_;
    };

    std::size_t chunk_capacity_{};
    chunk_header *head_{nullptr};
    void *buffer_{nullptr};
    T* allocator_{nullptr};
    T* own_{nullptr};

    basic_pool(const basic_pool&);
    basic_pool& operator=(const basic_pool&);

public:
    basic_pool(std::size_t chunk_size = chunk_capacity,
        T* allocator = nullptr) BTDEF_NOEXCEPT
        : chunk_capacity_(chunk_size)
        , allocator_(allocator)
    {   }

    basic_pool(void *buffer, size_t size,
        std::size_t chunk_size = chunk_capacity, T* allocator = nullptr) BTDEF_NOEXCEPT
        : chunk_capacity_(chunk_size)
        , buffer_(buffer)
        , allocator_(allocator)
    {
        assert(buffer);
        assert(size > sizeof(chunk_header));
        head_ = reinterpret_cast<chunk_header*>(buffer);
        head_->capacity_ = size - sizeof(chunk_header);
        head_->size_ = 0;
        head_->next_ = nullptr;
    }

    ~basic_pool() BTDEF_NOEXCEPT
    {
        clear();
        delete own_;
    }

    void clear() BTDEF_NOEXCEPT
    {
        while (head_ && (head_ != buffer_))
        {
            chunk_header* next = head_->next_;
            allocator_->free(head_);
            head_ = next;
        }
        if (head_ && (head_ == buffer_))
            head_->size_ = 0;
    }

    std::size_t capacity() const BTDEF_NOEXCEPT
    {
        std::size_t capacity = 0;
        for (chunk_header* c = head_; c != 0; c = c->next_)
            capacity += c->capacity_;
        return capacity;
    }

    std::size_t size() const BTDEF_NOEXCEPT
    {
        size_t size = 0;
        for (chunk_header* c = head_; c != 0; c = c->next_)
            size += c->size_;
        return size;
    }

    void* malloc(std::size_t size) BTDEF_NOEXCEPT
    {
        if (!size)
            return nullptr;

        size = BTDEF_ALLOCATOR_ALIGN(size);

        if (head_ == 0 || head_->size_ + size > head_->capacity_)
        {
            std::size_t next_chunk_capacity = chunk_capacity > size ?
                chunk_capacity : size;
            if (!add_chunk(next_chunk_capacity))
                return nullptr;
        }

        void *buffer = reinterpret_cast<char *>(head_) +
            BTDEF_ALLOCATOR_ALIGN(sizeof(chunk_header)) + head_->size_;
        head_->size_ += size;
        return buffer;
    }

    void* realloc(void* ptr, std::size_t size, std::size_t new_size) BTDEF_NOEXCEPT
    {
        if (!ptr)
            return malloc(new_size);

        if (new_size == 0)
            return nullptr;

        size = BTDEF_ALLOCATOR_ALIGN(size);
        new_size = BTDEF_ALLOCATOR_ALIGN(new_size);

        // Do not shrink if new size is smaller than original
        if (size >= new_size)
            return ptr;

        if (ptr == reinterpret_cast<char *>(head_) +
            BTDEF_ALLOCATOR_ALIGN(sizeof(chunk_header)) + head_->size_ - size)
        {
            std::size_t increment = static_cast<std::size_t>(new_size - size);
            if (head_->size_ + increment <= head_->capacity_)
            {
                head_->size_ += increment;
                return ptr;
            }
        }

        // Realloc process: allocate and copy memory,
        // do not free original buffer.
        void* new_buffer = malloc(new_size);
        if (new_buffer)
        {
            if (size)
                std::memcpy(new_buffer, ptr, size);
            return new_buffer;
        }
        else
            return nullptr;
    }

    static void free(void*) BTDEF_NOEXCEPT
    {   }

private:

    bool add_chunk(std::size_t capacity) BTDEF_NOEXCEPT
    {
        if (!allocator_)
            own_ = allocator_ = new T();

        chunk_header* chunk = reinterpret_cast<chunk_header*>(
            allocator_->malloc(BTDEF_ALLOCATOR_ALIGN(sizeof(chunk_header))
                                                                + capacity));

        if (chunk)
        {
            chunk->capacity_ = capacity;
            chunk->size_ = 0;
            chunk->next_ = head_;
            head_ =  chunk;
            return true;
        }
        else
            return false;
    }
};

typedef basic_pool<basic<char>> pool;

} // namespace allocator
} // namespace btdef
