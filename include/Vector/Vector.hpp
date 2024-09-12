#ifndef HY_VECTOR_HPP_
#define HY_VECTOR_HPP_
#include "Allocator.hpp"
#include "Allocator_traits.hpp"
#include <cstddef>

namespace hy {
template <typename T, typename Alloc = hy::allocator<T>> class vector {
public:
  using value_type = T;
  using allocator_type = Alloc;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type &;
  using const_reference = const value_type &;
  using pointer = typename std::allocator_traits<Alloc>::pointer;
  using const_point = typename std::allocator_traits<Alloc>::const_pointer;
  using iterator = pointer;           /*临时*/
  using const_iterator = const_point; /*临时*/
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  iterator begin() { return p_begin_; }

  const_iterator begin() const { return p_begin_; }

  const_iterator cbegin() const noexcept { return p_begin_; }

  iterator end() noexcept { return p_index_; }

  const_iterator end() const noexcept { return p_index_; }

  const_iterator cend() const noexcept { return p_index_; }

reverse_iterator rbegin() {
    return std::reverse_iterator(end());
}

const_reverse_iterator rbegin() const {
    return std::reverse_iterator(end());
}

const_reverse_iterator crbegin() const noexcept {
    return std::reverse_iterator(end());
}

  bool empty() const { return begin() == end(); }

  //   void swap(vector &other) noexcept(noexcept(
  //       allocator_traits<Alloc>::propagate_on_container_swap::value ||
  //       allocator_traits<Alloc>::is_always_equal::value)) {

  //       }

private:
  Alloc alloc_;
  T *p_begin_;
  T *p_end_;
  T *p_index_;  //目标数据的后一位

}; // class vector

template <typename Alloc> class vector<bool, Alloc> {
public:
private:
}; // class vector<bool>

} // namespace hy

#endif // HY_VECTOR_HPP_