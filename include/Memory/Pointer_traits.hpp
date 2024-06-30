#ifndef POINTER_TRAITS_HPP_
#define POINTER_TRAITS_HPP_
#include <cstddef>
#include <memory>
#include <type_traits>

namespace hy {
namespace detail {
template <typename T> struct pointer_traits_element_type_template {
  class undefine_;
  using element_type = undefine_;
}; /// struct pointer_traits_element_type_template

template <template <typename, typename...> typename TemplateParam, typename T,
          typename... Args>
struct pointer_traits_element_type_template<TemplateParam<T, Args...>> {
  using element_type = T;
}; /// struct pointer_traits_element_type_template

// template<typename >

template <typename Ptr, typename = void> struct pointer_traits_element_type {
  using element_type =
      typename pointer_traits_element_type_template<Ptr>::element_type;
}; /// pointer_traits_element_type

template <typename Ptr>
struct pointer_traits_element_type<Ptr,
                                   std::void_t<typename Ptr::element_type>> {
  using element_type = typename Ptr::element_type;
}; /// pointer_traits_element_type

template <typename Ptr, typename = void> struct pointer_traits_difference_type {
  using difference_type = std::ptrdiff_t;
}; /// struct pointer_traits_difference_type

template <typename Ptr>
struct pointer_traits_difference_type<
    Ptr, std::void_t<typename Ptr::difference_type>> {
  using difference_type = typename Ptr::difference_type;
}; /// struct pointer_traits_difference_type

template <typename T, typename U> struct pointer_traits_rebind_template {
  static_assert(false, "Error!");
};

template <typename T, template <typename, typename...> typename TemplateParam,
          typename U, typename... Args>
struct pointer_traits_rebind_template<T, TemplateParam<U, Args...>> {
  using rebind = TemplateParam<T, Args...>;
};

template <typename U, typename Ptr, typename = void>
struct pointer_traits_rebind {
  using rebind =
      typename hy::detail::pointer_traits_rebind_template<U, Ptr>::rebind;
}; /// struct pointer_traits_rebind

template <typename U, typename Ptr>
struct pointer_traits_rebind<Ptr, U,
                             std::void_t<typename Ptr::template rebind<U>>> {
  using rebind = typename Ptr::template rebind<U>;
}; /// struct pointer_traits_rebind

} // namespace detail

template <typename Ptr> struct pointer_traits {
  /*Member types*/
  using pointer = Ptr;
  using element_type =
      typename hy::detail::pointer_traits_element_type<Ptr>::element_type;
  using difference_type =
      typename hy::detail::pointer_traits_difference_type<Ptr>::difference_type;

  /*Member alias templates*/
  template <typename U>
  using rebind = typename hy::detail::pointer_traits_rebind<U, Ptr>::rebind;

  /*Member fundtions*/
  inline static pointer pointer_to(element_type &r) {
    return Ptr::pointer_to(r);
  }

}; // template <typename Ptr> struct pointer_traits

template <typename T> struct pointer_traits<T *> {
  using pointer = T *;
  using element_type = T;
  using difference_type = std::ptrdiff_t;

  template <typename U> using rebind = U *;

  static pointer pointer_to(element_type &r) noexcept {
    return std::addressof(r);
  }
}; // template <typename T> struct pointer_traits<T *>

} // namespace hy

#endif // POINTER_TRAITS_HPP_