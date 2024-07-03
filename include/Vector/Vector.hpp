#ifndef HY_VECTOR_HPP_
#define HY_VECTOR_HPP_

#include "Allocator.hpp"
#include "Allocator_traits.hpp"
#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include <utility>

namespace hy {
namespace detail {
/* 直接复用vector的pointer就行,没必要自己创建*/
// template <typename T, typename = void> struct vector_iterator {
//   using iterator = T *;
// }; /// struct vector_iterator

// template <typename T>
// struct vector_iterator<T, std::void_t<typename T::pointer>> {
//   using iterator = typename T::pointer;
// };

// template <typename T, typename = void> struct vector_const_iterator {
//   using const_iterator = const T *;
// }; /// struct vector_const_iterator

// template <typename T>
// struct vector_const_iterator<T, std::void_t<typename T::const_pointer>> {
//   using const_iterator = typename T::const_pointer;
// }; /// struct vector_const_iterator

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
  using iterator = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator = typename std::reverse_iterator<iterator>;
  using const_reverse_iterator = typename std::reverse_iterator<const_iterator>;

  /*static_assert*/
  static_assert(
      std::is_same_v<allocator_type, typename hy::allocator_traits<
                                         Allocator>::template rebind_alloc<T>>,
      "vector assert!");

  /*Member functions*/
  /*Constructors*/
  vector() noexcept(noexcept(Allocator()))
      : alloc_{}, p_begin_{nullptr}, p_end_{nullptr},
        p_end_of_storage_{nullptr} {}

  explicit vector(const Allocator &alloc) noexcept
      : alloc_{alloc}, p_begin_{nullptr}, p_end_{nullptr},
        p_end_of_storage_{nullptr} {}

  vector(size_type count, const T &value, const Allocator &alloc = Allocator())
      : alloc_{alloc}, p_begin_{alloc_.allocate(count * 2)}, p_end_{p_begin_},
        p_end_of_storage_{p_begin_ + count * 2} {
    try {
      for (size_type i = 0; i < count; ++i) {
        alloc_.construct(p_end_, value);
        ++p_end_;
      }
    } catch (...) {
      while (!empty()) {
        alloc_.destroy(--p_end_);
      }
      alloc_.deallocate(p_begin_, capacity());
      throw;
    }
  }

  explicit vector(size_type count, const Allocator &alloc = Allocator{})
      : alloc_{alloc}, p_begin_{alloc_.allocate(count * 2)}, p_end_{p_begin_},
        p_end_of_storage_{p_begin_ + count * 2} {
    try {
      for (size_type i = 0; i < count; ++i) {
        alloc_.construct(p_end_);
        ++p_end_;
      }
    } catch (...) {
      while (!empty()) {
        alloc_.destroy(--p_end_);
      }
      alloc_.deallocate(p_begin_, capacity());
      throw;
    }
  }

  template <class InputIt>
  vector(InputIt first, InputIt last, const Allocator &alloc = Allocator())
      : alloc_{alloc}, p_begin_{(alloc_.allocate(last - first) * 2)},
        p_end_{p_begin_}, p_end_of_storage_{p_begin_ + (last - first) * 2} {
    try {
      while (first != last) {
        alloc_.construct(p_end_, *first);
        ++first;
      }
    } catch (...) {
      while (!empty()) {
        alloc_.destroy(--p_end_);
      }
      alloc_.deallocate(begin(), capacity());
      throw;
    }
  }

  vector(const vector &other)
      : alloc_{hy::allocator_traits<allocator_type>::
                   select_on_container_copy_construction(
                       other.get_allocator())},
        p_begin_{alloc_.allocate(other.capacity())}, p_end_{p_begin_},
        p_end_of_storage_{p_begin_ + other.capacity()} {}

  vector(const vector &other, const Allocator &alloc)
      : alloc_{alloc}, p_begin_{alloc_.allocate(other.capacity())},
        p_end_{p_begin_}, p_end_of_storage_{p_begin_ + other.capacity()} {}

  vector(vector &&other) noexcept
      : alloc_{std::move(other.alloc_)}, p_begin_{other.p_begin_},
        p_end_{other.p_end_}, p_end_of_storage_{other.p_end_of_storage_} {
    other.p_begin_ = nullptr;
    other.p_end_ = nullptr;
    other.p_end_of_storage_ = nullptr;
  }

  vector(vector &&other, const Allocator &alloc) : alloc_{alloc} {
    if (alloc == other.get_allocator()) {
      p_begin_ = other.p_begin_;
      p_end_ = other.p_end_;
      p_end_of_storage_ = other.p_end_of_storage_;

      other.p_begin_ = nullptr;
      other.p_end_ = nullptr;
      other.p_end_of_storage_ = nullptr;
    } else {
      try {
        p_begin_ = alloc_.allocate(other.capacity());
        p_end_ = p_begin_;
        p_end_of_storage_ = p_begin_ + other.capacity();
        for (auto &ele : other) {
          alloc_.construct(p_end_, std::move(ele));
          ++p_end_;
        }
      } catch (...) {
        auto itor_rhs = other.begin();
        for (size_type i = 0; p_begin_ + i != p_end_; ++i) {
          other.alloc_.construct(itor_rhs, std::move(*(p_begin_ + i)));
          alloc_.destroy((p_begin_ + i));
          ++itor_rhs;
        }
        alloc_.deallocate(p_begin_, capacity());
        throw;
      }
    }
  }

  vector(std::initializer_list<T> init, const Allocator &alloc = Allocator())
      : alloc_{alloc}, p_begin_{alloc_.allocate(init.size() * 2)},
        p_end_{p_begin_}, p_end_of_storage_{p_begin_ + init.size() * 2} {
    try {
      for (auto &ele : init) {
        alloc_.construct(p_end_, std::move(ele));
        ++p_end_;
      }
    } catch (...) {
      while (!empty()) {
        alloc_.destroy(--p_end_);
      }
      alloc_.deallocate(p_begin_, init.size() * 2);
      throw;
    }
  }

  /*Destructors*/
  ~vector() {
    while (p_begin_ != p_end_) {
      alloc_.destroy(--p_end_);
    }
    alloc_.deallocate(p_begin_, capacity());
  }

  /*Assignments*/

  /*Element access functions*/
  allocator_type get_allocator() const noexcept { return alloc_; }

  reference at(size_type pos) {
    if (pos >= size()) {
      throw std::out_of_range{"vector's at function param pos out of range"};
    }
    return p_begin_[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= size()) {
      throw std::out_of_range{"vector's at function param pos out of range"};
    }
    return p_begin_[pos];
  }

  reference operator[](size_type size) { return p_begin_[size]; }

  const_reference operator[](size_type size) const { return p_begin_[size]; }

  reference front() { return *begin(); }

  const_reference front() const { return *cbegin(); }

  reference back() { return *std::prev(end()); }

  const_reference back() const { return *std::prev(cend()); }

  T *data() noexcept { return data_detail(p_begin_); }

  const T *data() const noexcept { return data_detail(p_begin_); }

  /*Iterators functions*/
  iterator begin() noexcept { return p_begin_; }

  const_iterator begin() const noexcept { return p_begin_; }

  const_iterator cbegin() const noexcept { return p_begin_; }

  iterator end() noexcept { return p_end_; }

  const_iterator end() const noexcept { return p_end_; }

  const_iterator cend() const noexcept { return p_end_; }

  reverse_iterator rbegin() noexcept { return std::reverse_iterator(end()); }

  reverse_iterator rbegin() const noexcept {
    return std::reverse_iterator(end());
  }

  const_reverse_iterator crbegin() const noexcept {
    return std::reverse_iterator(end());
  }

  reverse_iterator rend() noexcept { return std::reverse_iterator(begin()); }

  const_reverse_iterator rend() const noexcept {
    return std::reverse_iterator(begin());
  }

  const_reverse_iterator crend() const noexcept {
    return std::reverse_iterator(begin());
  }

  /*Capacity functions*/
  bool empty() const noexcept { return begin() == end(); }

  size_type size() const noexcept { return std::distance(p_begin_, p_end_); }

  size_type max_size() const noexcept {
    return std::numeric_limits<difference_type>::max();
  }

  inline void reserve(size_type new_cap) {
    if (new_cap <= capacity())
      return;
    if (new_cap > max_size())
      throw std::length_error{"vector's reserve is too long!"};
    auto temp_mem = alloc_.allocate(new_cap);
    try {

    } catch (...) {

      alloc_.deallocate(temp_mem, new_cap);
    }
  }

  size_type capacity() const noexcept {
    return std::distance(p_begin_, p_end_of_storage_);
  }

  // void shrink_to_fit() {} ///No implementation

  void shrink_to_fit() { /*TODO*/ }

  /*Modifiers functions*/
  void clear() noexcept {
    while (p_begin_ != p_end_) {
      alloc_.destroy(p_end_--);
    }
  }

  // template <class... Args>
  // iterator emplace(const_iterator pos, Args &&...args) {
  //   if ()
  // }

private:
  /*details*/
  template <typename Ptr> value_type *data_detail(Ptr *ptr) const noexcept {
    return ptr;
  }

  template <typename Ptr> value_type *data_detail(Ptr ptr) const noexcept {
    return empty() ? nullptr : std::addressof(*begin());
  }

private:
  Allocator alloc_;
  pointer p_begin_;          /// 头指针
  pointer p_end_;            /// 尾指针
  pointer p_end_of_storage_; /// 预分配存储尾指针

}; /// class vector

/*vector function*/
template <class T, class Alloc>
bool operator==(const hy::vector<T, Alloc> &lhs,
                const hy::vector<T, Alloc> &rhs) {
  auto itor_lhs = lhs.begin();
  auto itor_rhs = rhs.begin();
}

template <class T, class Alloc>
bool operator!=(const hy::vector<T, Alloc> &lhs,
                const hy::vector<T, Alloc> &rhs) {}

template <class T, class Alloc>
bool operator<(const hy::vector<T, Alloc> &lhs,
               const hy::vector<T, Alloc> &rhs) {}

template <class T, class Alloc>
bool operator<=(const hy::vector<T, Alloc> &lhs,
                const hy::vector<T, Alloc> &rhs) {}

template <class T, class Alloc>
bool operator>(const hy::vector<T, Alloc> &lhs,
               const hy::vector<T, Alloc> &rhs) {}

template <class T, class Alloc>
bool operator>=(const hy::vector<T, Alloc> &lhs,
                const hy::vector<T, Alloc> &rhs) {}

template <class T, class Alloc>
void swap(hy::vector<T, Alloc> &lhs,
          hy::vector<T, Alloc> &rhs) noexcept(noexcept(lhs.swap(rhs))) {}

namespace pmr {
template <class T>
using vector = hy::vector<T, std::pmr::polymorphic_allocator<T>>;
} // namespace pmr

/*特化 vector<bool>*/
template <class Allocator>
class vector<bool, Allocator> {
}; /// template <class Allocator> class vector<bool, Allocator>

} // namespace hy

#endif /// HY_VECTOR_HPP_