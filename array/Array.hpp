#pragma once
#ifndef ARRAY_HPP_
#define ARRAY_HPP_

#include <cstddef>
#include <type_traits>
#include <iterator>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <type_traits>
#include "Utility.hpp"

namespace hy
{
    template <typename T, std::size_t N>
    struct array_traits_
    {
        using type_ = T[N];
    };

    template <typename Tp>
    struct array_traits_<Tp, 0>
    {
        struct type_
        {
            char unuse[1];
            constexpr operator Tp *() const noexcept { return nullptr; }
            constexpr Tp &operator[](std::size_t) const noexcept { return reinterpret_cast<Tp &>(const_cast<char &>(unuse[0])); }
        };
    };

    template <typename T, std::size_t N>
    struct array
    {
        static_assert(std::is_move_assignable_v<T>, "Must be MoveAssignable.!");
        static_assert(std::is_move_constructible_v<T>, "Must be MoveConstructible!");

        typename array_traits_<T, N>::type_ data_;

        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using iterator = pointer;
        using const_iterator = const_pointer;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        constexpr reference at(size_type pos)
        {
            if (pos >= N)
                throw std::out_of_range{""};
            return data_[pos];
        }

        constexpr const_reference at(size_type pos) const
        {
            if (pos >= N)
                throw std::out_of_range{""};
            return data_[pos];
        }

        constexpr reference operator[](size_type pos)
        {
            return data_[pos];
        }

        constexpr const_reference operator[](size_type pos) const
        {
            return data_[pos];
        }

        constexpr reference front()
        {
            return data_[0];
        }

        constexpr const_reference front() const
        {
            return data_[0];
        }

        constexpr reference back()
        {
            return data_[N - 1];
        }

        constexpr const_reference back() const
        {
            return data_[N - 1];
        }

        constexpr T *data() noexcept
        {
            return data_;
        }

        constexpr const T *data() const noexcept
        {
            return data_;
        }

        constexpr iterator begin() noexcept
        {
            return std::addressof(data_[0]);
        }

        constexpr const_iterator begin() const noexcept
        {
            return std::addressof(data_[0]);
        }

        constexpr const_iterator cbegin() const noexcept
        {
            return std::addressof(data_[N]);
        }

        constexpr iterator end() noexcept
        {
            return std::addressof(data_[N]);
        }

        constexpr const_iterator end() const noexcept
        {
            return std::addressof(data_[N]);
        }

        constexpr const_iterator cend() const noexcept
        {
            return std::addressof(data_[N]);
        }

        constexpr reverse_iterator rbegin() noexcept
        {
            return reverse_iterator(end());
        }

        constexpr const_reverse_iterator rbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        constexpr const_reverse_iterator crbegin() const noexcept
        {
            return const_reverse_iterator(end());
        }

        constexpr reverse_iterator rend() noexcept
        {
            return reverse_iterator(begin());
        }

        constexpr const_reverse_iterator rend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        constexpr const_reverse_iterator crend() const noexcept
        {
            return const_reverse_iterator(begin());
        }

        constexpr bool empty() const noexcept
        {
            return N == 0;
        }

        constexpr size_type size() const noexcept
        {
            return N;
        }

        constexpr size_type max_size() const noexcept
        {
            return N;
        }

        void fill(const T &value)
        {
            std::fill_n(begin(), size(), value);
        }

        void swap(array &other) noexcept((std::is_nothrow_swappable_v<T>))
        {
            std::swap_ranges(begin(), end(), other.begin());
        }
    }; // class arrayy

    template <class T, std::size_t N>
    inline bool operator==(const hy::array<T, N> &lhs,
                           const hy::array<T, N> &rhs)
    {
        return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin);
    }

    template <class T, std::size_t N>
    inline bool operator!=(const std::array<T, N> &lhs,
                           const std::array<T, N> &rhs)
    {
        return !(lhs == rhs);
    }

    template <class T, std::size_t N>
    inline bool operator<(const std::array<T, N> &lhs,
                          const std::array<T, N> &rhs)
    {
        return std::lexicographical_compare(lhs.cbegin(), lhs.end(), rhs.cbegin(), rhs.cend());
    }

    template <class T, std::size_t N>
    inline bool operator>(const std::array<T, N> &lhs,
                          const std::array<T, N> &rhs)
    {
        return rhs < lhs;
    }

    template <class T, std::size_t N>
    inline bool operator<=(const std::array<T, N> &lhs,
                           const std::array<T, N> &rhs)
    {
        return !(lhs > rhs);
    }

    template <class T, std::size_t N>
    inline bool operator>=(const std::array<T, N> &lhs,
                           const std::array<T, N> &rhs)
    {
        return !(lhs < rhs);
    }

    template <std::size_t i, class T, std::size_t N>
    inline constexpr T &get(std::array<T, N> &a) noexcept
    {
        static_assert(i < N, "array index is within bounds");
        return a[i];
    }

    template <std::size_t i, class T, std::size_t N>
    inline constexpr T &&get(std::array<T, N> &&a) noexcept
    {
        static_assert(i < N, "array index is within bounds");
        return hy::move(a[i]);
    }

    template <std::size_t i, class T, std::size_t N>
    inline constexpr const T &get(const std::array<T, N> &a) noexcept
    {
        static_assert(i < N, "array index is within bounds");
        return a[i];
    }

    // 屁用没有
    template <std::size_t i, class T, std::size_t N>
    inline constexpr const T &&get(const std::array<T, N> &&a) noexcept
    {
        static_assert(i < N, "array index is within bounds");
        return hy::move(a[i]);
    }

    template <class T, std::size_t N,
              typename = std::enable_if_t<std::is_swappable_v<T> || N == 0>>
    inline void swap(std::array<T, N> &lhs,
                     std::array<T, N> &rhs) noexcept(noexcept(lhs.swap(rhs)))
    {
        lhs.swap(rhs);
    }

    template <class T, std::size_t N>
    struct tuple_size<hy::array<T, N>> : std::integral_constant<std::size_t, N>
    {
    };

    template <class T>
    inline constexpr std::size_t tuple_size_v = tuple_size<T>::value;

    template <std::size_t I, typename T, std::size_t N>
    struct tuple_element<I, std::array<T, N>>
    {
        static_assert(I < N, "array index is in range");
        using type = T;
    };

    template <typename T, typename... U>
    array(T, U...)
        -> array<std::enable_if_t<(std::is_same_v<T, U> && ...), T>,
                 1 + sizeof...(U)>;
}

#endif // ARRAY_HPP_