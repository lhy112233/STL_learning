#pragma once
#ifndef UTILITY_HPP_
#define UTILITY_HPP_
#include <type_traits>

namespace hy
{
    template <typename T>
    [[nodiscard]] decltype(auto) move(T &&t) noexcept
    {
        return static_cast<std::remove_reference_t<T> &&>(t);
    }

    template <typename T>
    [[nodiscard]] decltype(auto) forward(std::remove_reference_t<T> &t) noexcept
    {
        return static_cast<T &&>(t);
    }

    template <typename T>
    [[nodiscard]] decltype(auto) forward(std::remove_reference_t<T> &&t) noexcept
    {
        static_assert(!std::is_lvalue_reference_v<T>, "hy::forward must not be used to convert an rvalue to an lvalue");
        return static_cast<T &&>(t);
    }

    template <std::size_t I, class T>
    struct tuple_element;

    template <class T>
    struct tuple_size;
}

#endif