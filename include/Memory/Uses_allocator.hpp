#ifndef HY_USES_ALLOCATOR_HPP_
#define HY_USES_ALLOCATOR_HPP_
#include <type_traits>
namespace hy {
template <class T, class Alloc, typename = void>
struct uses_allocator : std::false_type {}; /// struct uses_allocator

template <class T, class Alloc>
struct uses_allocator<T, Alloc,
                      std::void_t<typename T::allocator_type,
                                  std::enable_if_t<std::is_convertible_v<
                                      Alloc, typename T::allocator_type>>>>
    : std::true_type {}; /// struct uses_allocator

template< class T, class Alloc >
inline constexpr bool uses_allocator_v = uses_allocator<T, Alloc>::value;

} // namespace hy

#endif /// HY_USES_ALLOCATOR_HPP_