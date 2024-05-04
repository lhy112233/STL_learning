#ifndef UTILITY_HPP_
#define UTILITY_HPP_
#include <type_traits>
#include "Type_traits.hpp"

namespace hy
{
    template <std::size_t I, class T>
    struct tuple_element;

    template <class T>
    struct tuple_size;

    template <typename T>
    [[nodiscard]] decltype(auto) move(T &&t) noexcept
    {
        return static_cast<hy::remove_reference_t<T> &&>(t);
    } // hy::move

    template <typename T>
    [[nodiscard]] decltype(auto) forward(std::remove_reference_t<T> &t) noexcept
    {
        return static_cast<T &&>(t);
    } // hy::forward

    template <typename T>
    [[nodiscard]] decltype(auto) forward(std::remove_reference_t<T> &&t) noexcept
    {
        static_assert(!std::is_lvalue_reference_v<T>, "hy::forward must not be used to convert an rvalue to an lvalue");
        return static_cast<T &&>(t);
    } // hy::forward

    // Begin struct integral_constant

    // End struct integral_constant



}

#endif