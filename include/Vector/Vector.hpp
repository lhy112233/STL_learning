#ifndef HY_VECTOR_HPP_
#define HY_VECTOR_HPP_

#include "Allocator.hpp"
#include "Allocator_traits.hpp"
#include "Unique_ptr.hpp"
#include "Utility.hpp"
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
  inline static constexpr std::size_t kGrowthFactor = 2;

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
  vector() noexcept(noexcept(Allocator())) = default;

  explicit vector(const Allocator &alloc) noexcept : alloc_{alloc} {}

  vector(size_type count, const T &value, const Allocator &alloc = Allocator())
      : alloc_{alloc}, p_begin_{alloc_.allocate(count * 2)}, p_end_{p_begin_ +
                                                                    count},
        p_end_of_storage_(p_begin_ + count * 2) {
    std::uninitialized_fill_n(begin(), count, value);
  }

  explicit vector(size_type count, const Allocator &alloc = Allocator{})
      : alloc_{alloc}, p_begin_{alloc_.allocate(count * 2)}, p_end_{p_begin_},
        p_end_of_storage_{p_begin_ + count * 2} {
          std::uninitialized_value_construct_n(begin(), count);
  }

  template <class InputIt, typename = std::enable_if_t<std::i>>
  vector(InputIt first, InputIt last, const Allocator &alloc = Allocator())
      : alloc_{alloc}, p_begin_{(alloc_.allocate(last - first) * 2)},
        p_end_{p_begin_}, p_end_of_storage_{p_begin_ + (last - first) * 2} {
          std::uninitialized_copy(first, last, begin());
        }

  vector(const vector &other)
      : alloc_{hy::allocator_traits<allocator_type>::
                   select_on_container_copy_construction(
                       other.get_allocator())},
        p_begin_{alloc_.allocate(other.capacity())}, p_end_{p_begin_},
        p_end_of_storage_{p_begin_ + other.capacity()} {
          std::uninitialized_copy(other.begin(), other.end(), begin());
  }

  vector(const vector &other, const Allocator &alloc)
      : alloc_{alloc}, p_begin_{alloc.allocate(other.capacity())},
        p_end_{p_begin_}, p_end_of_storage_{p_begin_ + other.capacity()} {
    try {
      for (auto &ele : other) {
        new (hy::detail::to_address(p_end_)) T(ele);
        ++p_end_;
      }
    } catch (...) {
      while (!empty()) {
        hy::detail::to_address(--p_end_)->~T();
      }
      alloc.deallocate(p_begin_, other.capacity());
      throw;
    }
  }

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
          new (hy::detail::to_address(p_end_)) T(std::move(ele));
          ++p_end_;
        }
      } catch (...) {
        auto itor_rhs = other.begin();
        for (size_type i = 0; p_begin_ + i != p_end_; ++i) {
          new (hy::detail::to_address(itor_rhs)) T(std::move(*(p_begin_ + i)));
          hy::detail::to_address(p_begin_ + i)->~T();
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
    std::uninitialized_move(init.begin(),init.end(),p_begin_);
  }

  /*Destructors*/
  ~vector() {
    std::destroy_n(p_begin_, size());
    alloc_.deallocate(p_begin_, capacity());
  }

  /*Assignments*/
  vector &operator=(const vector &other) {
    if constexpr (std::allocator_traits<allocator_type>::
                      propagate_on_container_copy_assignment::value) {
      auto tmp_alloc = std::allocator_traits<
          Allocator>::select_on_container_copy_construction(other.alloc_);
      if (tmp_alloc != alloc_) {
      std:
        de
      }
    }
    auto temp_begin = alloc_.allocate(other.capacity());
    auto temp_end = temp_begin;
    auto temp_end_of_storage = temp_begin + other.capacity();

    try {
      for (auto &ele : other) {
        new (hy::detail::to_address(p_end_)) T(ele);
        ++p_end_;
      }
    } catch (...) {
      while (!empty()) {
        hy::detail::to_address(--p_end_)->~T();
      }
      alloc_.deallocate(capacity());
      throw;
    }
    swap(p_begin_, temp_begin);
    swap(p_end_, temp_end);
    swap(p_end_of_storage_, temp_end_of_storage);
    alloc_.deallocate(temp_begin, capacity());
    return *this;
  }

  vector &operator=(vector &&other) noexcept(
      std::allocator_traits<
          Allocator>::propagate_on_container_move_assignment::value ||
      std::allocator_traits<Allocator>::is_always_equal::value) {
    if constexpr (std::allocator_traits<allocator_type>::
                      propagate_on_container_move_assignment::value) {
      std::destroy(begin(), end());
      std::alloc_ = std::move(other.alloc_);
    }
  }

  vector &operator=(std::initializer_list<value_type> ilist) {
    auto temp_begin =
        alloc_.allocate(capacity() > ilist.size() ? capacity() : ilist.size());
    auto temp_end = temp_begin;
    auto temp_end_of_storage =
        temp_begin + (capacity() > ilist.size() ? capacity() : ilist.size());
    try {
      for (auto &ele : ilist) {
        new (hy::detail::to_address(p_end_)) T(std::move(ele));
        ++p_end_;
      }
    } catch (...) {
      while (!empty()) {
        hy::detail::to_address(--p_end_)->~T();
      }
      alloc_.deallocate(capacity());
      throw;
    }
    swap(p_begin_, temp_begin);
    swap(p_end_, temp_end);
    swap(p_end_of_storage_, temp_end_of_storage);
    alloc_.deallocate(temp_begin, capacity());
    return *this;
  }

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

  void shrink_to_fit() { /*TODO*/
  }

  /*Modifiers functions*/
  void clear() noexcept {
    while (p_begin_ != p_end_) {
      hy::detail::to_address(p_end_--)->~T();
    }
  }

  // template <class... Args>
  // iterator emplace(const_iterator pos, Args &&...args) {
  // if ()
  // }

  void pop_back() {
    std::allocator_traits<Allocator>::destroy(alloc_,
                                              hy::detail::to_address(--end()));
  }

  void resize(size_type count) {
    if (count == size()) {
      return;
    } else if (count < size()) {
      std::destroy(begin() + count - 1, end());
      end() = begin() + count - 1;
      return;
    } else {
      try {
        statements
      } catch (...) {
      }
    }
  }

  void resize(size_type count, const value_type &value) {}

  void swap(vector &other) noexcept(
      std::allocator_traits<Allocator>::propagate_on_container_swap::value ||
      std::allocator_traits<Allocator>::is_always_equal::value) {
    /// 此constexpr只有C++17及以上才有，否则去掉/*TODO */
    if constexpr (hy::allocator_traits<
                      allocator_type>::propagate_on_container_swap::value) {
      swap(alloc_, other.alloc_);
    }
    swap(p_begin_, other.p_begin_);
    swap(p_end_, other.p_end_);
    swap(p_end_of_storage_, other.p_end_of_storage_);
  }

private:
  /*details*/
  template <typename Ptr> value_type *data_detail(Ptr *ptr) const noexcept {
    return ptr;
  }

  template <typename Ptr> value_type *data_detail(Ptr ptr) const noexcept {
    return empty() ? nullptr : std::addressof(*begin());
  }

private:
  Allocator alloc_ = Allocator{};
  pointer p_begin_ = nullptr;          /// 头指针
  pointer p_end_ = nullptr;            /// 尾指针
  pointer p_end_of_storage_ = nullptr; /// 预分配存储尾指针

}; /// class vector

/*vector function*/
template <class T, class Alloc>
bool operator==(const hy::vector<T, Alloc> &lhs,
                const hy::vector<T, Alloc> &rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Alloc>
bool operator!=(const hy::vector<T, Alloc> &lhs,
                const hy::vector<T, Alloc> &rhs) {
  return !(lhs == rhs);
}

template <class T, class Alloc>
bool operator<(const hy::vector<T, Alloc> &lhs,
               const hy::vector<T, Alloc> &rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end());
}

template <class T, class Alloc>
bool operator<=(const hy::vector<T, Alloc> &lhs,
                const hy::vector<T, Alloc> &rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end(),
                                      [](const auto &lhs, const auto &rhs) {
                                        return (lhs < rhs || lhs == rhs);
                                      });
}

template <class T, class Alloc>
bool operator>(const hy::vector<T, Alloc> &lhs,
               const hy::vector<T, Alloc> &rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                      rhs.end(),
                                      [](const auto &lhs, const auto &rhs) {
                                        return !(lhs < rhs || lhs == rhs);
                                      });
}

template <class T, class Alloc>
bool operator>=(const hy::vector<T, Alloc> &lhs,
                const hy::vector<T, Alloc> &rhs) {
  return std::lexicographical_compare(
      lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
      [](const auto &lhs, const auto &rhs) { return !(lhs < rhs); });
}

template <class T, class Alloc>
void swap(hy::vector<T, Alloc> &lhs,
          hy::vector<T, Alloc> &rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

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