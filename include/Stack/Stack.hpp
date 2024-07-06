#ifndef HY_STACK_HPP_
#define HY_STACK_HPP_
#include "Uses_allocator"
#include <algorithm>
#include <deque>
#include <type_traits>
#include <utility>


namespace hy {
template <typename T, typename Container = std::deque<T>> class stack {
public:
  /*Member types*/
  using container_type = Container;
  using value_type = typename Container::value_type;
  using size_type = typename Container::size_type;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;

  /*Member functions*/
  /*Constructors*/
  stack() : stack(Container()) {} /// 委托构造
  explicit stack(const Container &cont) : cont_{cont} {}
  explicit stack(Container &&cont) : cont_{std::move(cont)} {}
  stack(const stack &other) = default;
  stack(stack &&other) = default;

  template <typename Alloc, typename = std::enable_if_t<
                                std::uses_allocator<Container, Alloc>::value>>
  explicit stack(const Alloc &alloc) : cont_{alloc} {}

  template <class Alloc, typename = std::enable_if_t<
                             std::uses_allocator<Container, Alloc>::value>>
  stack(const Container &cont, const Alloc &alloc) : cont_{cont, alloc} {}

  template <class Alloc, typename = std::enable_if_t<
                             std::uses_allocator<Container, Alloc>::value>>
  stack(Container &&cont, const Alloc &alloc) : cont_{std::move(cont), alloc} {}

  template <class Alloc, typename = std::enable_if_t<
                             std::uses_allocator<Container, Alloc>::value>>
  stack(const stack &other, const Alloc &alloc) : cont_{other.cont_, alloc} {}

  template <class Alloc, typename = std::enable_if_t<
                             std::uses_allocator<Container, Alloc>::value>>
  stack(stack &&other, const Alloc &alloc)
      : cont_{std::move(other.cont_), alloc} {}

  /*Destructor*/
  ~stack() = default;

  /*Assignmen*/
  stack &operator=(const stack &other) = default;
  stack &operator=(stack &&other) = default;

  /*Element access*/
  reference top() { return cont_.back(); }

  const_reference top() const { return cont_.back(); }

  /*Capacity*/
  bool empty() const { return cont_.empty(); }

  size_type size() const { return cont_.size(); }

  /*Modifiers*/
  void push(const value_type &value) { cont_.push_back(value); }

  void push(value_type &&value) { cont_.push_back(std::move(value)); }

  template <typename... Args> decltype(auto) emplace(Args &&...args) {
    return cont_.emplace_back(std::forward<Args>(args)...);
  }

  void pop() { cont_.pop_back(); }

  void swap(stack &other) noexcept(std::is_nothrow_swappable_v<Container>) {
    using std::swap;
    swap(cont_, other.cont_);
  }

private:
  Container cont_;

}; /// class stack

template <class T, class Container>
bool operator==(const hy::stack<T, Container> &lhs,
                const hy::stack<T, Container> &rhs) {
  lhs.cont_ == rhs.cont_;
}

template <class T, class Container>
bool operator!=(const hy::stack<T, Container> &lhs,
                const hy::stack<T, Container> &rhs) {
  return !lhs == rhs;
}

template <class T, class Container>
bool operator<(const hy::stack<T, Container> &lhs,
               const hy::stack<T, Container> &rhs) {
  return lhs < rhs;
}

template <class T, class Container>
bool operator<=(const hy::stack<T, Container> &lhs,
                const hy::stack<T, Container> &rhs) {
  return lhs <= rhs;
}

template <class T, class Container>
bool operator>(const hy::stack<T, Container> &lhs,
               const hy::stack<T, Container> &rhs) {
  return lhs > rhs;
}

template <class T, class Container>
bool operator>=(const hy::stack<T, Container> &lhs,
                const hy::stack<T, Container> &rhs) {
  return lhs >= rhs;
}

template <class T, class Container,
          typename = std::enable_if_t<std::is_swappable_v<Container>>>
void swap(hy::stack<T, Container> &lhs,
          hy::stack<T, Container> &rhs) noexcept(noexcept(lhs.swap(rhs))) {
  return lhs.swap(rhs);
}

template <class T, class Container, class Alloc>
struct uses_allocator<hy::stack<T, Container>, Alloc>
    : std::uses_allocator<Container, Alloc>::type {};

/*TODO:推导指引*/

} // namespace hy

#endif /// HY_STACK_HPP_