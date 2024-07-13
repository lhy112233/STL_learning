#ifndef HY_POLYMORPHIC_ALLOCATOR_HPP_
#define HY_POLYMORPHIC_ALLOCATOR_HPP_
#include "Memory_resource.hpp"
#include <type_traits>
#include <utility>
namespace hy {

namespace detail {
template <typename U> struct is_pair final : std::false_type {};

template <typename First, typename Second>
struct is_pair<std::pair<First, Second>> final : std::true_type {};
} // namespace detail

namespace pmr {

template <class T> class polymorphic_allocator {
  /*Friends*/
  friend bool operator==(const polymorphic_allocator &lhs,
                         const polymorphic_allocator &rhs) noexcept;
  friend bool operator!=(const polymorphic_allocator &lhs,
                         const polymorphic_allocator &rhs) noexcept;

public:
  /*Member types*/
  using value_type = T;
  /*Member functions*/
  /*Constructors*/
  polymorphic_allocator() noexcept
      : p_memory_resource{hy::pmr::get_default_resource()} {}
  polymorphic_allocator(const polymorphic_allocator &other) : p_memory_resource{other.resource()} {}
  template <class U>
  polymorphic_allocator(const polymorphic_allocator<U> &other) noexcept
      : p_memory_resource{other.resource()} {}
  polymorphic_allocator(hy::pmr::memory_resource *r) : p_memory_resource{r} {}

  /*Destructor*/
  ~polymorphic_allocator() = default;

  /*Assignment*/
  polymorphic_allocator &operator=(polymorphic_allocator &) = delete;
  polymorphic_allocator &operator=(polymorphic_allocator &&) = delete;

  /*Allocate*/
  T *allocate(std::size_t n) {
    if (n > std::numeric_limits<std::size_t>::max() / sizeof(T)) {
      throw std::bad_array_new_length{};
    }
    return static_cast<T *>(resource()->allocate(n * sizeof(T), alignof(T)));
  }

  /*Deallocate*/
  void deallocate(T *p, std::size_t n) {
    this->resource()->deallocate(p, n * sizeof(T), alignof(T));
  }

  /*Construct*/
  template <class U, class... Args>
  auto construct(U *p, Args &&...args)
      -> std::enable_if_t<!(::hy::detail::is_pair<U>::value)> {

      }

  

/*TODO*/










  /*Destry*/
  template <class U> void destroy(U *p) { p->~U(); }

  polymorphic_allocator select_on_container_copy_construction() const {
    return hy::pmr::polymorphic_allocator<T>{};
  }

  hy::pmr::memory_resource *resource() const { return p_memory_resource; }

private:
  /*Datas*/
  hy::pmr::memory_resource *p_memory_resource;
};

/*Non-Member functions*/
template <class T1, class T2>
bool operator==(const hy::pmr::polymorphic_allocator<T1> &lhs,
                const hy::pmr::polymorphic_allocator<T2> &rhs) noexcept {
  return *lhs.resource() == *rhs.resource();
}

template <class T1, class T2>
bool operator!=(const hy::pmr::polymorphic_allocator<T1> &lhs,
                const hy::pmr::polymorphic_allocator<T2> &rhs) noexcept {
  return !(lhs == rhs);
}
} // namespace pmr

} // namespace hy

#endif /// HY_POLYMORPHIC_ALLOCATOR_HPP_