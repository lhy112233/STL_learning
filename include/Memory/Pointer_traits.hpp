#ifndef POINTER_TRAITS_HPP_
#define POINTER_TRAITS_HPP_
#include <cstddef>
#include <memory>
#include <type_traits>

namespace hy {
template <typename Ptr> struct pointer_traits {
private:
  template <typename Tp, typename = void> struct difference_type_impl {
    using difference_type = std::ptrdiff_t;
  };

  template <typename Tp>
  struct difference_type_impl<Tp, typename Ptr::difference_type> {
    using difference_type = typename Ptr::difference_type;
  };

public:
  using pointer = Ptr;
  // using element_type = ;
  using difference_type = typename difference_type_impl<Ptr>::difference_type;



  static pointer pointer_to(element_type &r) { return Ptr::pointer_to(r); }
};

template <typename T> struct pointer_traits<T *> {
  using pointer = T *;
  using element_type = T;
  using difference_type = std::ptrdiff_t;

  template <typename U> using rebind = U *;

  static pointer pointer_to(element_type &r) noexcept {
    return std::addressof(r);
  }
};

} // namespace hy

#endif // POINTER_TRAITS_HPP_