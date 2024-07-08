#ifndef SCOPED_ALLOCATOR_HPP_
#define SCOPED_ALLOCATOR_HPP_
#include "Allocator_traits.hpp"
namespace hy {
/*forward declear*/
template <typename, typename...> class scoped_allocator_adaptor;

/*detail*/
namespace detail {
template <typename, typename... InnerAllocs>
struct scoped_allocator_adaptor_inner_allocator_type {
  static_assert(sizeof...(InnerAllocs) != 0, "Parameter pack cannot be zero");
  using type = scoped_allocator_adaptor<InnerAllocs...>;
}; /// class scoped_allocator_adaptor_inner_allocator_type

template <typename OuterAlloc>
struct scoped_allocator_adaptor_inner_allocator_type<OuterAlloc> {
  using type = scoped_allocator_adaptor<OuterAlloc>;
}; /// class scoped_allocator_adaptor_inner_allocator_type
} // namespace detail

/*class scoped_allocator_adaptor*/
template <typename OuterAlloc, typename... InnerAllocs>
class scoped_allocator_adaptor : public OuterAlloc {
public:
  /*Member types*/
  using outer_allocator_type = OuterAlloc;
  using inner_allocator_type =
      typename hy::detail::scoped_allocator_adaptor_inner_allocator_type<
          OuterAlloc, InnerAllocs...>::type;
  using value_type = typename hy::allocator_traits<OuterAlloc>::value_type;
  using size_type = typename std::allocator_traits<OuterAlloc>::size_type;
  using difference_type =
      typename std::allocator_traits<OuterAlloc>::difference_type;
  using pointer = typename std::allocator_traits<OuterAlloc>::pointer;
  using const_pointer =
      typename std::allocator_traits<OuterAlloc>::const_pointer;
  using void_pointer = typename std::allocator_traits<OuterAlloc>::void_pointer;
  using const_void_pointer =
      typename std::allocator_traits<OuterAlloc>::const_void_pointer;

  /*Member functions*/
  /*Custructors*/
  scoped_allocator_adaptor();

  /*Destructor*/
  ~scoped_allocator_adaptor() = default;

  /*Assignment*/
  scoped_allocator_adaptor &
  operator=(const scoped_allocator_adaptor &other) = default;
  scoped_allocator_adaptor &
  operator=(scoped_allocator_adaptor &&other) = default;

  /*Functions*/
  inner_allocator_type &inner_allocator() noexcept {}

  const inner_allocator_type &inner_allocator() const noexcept {}












  scoped_allocator_adaptor select_on_container_copy_construction() const{
    return ;
  }


  private:
  hy::scoped_allocator_adaptor<InnerAllocs...> scoped_allocator_adaptor_;

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