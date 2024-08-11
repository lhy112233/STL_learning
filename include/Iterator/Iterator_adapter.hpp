#ifndef HY_ITERATOR_ADAPTER_HPP_
#define HY_ITERATOR_ADAPTER_HPP_
#include "Iterator_traits.hpp"
#include <memory>

namespace hy {
template <class Iter> class reverse_iterator {
public:
  using iterator_type = Iter;
  using iterator_category =
      typename hy::iterator_traits<Iter>::iterator_category;
  using value_type = typename hy::iterator_traits<Iter>::value_type;
  using difference_type = typename hy::iterator_traits<Iter>::difference_type;
  using pointer = typename hy::iterator_traits<Iter>::pointer;
  using reference = typename hy::iterator_traits<Iter>::reference;

  /*Constructors*/
  constexpr reverse_iterator() : current_{} {}
  constexpr explicit reverse_iterator(iterator_type x) : current_{x} {}
  template <class U>
  constexpr reverse_iterator(const reverse_iterator<U> &other)
      : current_{other.current_} {}

/*Assignments*/
  template <class U>
  constexpr reverse_iterator &operator=(const reverse_iterator<U> &other) {
    current_ = other.base();
    return *this;
  }

/*Getters*/
  constexpr Iter base() const { return current_; }

  constexpr reference operator*() const {
    Iter tmp = current_;
    return *--tmp;
  }

  constexpr pointer operator->() const { return std::addressof(operator*()); }

  constexpr reference operator[](difference_type n) const {
    return base()[-n - 1];
  }

/*Setters*/
  constexpr reverse_iterator &operator++() {
    --current_;
    return *this;
  }

  constexpr reverse_iterator &operator--() {
    ++current_;
    return *this;
  }

  constexpr reverse_iterator operator++(int) {
    auto ret = *this;
    --current_;
    return ret;
  }

  constexpr reverse_iterator operator--(int) {
    auto ret = *this;
    ++current_;
    return ret;
  }

  constexpr reverse_iterator operator+(difference_type n) const {
    current_ -= n;
    return reverse_iterator(base() - n);
  }

  constexpr reverse_iterator operator-(difference_type n) const {
    current_ += n;
    return reverse_iterator(base() + n);
  }

  constexpr reverse_iterator &operator+=(difference_type n) {
    current_ -= n;
    return *this;
  }

  constexpr reverse_iterator &operator-=(difference_type n) {
    current_ += n;
    return *this;
  }

protected:
  Iter current_;
};

/*Non member functions*/
template< class Iterator1, class Iterator2 >
constexpr bool operator==( const hy::reverse_iterator<Iterator1>& lhs,
                           const hy::reverse_iterator<Iterator2>& rhs ){
                            return lhs.base() == rhs.base();
                           }

template< class Iterator1, class Iterator2 >
constexpr bool operator!=( const hy::reverse_iterator<Iterator1>& lhs,
                           const hy::reverse_iterator<Iterator2>& rhs ){
                            return !(lhs == rhs);
                           }

template< class Iterator1, class Iterator2 >
constexpr bool operator<( const hy::reverse_iterator<Iterator1>& lhs,
                          const hy::reverse_iterator<Iterator2>& rhs ){
                            return lhs < rhs;
                          }

template< class Iterator1, class Iterator2 >
constexpr bool operator<=( const hy::reverse_iterator<Iterator1>& lhs,
                           const hy::reverse_iterator<Iterator2>& rhs ){
                            return (lhs < rhs) || (lhs == rhs);
                           }

template< class Iterator1, class Iterator2 >
constexpr bool operator>( const hy::reverse_iterator<Iterator1>& lhs,
                          const hy::reverse_iterator<Iterator2>& rhs ){
                            return !(lhs <= rhs);
                          }

template< class Iterator1, class Iterator2 >
constexpr bool operator>=( const hy::reverse_iterator<Iterator1>& lhs,
                           const hy::reverse_iterator<Iterator2>& rhs ){
                            return !(lhs < rhs);
                           }

template< class Iter >
constexpr reverse_iterator<Iter>
    operator+( typename reverse_iterator<Iter>::difference_type n,
               const reverse_iterator<Iter>& it ){
                return reverse_iterator<Iter>(it.base() - n);
               }

template< class Iterator1, class Iterator2 >
constexpr auto operator-( const reverse_iterator<Iterator1>& lhs,
                          const reverse_iterator<Iterator2>& rhs
                        ) -> decltype(rhs.base() - lhs.base()){
                            return rhs.base() - lhs.base();
                        }

template< class Iter >
constexpr hy::reverse_iterator<Iter> make_reverse_iterator( Iter i ){
    return hy::reverse_iterator<Iter>{i};
}


} // namespace hy

#endif // HY_ITERATOR_ADAPTER_HPP_