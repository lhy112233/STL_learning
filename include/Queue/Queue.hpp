#ifndef HY_QUEUE_HPP_
#define HY_QUEUE_HPP_

#include <deque>
#include <type_traits>

namespace hy {

template <typename T, typename Container = std::deque<T>> class queue {
  /*static_assert*/
public:
  /*Member types*/
  using container_type = Container;
  using value_type = typename Container::value_type;
  using size_type = typename Container::size_type;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;

  /*Member objects*/
  /*Constructors*/
  queue() : queue(Container()) {}
  explicit queue(const Container &cont) : cont_{cont} {}
  explicit queue(Container &&cont) : cont_{std::move(cont)} {}
  queue(const queue &other) = default;
  queue(const queue &&other) = default;

  template <class Alloc, typename = std::enable_if_t<
                             hy::uses_allocator<Container, Alloc>::value>>
  explicit queue(const Alloc &alloc) : cont_{alloc} {}

  template <class Alloc, typename = std::enable_if_t<
                             hy::uses_allocator<Container, Alloc>::value>>
  queue(const Container &cont, const Alloc &alloc) : cont_{cont, alloc} {}

  template <class Alloc, typename = std::enable_if_t<
                             hy::uses_allocator<Container, Alloc>::value>>
  queue(Container &&cont, const Alloc &alloc) : cont_{std::move(cont), alloc} {}

  template <class Alloc, typename = std::enable_if_t<
                             hy::uses_allocator<Container, Alloc>::value>>
  queue(const queue &other, const Alloc &alloc) : cont_{other.cont_, alloc} {}

  template <class Alloc, typename = std::enable_if_t<
                             hy::uses_allocator<Container, Alloc>::value>>
  queue(queue &&other, const Alloc &alloc)
      : cont_{std::move(other.cont_), alloc} {}

  /*Destructor)*/
  ~queue() = default;

  /*Assignment*/
  queue &operator=(const queue &other) = default;

  queue &operator=(queue &&other) = default;

  /*Element access*/
  reference front() { return cont_.front(); }

  const_reference front() const { cont_.front(); }

  reference back() { return cont_.back(); }

  const_reference back() const { return cont_.back(); }

  /*Capacity*/
  bool empty() const { return cont_.empty(); }

  size_type size() const { return cont_.size(); }

  /*Modifiers*/
  void push(const value_type &value) { cont_.push_back(value); }

  void push(value_type &&value) { cont_.push_back(std::move(value)); }

  template <class... Args> decltype(auto) emplace(Args &&...args) {
    return cont_.emplace_back(std::forward<Args>(args)...);
  }

  void pop() { cont_.pop_front(); }

  void swap(queue &other) noexcept(std::is_nothrow_swappable_v<Container>) {
    using std::swap;
    swap(cont_.other.cont_);
  }

private:
  Container cont_;

}; /// class queue

/*Non-member functions*/
template <class T, class Container>
bool operator==(const hy::queue<T, Container> &lhs,
                const hy::queue<T, Container> &rhs) {
  return lhs.cont_ == rhs.cont_;
}

template <class T, class Container>
bool operator!=(const hy::queue<T, Container> &lhs,
                const hy::queue<T, Container> &rhs) {
  return !(lhs == rhs);
}

template <class T, class Container>
bool operator<(const hy::queue<T, Container> &lhs,
               const hy::queue<T, Container> &rhs) {
  return lhs.cont_ < rhs.cont_;
}

template <class T, class Container>
bool operator<=(const hy::queue<T, Container> &lhs,
                const hy::queue<T, Container> &rhs) {
  return lhs.cont_ <= rhs.cont_;
}

template <class T, class Container>
bool operator>(const hy::queue<T, Container> &lhs,
               const hy::queue<T, Container> &rhs) {
  return lhs.cont_ > rhs.cont_;
}

template <class T, class Container>
bool operator>=(const hy::queue<T, Container> &lhs,
                const hy::queue<T, Container> &rhs) {
  return lhs.cont_ >= rhs.cont_;
}

template <typename T, typename Container,
          typename = std::enable_if_t<std::is_swappable_v<Container>>>
void swap(hy::queue<T, Container> &lhs,
          hy::queue<T, Container> &rhs) noexcept(noexcept(lhs.swap(rhs))) {
  lhs.swap(rhs);
}

/*Helper classes*/
template< class T, class Container, class Alloc >
struct uses_allocator<hy::queue<T, Container>, Alloc>
    : std::uses_allocator<Container, Alloc>::type {};

} // namespace hy

#endif /// HY_QUEUE_HPP_