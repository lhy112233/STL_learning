#ifndef ALLOCATOR_HPP_
#define ALLOCATOR_HPP_

#include <cstddef>
#include <limits>
#include <memory>
#include <new>
#include <type_traits>

// std::allocator<int> a;
// 即给定分配器的任何实例都可交换?

namespace hy {

template <typename T> class allocator {
public:
  static_assert(!std::is_const_v<T>,
                "The C++ Standard forbids containers of const elements "
                "because allocator<const T> is ill-formed.");
  static_assert(!std::is_function_v<T>,
                "The C++ Standard forbids allocators for function elements "
                "because of [allocator.requirements].");
  static_assert(!std::is_reference_v<T>,
                "The C++ Standard forbids allocators for reference elements "
                "because of [allocator.requirements].");

  using value_type = T;
  using pointer [[deprecated]] = T *;
  using const_pointer [[deprecated]] = const T *;
  using reference [[deprecated]] = T &;
  using const_reference [[deprecated]] = const T &;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using propagate_on_container_move_assignment = std::true_type;
  using is_always_equal = std::true_type;
  template <typename U> struct [[deprecated]] rebind {
    using other = allocator<U>;
  };

  allocator() noexcept = default;
  allocator(const allocator &) noexcept = default;
  template <typename U> allocator(const allocator<U> &) noexcept {};

  ~allocator() = default;

  [[deprecated]] pointer address(reference x) const noexcept {
    return std::addressof(x);
  }

  [[deprecated]] const_pointer address(const_reference x) const noexcept {
    return std::addressof(x);
  }

  // 和无hint版本行为一致。无法实现hint的就近分配(libcxx的实现也无法达到此目标)
  [[deprecated]] T *allocate(std::size_t n, const void *hint) {
    if (std::numeric_limits<std::size_t>::max() / sizeof(T) < n)
      throw std::bad_array_new_length{};
    return static_cast<T *>(::operator new(n * sizeof(T)));
  }

  T *allocate(std::size_t n) {
    if (std::numeric_limits<std::size_t>::max() / sizeof(T) < n)
      throw std::bad_array_new_length{};
    return static_cast<T *>(::operator new(n * sizeof(T)));
  }

  void deallocate(T *p, std::size_t n) { ::operator delete(p); }

  size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }

  template <class U, class... Args>
  [[deprecated]] void construct(U *p, Args &&...args) {
    ::new (static_cast<void *>(p)) T(std::forward<Args>(args)...);
  }

  template <class U> [[deprecated]] void destroy(U *p) { p->~U(); }
};

template <> struct [[deprecated]] allocator<void> {
  using value_type = void;
  using pointer [[deprecated]] = void *;
  using const_pointer [[deprecated]] = const void *;
  using propagate_on_container_move_assignment = std::true_type;
  template <class U> struct [[deprecated]] rebind {
    typedef allocator<U> other;
  };
  using is_always_equal = std::true_type;
};

template <class T1, class T2>
bool operator==(const allocator<T1> &lhs, const allocator<T2> &rhs) noexcept {
  return true;
}

template <class T1, class T2>
bool operator!=(const allocator<T1> &lhs, const allocator<T2> &rhs) noexcept {
  return false;
}

} // namespace hy

#endif // ALLOCATOR_HPP_