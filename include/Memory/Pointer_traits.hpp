#ifndef POINTER_TRAITS_HPP_
#define POINTER_TRAITS_HPP_
#include <cstddef>
#include <memory>
#include <type_traits>
std::pointer_traits<int> a;

namespace hy {
template <typename Ptr> struct pointer_traits {
private:
  template<typename Tp,typename = void>
  struct element_type_impl;

  template<typename Tp >
  struct element_type_impl<Tp,typename Tp::element_type>{
    using element_type = typename Tp::element_type;
  };

  template<>


  template <typename Tp, typename = void> struct difference_type_impl {
    using difference_type = std::ptrdiff_t;
  };

  template <typename Tp>
  struct difference_type_impl<Tp, typename Ptr::difference_type> {
    using difference_type = typename Ptr::difference_type;
  };

  class undefined;

  template <typename Tp,typename Up,typename = void>
  struct rebind_type{};

  template<typename Tp,typename Up>
  struct rebind_type<Tp, Up,std::void_t<typename Tp::template rebind<Up>>>{
    using rebind = typename Tp::template rebind<Up>;
  };



public:
  using pointer = Ptr;
  using element_type = typename element_type_impl<Ptr>::element_type;
  using difference_type = typename difference_type_impl<Ptr>::difference_type;

  template<typename U>
  using rebind = typename rebind_type<Ptr, U>::type;

  static pointer pointer_to(element_type &r) { return Ptr::pointer_to(r); }
};  //template <typename Ptr> struct pointer_traits 

template <typename T> struct pointer_traits<T *> {
  using pointer = T *;
  using element_type = T;
  using difference_type = std::ptrdiff_t;

  template <typename U> using rebind = U *;

  static pointer pointer_to(element_type &r) noexcept {
    return std::addressof(r);
  }
};  //template <typename T> struct pointer_traits<T *>

} // namespace hy

#endif // POINTER_TRAITS_HPP_