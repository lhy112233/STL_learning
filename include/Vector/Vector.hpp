#ifndef HY_VECTOR_HPP_
#define HY_VECTOR_HPP_

#include "Allocator.hpp"
#include "Allocator_traits.hpp"
#include <cstddef>
#include <type_traits>

namespace hy {
namespace detail {
template <typename T, typename = void> struct vector_iterator {
  using iterator = T *;
}; /// struct vector_iterator

template <typename T>
struct vector_iterator<T, std::void_t<typename T::pointer>> {
  using iterator = typename T::pointer;
};

template <typename T, typename = void> struct vector_const_iterator {
  using const_iterator = const T *;
}; /// struct vector_const_iterator

template <typename T>
struct vector_const_iterator<T, std::void_t<typename T::const_pointer>> {
  using const_iterator = typename T::const_pointer;
}; /// struct vector_const_iterator

} // namespace detail

template <typename T, typename Allocator = hy::allocator<T>> class vector {
public:
  /*Member types*/
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = typename hy::allocator_traits<Allocator>::pointer;
  using const_pointer =
      typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = typename hy::detail::vector_iterator<T>;
  using const_iterator = typename hy::detail::vector_const_iterator<T>;
  using reverse_iterator = typename std::reverse_iterator<iterator>;
  using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

  /*Member functions*/
  /*Constructors*/
  vector() noexcept(noexcept(Allocator())) {}

  /*Destructors*/
  ~vector(){}

  /*Assignments*/

  /*Element access functions*/

  /*Iterators functions*/

  /*Capacity functions*/
  bool empty() const noexcept { return p_begin_ == p_end_; }

  size_type size() const noexcept { return std::distance(p_begin_, p_end_); }

  size_type max_size() const noexcept {
    return std::numeric_limits<difference_type>::max();
  }

  void reserve(size_type new_cap) { /*TODO*/ }

  size_type capacity() const noexcept {
    return static_cast<size_type>(p_end_of_storage_ - p_begin_);
  }

  void shrink_to_fit() { /*TODO*/ }

  /*Modifiers functions*/
  void clear() noexcept {}

private:
  pointer p_begin_;          /// 头指针
  pointer p_end_;            /// 尾指针
  pointer p_end_of_storage_; /// 预分配存储尾指针

}; /// class vector

} // namespace hy

#endif /// HY_VECTOR_HPP_