#ifndef ALLOCATOR_TRAITS_HPP_
#define ALLOCATOR_TRAITS_HPP_
#include "Pointer_traits.hpp"
#include <limits>
#include <type_traits>
#include <utility>

namespace hy {
namespace detail {
template <typename Alloc, typename Value_type, typename = void>
struct allocator_traits_pointer {
  using pointer = Value_type *;
}; /// allocator_traits_pointer

template <typename Alloc, typename Value_type>
struct allocator_traits_pointer<Alloc, Value_type,
                                std::void_t<typename Alloc::pointer>> {
  using pointer = typename Alloc::pointer;
}; /// allocator_traits_pointer

template <typename Alloc, typename Value_type, typename Pointer,
          typename = void>
struct allocator_traits_const_pointer {
  using const_pointer =
      typename hy::pointer_traits<Pointer>::template rebind<const Value_type>;
}; /// allocator_traits_const_pointer

template <typename Alloc, typename Value_type, typename Pointer>
struct allocator_traits_const_pointer<
    Alloc, Value_type, Pointer, std::void_t<typename Alloc::const_pointer>> {
  using const_pointer = typename Alloc::const_pointer;
}; /// allocator_traits_const_pointer

template <typename Alloc, typename Pointer, typename = void>
struct allocator_traits_void_pointer {
  using void_pointer =
      typename hy::pointer_traits<Pointer>::template rebind<void>;
}; /// allocator_traits_void_pointer

template <typename Alloc, typename Pointer>
struct allocator_traits_void_pointer<
    Alloc, Pointer, std::void_t<typename Alloc::void_pointer>> {
  using void_pointer = typename Alloc::void_pointer;
}; /// allocator_traits_void_pointer

template <typename Alloc, typename Pointer, typename = void>
struct allocator_traits_const_void_pointer {
  using const_void_pointer =
      typename hy::pointer_traits<Pointer>::template rebind<const void>;
}; /// allocator_traits_const_void_pointer

template <typename Alloc, typename Pointer>
struct allocator_traits_const_void_pointer<
    Alloc, Pointer, std::void_t<typename Alloc::const_void_pointer>> {
  using const_void_pointer = typename Alloc::const_void_pointer;
}; /// allocator_traits_const_void_pointer

template <typename Alloc, typename Pointer, typename = void>
struct allocator_traits_difference_type {
  using difference_type =
      typename hy::pointer_traits<Pointer>::difference_type;
}; /// allocator_traits_difference_type

template <typename Alloc, typename Pointer>
struct allocator_traits_difference_type<
    Alloc, Pointer, std::void_t<typename Alloc::difference_type>> {
  using difference_type = typename Alloc::difference_type;
}; /// allocator_traits_difference_type

template <typename Alloc, typename Difference_type, typename = void>
struct allocator_traits_size_type {
  using size_type = typename std::make_unsigned<Difference_type>::type;
}; /// allocator_traits_size_type

template <typename Alloc, typename Difference_type>
struct allocator_traits_size_type<Alloc, Difference_type,
                                  std::void_t<typename Alloc::size_type>> {
  using size_type = typename Alloc::size_type;
}; /// allocator_traits_size_type

template <typename Alloc, typename = void>
struct allocator_traits_propagate_on_container_copy_assignment {
  using propagate_on_container_copy_assignment = typename std::false_type;
}; /// allocator_traits_propagate_on_container_copy_assignment

template <typename Alloc>
struct allocator_traits_propagate_on_container_copy_assignment<
    Alloc,
    std::void_t<typename Alloc::propagate_on_container_copy_assignment>> {
  using propagate_on_container_copy_assignment =
      typename Alloc::propagate_on_container_copy_assignment;
}; /// allocator_traits_propagate_on_container_copy_assignment

template <typename Alloc, typename = void>
struct allocator_traits_propagate_on_container_move_assignment {
  using propagate_on_container_move_assignment = typename std::false_type;
}; /// allocator_traits_propagate_on_container_move_assignment

template <typename Alloc>
struct allocator_traits_propagate_on_container_move_assignment<
    Alloc,
    std::void_t<typename Alloc::propagate_on_container_move_assignment>> {
  using propagate_on_container_move_assignment =
      typename Alloc::propagate_on_container_move_assignment;
}; /// allocator_traits_propagate_on_container_move_assignment

template <typename Alloc, typename = void>
struct allocator_traits_propagate_on_container_swap {
  using propagate_on_container_swap = typename std::false_type;
}; /// allocator_traits_propagate_on_container_swap

template <typename Alloc>
struct allocator_traits_propagate_on_container_swap<
    Alloc, std::void_t<typename Alloc::propagate_on_container_swap>> {
  using propagate_on_container_swap =
      typename Alloc::propagate_on_container_swap;
}; /// allocator_traits_propagate_on_container_swaps

template <typename Alloc, typename = void>
struct allocator_traits_is_always_equal {
  using is_always_equal = typename std::is_empty<Alloc>::type;
}; /// allocator_traits_is_always_equal

template <typename Alloc>
struct allocator_traits_is_always_equal<Alloc,
                                        typename Alloc::is_always_equal> {
  using is_always_equal = typename Alloc::is_always_equal;
}; /// allocator_traits_is_always_equal

template <typename Alloc, typename T, typename = void>
struct allocator_traits_rebind_alloc {
  using rebind_alloc = Alloc;
}; /// allocator_traits_rebind_alloc

template <typename Alloc, typename T>
struct allocator_traits_rebind_alloc<
    Alloc, T, std::void_t<typename Alloc::template rebind<T>::other>> {
  using rebind_alloc = typename Alloc::template rebind<T>::other;
}; /// allocator_traits_rebind_alloc

template <typename Alloc>
inline auto
allocator_traits_select_on_container_copy_construction(const Alloc &a, int)
    -> decltype(a.select_on_container_copy_construction()) {
  static_assert(
      std::is_same_v<Alloc,
                     decltype(a.select_on_container_copy_construction())>,
      "Alloc ane decltype(a.select_on_container_copy_construction()) is no "
      "same!");
  return a.select_on_container_copy_construction();
} /// allocator_traits_select_on_container_copy_construction

template <typename Alloc>
inline auto
allocator_traits_select_on_container_copy_construction(const Alloc &a,
                                                       ...) -> Alloc {
  return a;
} /// allocator_traits_select_on_container_copy_construction

} // namespace detail

template <class Alloc> class allocator_traits {
  /*Member types*/
  using allocator_type = Alloc;
  using value_type = typename Alloc::value_type;
  using pointer =
      typename hy::detail::allocator_traits_pointer<Alloc, value_type>::pointer;
  using const_pointer = typename hy::detail::allocator_traits_const_pointer<
      Alloc, value_type, pointer>::const_pointer;
  using void_pointer =
      typename hy::detail::allocator_traits_void_pointer<Alloc,
                                                         pointer>::void_pointer;
  using const_void_pointer =
      typename hy::detail::allocator_traits_const_void_pointer<
          Alloc, pointer>::const_void_pointer;
  using difference_type = typename hy::detail::allocator_traits_difference_type<
      Alloc, pointer>::difference_type;
  using size_type = typename hy::detail::allocator_traits_size_type<
      Alloc, difference_type>::size_type;
  using propagate_on_container_copy_assignment = typename hy::detail::
      allocator_traits_propagate_on_container_copy_assignment<
          Alloc>::propagate_on_container_copy_assignment;
  using propagate_on_container_move_assignment = typename hy::detail::
      allocator_traits_propagate_on_container_move_assignment<
          Alloc>::propagate_on_container_move_assignment;
  using propagate_on_container_swap =
      typename hy::detail::allocator_traits_propagate_on_container_swap<
          Alloc>::propagate_on_container_swap;
  using is_always_equal = typename hy::detail::allocator_traits_is_always_equal<
      Alloc>::is_always_equal;

  /*Member alias templates*/
  template <typename T>
  using rebind_alloc =
      typename hy::detail::allocator_traits_rebind_alloc<Alloc,
                                                         T>::rebind_alloc;

  template <typename T>
  using rebind_traits = typename hy::allocator_traits<rebind_alloc<T>>;

  /*Member functions*/
  static pointer allocate(Alloc &a, size_type n) { return a.allocate(n); }
  static pointer allocate(Alloc &a, size_type n, const_void_pointer hint) {
    return a.allocate(n, hint);
  }; /// pointer allocate(Alloc &a, size_type n)

  static void deallocate(Alloc &a, pointer p, size_type n) {
    a.deallocate(p, n);
  } /// void deallocate(Alloc &a, pointer p, size_type n)

  static Alloc select_on_container_copy_construction(const Alloc &a) {
    return hy::detail::allocator_traits_select_on_container_copy_construction(
        a, 1);
  } /// Alloc select_on_container_copy_construction(const Alloc &a)

  /*construct detail*/
  template <typename T, typename... Args>
  inline static auto detail_construct(Alloc &a, T *p, Args &&...args, int)
      -> decltype(static_cast<void>(a.construct(p,
                                                std::forward<Args>(args)...)),
                  void()) {
    a.construct(p, std::forward<Args>(args)...);
  }

  template <typename T, typename... Args>
  inline static auto detail_construct(Alloc &a, T *p, Args &&...args,
                                      ...) -> void {
    ::new (static_cast<void *>(p)) T(std::forward<Args>(args)...);
  }

  template <class T, class... Args>
  inline static void construct(Alloc &a, T *p, Args &&...args) {
    detail_construct(a, p, std::forward<Args>(args)..., 1);
  }

  /*destroy detail*/
  template <typename T>
  inline static auto detail_destroy(Alloc &a, T *p, int)
      -> decltype(static_cast<void>(a.destroy(p)), void()) {
    a.destroy(p);
  } /// auto detail_destroy(Alloc& a, T* p, ...)

  template <typename T>
  inline static auto detail_destroy(Alloc &a, T *p, ...) -> void {
    p->~T();
  } /// auto detail_destroy(Alloc& a, T* p, ...)

  template <class T> static void destroy(Alloc &a, T *p) {
    detail_destroy(a, p);
  } /// void destroy( Alloc& a, T* p )

  /*max_size detail*/
  template <typename Up>
  inline static auto detail_max_size(const Alloc &a,
                                     int) noexcept -> decltype(a.max_size()) {
    static_assert(std::is_same_v<size_type, decltype((a.max_size()))>,
                  "size_type and decltype((a.max_size())) is no same!");
    return a.max_size();
  } /// aauto detail_max_size(const Alloc &a,int)

  template <typename Up>
  inline static auto detail_max_size(const Alloc &a,
                                     ...) noexcept -> size_type {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  } /// auto detail_max_size(const Alloc &a,...)

  static size_type max_size(const Alloc &a) noexcept {
    return detail_max_size(a, 1);
  } /// size_type max_size(const Alloc &a)

}; /// class allocator_traits
} // namespace hy

#endif // ALLOCATOR_TRAITS_HPP_