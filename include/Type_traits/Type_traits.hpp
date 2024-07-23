#ifndef TYPR_TRAITS_HPP_
#define TYPR_TRAITS_HPP_

#include <cstddef>  //nullptr_t

namespace hy
{
    template <class T, T v>
    struct integral_constant
    {
        using value_type = T;
        using type = typename hy::integral_constant<T, v>;

        static constexpr T value = v;

        constexpr operator value_type() const noexcept { return value; }
        constexpr value_type operator()() const noexcept { return value; }

    }; // hy::integral_constant

    using true_type = hy::integral_constant<bool, true>;

    using false_type = hy::integral_constant<bool, false>;

    template <bool B>
    using bool_constant = integral_constant<bool, B>;

    template <typename T, typename U>
    struct is_same : hy::false_type
    {
    };

    template <typename T>
    struct is_same<T, T> : hy::true_type
    {
    };

    template <typename T, typename U>
    inline constexpr bool is_same_v = is_same<T, U>::value;

    template <class T>
    struct remove_cv
    {
        using type = T;
    };

    template <class T>
    struct remove_cv<const T>
    {
        using type = T;
    };

    template <class T>
    struct remove_cv<volatile T>
    {
        using type = T;
    };

    template <class T>
    struct remove_cv<const volatile T>
    {
        using type = T;
    };

    template <class T>
    struct remove_const
    {
        using type = T;
    };

    template <class T>
    struct remove_const<const T>
    {
        using type = T;
    };

    template <class T>
    struct remove_volatile
    {
        using type = T;
    };

    template <class T>
    struct remove_volatile<volatile T>
    {
        typedef T type;
    };

    template <typename T>
    using remove_cv_t = typename remove_cv<T>::type;

    template <class T>
    using remove_const_t = typename remove_const<T>::type;

    template <class T>
    using remove_volatile_t = typename remove_volatile<T>::type;

    template <typename T>
    struct is_void : hy::is_same<void, hy::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_void_v = is_void<T>::value;

    template <typename T>
    struct is_null_pointer : hy::is_same<std::nullptr_t, hy::remove_cv_t<T>>
    {
    };

    template <typename T>
    inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

    // template <typename T>
    // struct is_integral : 


    template <typename T>
    struct remove_reference
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T &>
    {
        using type = T;
    };

    template <typename T>
    struct remove_reference<T &&>
    {
        using type = T;
    };

    template <typename T>
    using remove_reference_t = typename remove_reference<T>::type;





    /*CPP23*/
    
template<class T>
struct remove_cvref
{
    using type = hy::remove_cv_t<hy::remove_reference_t<T>>;
};

template< class T >
using remove_cvref_t = typename hy::remove_cvref<T>::type;

} // namespace hy

#endif // TYPR_TRAITS_HPP_