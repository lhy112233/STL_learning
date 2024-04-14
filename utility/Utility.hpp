#ifndef UTILITY_HPP_
#define UTILITY_HPP_
#include <type_traits>

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

    // Begin std::conditional
    template <bool B, class T, class F>
    struct conditional
    {
        using type = T;
    };

    template <class T, class F>
    struct conditional<false, T, F>
    {
        using type = F;
    }; // End std::conditional

    // Begin struct conjunction
    template <typename...>
    struct conjunction : hy::true_type
    {
    };
    template <typename B1>
    struct conjunction<B1> : B1
    {
    };

    template <typename B1, typename... Bn>
    struct conjunction<B1, Bn...>
        : hy::conditional_t<bool(B1::value), conjunction<Bn...>, B1>
    {
    }; // End struct conjunction

}

#endif