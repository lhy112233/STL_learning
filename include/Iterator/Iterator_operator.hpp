#ifndef HY_ITERATOR_OPERATOR_HPP_
#define HY_ITERATOR_OPERATOR_HPP_
#include "Iterator_traits.hpp"
#include <type_traits>

namespace hy {
template <class It, class Distance> constexpr void advance(It &it, Distance n) {
  using category = typename hy::iterator_traits<It>::iterator_category;
  static_assert(std::is_base_of_v<hy::input_iterator_tag, category>);

  auto dist = typename hy::iterator_traits<It>::difference_type(n);
  if constexpr (std::is_base_of_v<hy::random_access_iterator_tag, category>)
    it += dist;
  else {
    while (dist > 0) {
      --dist;
      ++it;
    }
    if constexpr (std::is_base_of_v<hy::bidirectional_iterator_tag, category>)
      while (dist < 0) {
        ++dist;
        --it;
      }
  }
}

template <class It>
constexpr typename hy::iterator_traits<It>::difference_type distance(It first,
                                                                     It last) {
  using category = typename hy::iterator_traits<It>::iterator_category;
  static_assert(std::is_base_of_v<hy::input_iterator_tag, category>);

  if constexpr (std::is_base_of_v<hy::random_access_iterator_tag, category>)
    return last - first;
  else {
    typename hy::iterator_traits<It>::difference_type result = 0;
    while (first != last) {
      ++first;
      ++result;
    }
    return result;
  }
}

template <class InputIt>
constexpr // C++17 起
    InputIt
    next(InputIt it,
         typename hy::iterator_traits<InputIt>::difference_type n = 1) {
  hy::advance(it, n);
  return it;
}

template <class BidirIt>
constexpr BidirIt
prev(BidirIt it, typename hy::iterator_traits<BidirIt>::difference_type n = 1) {
  hy::advance(it, -n);
  return it;
}

} // namespace hy
#endif // HY_ITERATOR_OPERATOR_HPP_