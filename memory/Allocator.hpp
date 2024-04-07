#pragma once
#ifndef ALLOCATOR_HPP_
#define ALLOCATOR_HPP_

#include <type_traits>
#include <cstddef>
#include <memory>

// 即给定分配器的任何实例都可交换?

namespace hy
{

    template <typename T>
    struct allocator
    {
        using value_type = T;
        using pointer [[deprecated]] = T *;
        using const_pointer [[deprecated]] = const T *;
        using reference [[deprecated]] = T &;
        using const_reference [[deprecated]] = const T &;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using propagate_on_container_move_assignment = std::true_type;
        using is_always_equal = std::true_type;
        template <typename U>
        struct [[deprecated]] rebind
        {
            typedef allocator<U> other;
        };
    };

    template <>
    struct [[deprecated]] allocator<void>
    {
        using value_type = void;
        using pointer [[deprecated]] = void *;
        using const_pointer [[deprecated]] = const void *;
        using propagate_on_container_move_assignment = std::true_type;
        template <class U>
        struct [[deprecated]] rebind
        {
            typedef allocator<U> other;
        };
        using is_always_equal = std::true_type;
    };

    template <class T1, class T2>
    bool operator==(const allocator<T1> &lhs, const allocator<T2> &rhs) noexcept
    {
        return true;
    }

    template <class T1, class T2>
    bool operator!=(const allocator<T1> &lhs, const allocator<T2> &rhs) noexcept
    {
        return false;
    }

}

#endif // ALLOCATOR_HPP_