#ifndef HY_ITERATOR_TRAITS_HPP_
#define HY_ITERATOR_TRAITS_HPP_

#include <cstddef>
#include <cstdio>
#include <type_traits>
namespace hy {
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};
struct contiguous_iterator_tag : public random_access_iterator_tag {};

template <typename Iter, typename = void> struct iterator_traits {};

template <typename Iter>
struct iterator_traits<
    Iter, std::void_t<typename Iter::difference_type, typename Iter::value_type,
                      typename Iter::pointer, typename Iter::reference,
                      typename Iter::iterator_category>> {
  using difference_type = typename Iter::difference_type;
  using value_type = typename Iter::value_type;
  using pointer = typename Iter::pointer;
  using reference = typename Iter::reference;
  using iterator_category = typename Iter::iterator_category;
};

template <class T> struct iterator_traits<T *, void> {
  using difference_type = typename std::ptrdiff_t;
  using value_type = T;
  using pointer = T *;
  using reference = T &;
  using iterator_category = hy::random_access_iterator_tag;
};

template <class T> struct iterator_traits<const T *, void> {
  using difference_type = typename std::ptrdiff_t;
  using value_type = T;
  using pointer = const T *;
  using reference = const T &;
  using iterator_category = typename hy::random_access_iterator_tag;
};

} // namespace hy

#endif // HY_ITERATOR_TRAITS_HPP_