#ifndef SCOPED_ALLOCATOR_HPP_
#define SCOPED_ALLOCATOR_HPP_
#include "Allocator_traits.hpp"
#include "Type_traits.hpp"
#include <algorithm>
#include <type_traits>
namespace hy {
/*class scoped_allocator_adaptor*/
template <typename OuterAlloc, typename... InnerAllocs>
class scoped_allocator_adaptor : public OuterAlloc {
public:
  /*Member types*/
  using outer_allocator_type = OuterAlloc;
  using inner_allocator_type = scoped_allocator_adaptor<InnerAllocs...>;
  using value_type = typename hy::allocator_traits<OuterAlloc>::value_type;
  using size_type = typename hy::allocator_traits<OuterAlloc>::size_type;
  using difference_type =
      typename hy::allocator_traits<OuterAlloc>::difference_type;
  using pointer = typename hy::allocator_traits<OuterAlloc>::pointer;
  using const_pointer =
      typename hy::allocator_traits<OuterAlloc>::const_pointer;
  using void_pointer = typename hy::allocator_traits<OuterAlloc>::void_pointer;
  using const_void_pointer =
      typename hy::allocator_traits<OuterAlloc>::const_void_pointer;
  using propagate_on_container_copy_assignment = std::integral_constant<
      bool, std::disjunction_v<typename hy::allocator_traits<OuterAlloc>::
                                   propagate_on_container_copy_assignment,
                               typename hy::allocator_traits<InnerAllocs>::
                                   propagate_on_container_copy_assignment...>>;
  using propagate_on_container_move_assignment = std::integral_constant<
      bool, std::disjunction_v<typename hy::allocator_traits<OuterAlloc>::
                                   propagate_on_container_move_assignment,
                               typename hy::allocator_traits<InnerAllocs>::
                                   propagate_on_container_move_assignment...>>;
  using propagate_on_container_swap = std::integral_constant<
      bool, std::disjunction_v<typename hy::allocator_traits<
                                   OuterAlloc>::propagate_on_container_swap,
                               typename hy::allocator_traits<InnerAllocs>::
                                   propagate_on_container_swap...>>;
  using is_always_equal = std::integral_constant<
      bool,
      std::conjunction_v<
          typename hy::allocator_traits<OuterAlloc>::is_always_equal,
          typename hy::allocator_traits<InnerAllocs>::is_always_equal...>>;

  template <class T> struct rebind {
    typedef scoped_allocator_adaptor<
        typename hy::allocator_traits<OuterAlloc>::template rebind_alloc<T>,
        InnerAllocs...>
        other;
  };

  /*Member functions*/
  /*Custructors*/
  scoped_allocator_adaptor() : OuterAlloc{}, scoped_allocator_adaptor_{} {};

  template <class OuterA2, typename = std::enable_if_t<std::is_constructible<
                               OuterAlloc, OuterA2>::value>>
  scoped_allocator_adaptor(OuterA2 &&outerAlloc,
                           const InnerAllocs &...innerAllocs) noexcept
      : OuterAlloc{std::forward<OuterA2>(outerAlloc)},
        scoped_allocator_adaptor_{
            hy::allocator_traits<InnerAllocs>::
                select_on_container_copy_construction(innerAllocs)...} {}

  scoped_allocator_adaptor(const scoped_allocator_adaptor &other) noexcept
      : OuterAlloc{hy::allocator_traits<OuterAlloc>::allocator_traits(other)},
        scoped_allocator_adaptor_{other.inner_allocator()} {}

  scoped_allocator_adaptor(scoped_allocator_adaptor &&other) noexcept = default;

  template <class OuterA2>
  scoped_allocator_adaptor(
      const scoped_allocator_adaptor<OuterA2, InnerAllocs...> &other) noexcept
      : OuterAlloc{other},
        scoped_allocator_adaptor_{other.scoped_allocator_adaptor_} {}

  template <class OuterA2>
  scoped_allocator_adaptor(
      scoped_allocator_adaptor<OuterA2, InnerAllocs...> &&other) noexcept
      : OuterAlloc{std::move(other)},
        scoped_allocator_adaptor_{std::move(other.scoped_allocator_adaptor_)} {}

  /*Destructor*/
  ~scoped_allocator_adaptor() = default;

  /*Assignment*/
  scoped_allocator_adaptor &
  operator=(const scoped_allocator_adaptor &other) = default;
  scoped_allocator_adaptor &
  operator=(scoped_allocator_adaptor &&other) = default;

  /*Functions*/
  inner_allocator_type &inner_allocator() noexcept {
    return scoped_allocator_adaptor_;
  }

  const inner_allocator_type &inner_allocator() const noexcept {
    return scoped_allocator_adaptor_;
  }

  outer_allocator_type &outer_allocator() noexcept {
    return static_cast<OuterAlloc &>(*this);
  }

  const outer_allocator_type &outer_allocator() const noexcept {
    return static_cast<const OuterAlloc &>(*this);
  }

  pointer allocate(size_type n) {
    return hy::allocator_traits<OuterAlloc>::allocate(outer_allocator(), n);
  }

  pointer allocate(size_type n, const_void_pointer hint) {
    return hy::allocator_traits<OuterAlloc>::allocate(outer_allocator(), n,
                                                      hint);
  }

  void deallocate(pointer p, size_type n) noexcept {
    hy::allocator_traits<OuterAlloc>::deallocate(outer_allocator(), p, n);
  }

  size_type max_size() const {
    return hy::allocator_traits<OuterAlloc>::max_size(outer_allocator());
  }

  template <class T, class... Args> void construct(T *p, Args &&...args) {
    /*TODO*/
  }

  template <class T> void destroy(T *p) { /*TODO*/ }

  scoped_allocator_adaptor select_on_container_copy_construction() const {
    return scoped_allocator_adaptor(*this);
  }

private:
  hy::scoped_allocator_adaptor<InnerAllocs...> scoped_allocator_adaptor_;
}; /// class scoped_allocator_adaptor

template <typename OuterAlloc>
class scoped_allocator_adaptor<OuterAlloc> : public OuterAlloc {
public:
  /*Member types*/
  using outer_allocator_type = OuterAlloc;
  using inner_allocator_type = scoped_allocator_adaptor<OuterAlloc>;
  using value_type = typename hy::allocator_traits<OuterAlloc>::value_type;
  using size_type = typename hy::allocator_traits<OuterAlloc>::size_type;
  using difference_type =
      typename hy::allocator_traits<OuterAlloc>::difference_type;
  using pointer = typename hy::allocator_traits<OuterAlloc>::pointer;
  using const_pointer =
      typename hy::allocator_traits<OuterAlloc>::const_pointer;
  using void_pointer = typename hy::allocator_traits<OuterAlloc>::void_pointer;
  using const_void_pointer =
      typename hy::allocator_traits<OuterAlloc>::const_void_pointer;
  using propagate_on_container_copy_assignment = std::integral_constant<
      bool, hy::allocator_traits<
                OuterAlloc>::propagate_on_container_copy_assignment::value>;
  using propagate_on_container_move_assignment = std::integral_constant<
      bool, hy::allocator_traits<
                OuterAlloc>::propagate_on_container_move_assignment::value>;
  using propagate_on_container_swap = std::integral_constant<
      bool,
      hy::allocator_traits<OuterAlloc>::propagate_on_container_swap::value>;
  using is_always_equal = std::integral_constant<
      bool, hy::allocator_traits<OuterAlloc>::is_always_equal::value>;
  template <class T> struct rebind {
    typedef scoped_allocator_adaptor<
        typename hy::allocator_traits<OuterAlloc>::template rebind_alloc<T>>
        other;
  };

  /*Member functions*/
  /*Constructors*/
  scoped_allocator_adaptor() : OuterAlloc{} {};
  template <class OuterA2, typename = std::enable_if_t<std::is_constructible<
                               OuterAlloc, OuterA2>::value>>
  scoped_allocator_adaptor(OuterA2 &&outerAlloc) noexcept
      : OuterAlloc{std::forward<OuterA2>(outerAlloc)} {}

  scoped_allocator_adaptor(const scoped_allocator_adaptor &other) noexcept
      : OuterAlloc{hy::allocator_traits<
            OuterAlloc>::select_on_container_copy_construction(other)} {}

  scoped_allocator_adaptor(scoped_allocator_adaptor &&other) noexcept = default;

  template <class OuterA2>
  scoped_allocator_adaptor(
      const hy::scoped_allocator_adaptor<OuterA2> &other) noexcept
      : OuterAlloc{other} {}

  template <class OuterA2>
  scoped_allocator_adaptor(
      hy::scoped_allocator_adaptor<OuterA2> &&other) noexcept
      : OuterAlloc{std::move(other)} {}

  /*Destructor*/
  ~scoped_allocator_adaptor() = default;

  /*Assignment*/
  scoped_allocator_adaptor &
  operator=(const scoped_allocator_adaptor &other) = default;
  scoped_allocator_adaptor &
  operator=(scoped_allocator_adaptor &&other) = default;

  /*Getter*/
  inner_allocator_type &inner_allocator() noexcept { return *this; }

  const inner_allocator_type &inner_allocator() const noexcept { return *this; }

  outer_allocator_type &outer_allocator() noexcept {
    return static_cast<OuterAlloc &>(*this);
  }

  const outer_allocator_type &outer_allocator() const noexcept {
    return static_cast<const OuterAlloc &>(*this);
  }

  /*Allocate*/
  pointer allocate(size_type n) {
    return hy::allocator_traits<OuterAlloc>::allocate(outer_allocator(), n);
  }

  pointer allocate(size_type n, const_void_pointer hint) {
    return hy::allocator_traits<OuterAlloc>::allocate(outer_allocator(), n,
                                                      hint);
  }

  /*Deallocate*/
  void deallocate(pointer p, size_type n) noexcept {
    hy::allocator_traits<OuterAlloc>::deallocate(outer_allocator(), p, n);
  }

  size_type max_size() const {
    hy::allocator_traits<OuterAlloc>::max_size(outer_allocator());
  }

  /*Construct*/

  /*Destroy*/
  //   template< class T >
  // void destroy( T* p ){
  //   std::allocator_traits<OUTERMOST>::destroy(OUTERMOST(*this), p);
  // }

  scoped_allocator_adaptor select_on_container_copy_construction() const {
    return hy::scoped_allocator_adaptor(*this);
  }

}; /// class scoped_allocator_adaptor

/*Non-member functions*/
template <class OuterAlloc1, class OuterAlloc2, class... InnerAllocs>
bool operator==(
    const scoped_allocator_adaptor<OuterAlloc1, InnerAllocs...> &lhs,
    const scoped_allocator_adaptor<OuterAlloc2, InnerAllocs...> &rhs) noexcept {
  return (lhs.outer_allocator() == rhs.outer_allocator()) &&
         ((sizeof...(InnerAllocs) > 0)
              ? (lhs.inner_allocator() == rhs.inner_allocator())
              : true);
}

template <class OuterAlloc1, class OuterAlloc2, class... InnerAllocs>
bool operator!=(
    const scoped_allocator_adaptor<OuterAlloc1, InnerAllocs...> &lhs,
    const scoped_allocator_adaptor<OuterAlloc2, InnerAllocs...> &rhs) noexcept {
  return !(lhs == rhs);
}

} // namespace hy

#endif /// SCOPED_ALLOCATOR_HPP_