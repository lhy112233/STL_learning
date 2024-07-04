#ifndef HY_USER_ALLOCATOR_HPP_
#define HY_USER_ALLOCATOR_HPP_

#include "Type_traits.hpp"
#include <type_traits>

template <typename T, typename Alloc>
struct uses_allocator : hy::false_type {}; /// struct uses_allocator

template <typename T, typename Alloc>
struct uses_allocator<
    T, std::void_t<std::enable_if_t<std::conjunction_v<
           hy::has_allocator_type<T>,
           std::is_convertible<Alloc, typename T::allocator_type>>>>>
    : hy::true_type {} /// struct uses_allocator

template< typename T, typename Alloc >
inline constexpr bool uses_allocator_v = uses_allocator<T, Alloc>::value;

#endif /// HY_USER_ALLOCATOR_HPP_