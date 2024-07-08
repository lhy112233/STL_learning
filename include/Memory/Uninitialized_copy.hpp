#ifndef HY_UNINITIALIZED_COPY_HPP_
#define HY_UNINITIALIZED_COPY_HPP_
#include <execution>
#include <memory.h>
#include <type_traits>
namespace hy {

template <class InputIt, class NoThrowForwardIt>
inline NoThrowForwardIt uninitialized_copy(InputIt first, InputIt last,
                                           NoThrowForwardIt d_first) {
  using T = typename std::iterator_traits<NoThrowForwardIt>::value_type;
  NoThrowForwardIt current = d_first;
  try {
    for (; first != last; ++first, (void)++current)
      ::new (static_cast<void *>(std::addressof(*current))) T(*first);
    return current;
  } catch (...) {
    for (; d_first != current; ++d_first)
      d_first->~T();
    throw;
  }
} /// uninitialized_copy

template <class ExecutionPolicy, class ForwardIt, class NoThrowForwardIt,
          typename = std::enable_if_t<
              std::is_execution_policy_v<std::decay_t<ExecutionPolicy>>>>
inline NoThrowForwardIt uninitialized_copy(ExecutionPolicy &&policy,
                                           ForwardIt first, ForwardIt last,
                                           NoThrowForwardIt d_first) {
  /*TODO*/
} /// uninitialized_copy

template <class InputIt, class Size, class NoThrowForwardIt>
inline NoThrowForwardIt uninitialized_copy_n(InputIt first, Size count,
                                             NoThrowForwardIt d_first) {
  using T = typename std::iterator_traits<NoThrowForwardIt>::value_type;
  NoThrowForwardIt current = d_first;
  try {
    for (; count > 0; ++first, (void)++current, --count)
      ::new (static_cast<void *>(std::addressof(*current))) T(*first);
  } catch (...) {
    for (; d_first != current; ++d_first)
      d_first->~T();
    throw;
  }
  return current;
}   ///uninitialized_copy_n

template <class ExecutionPolicy, class ForwardIt, class Size,
          class NoThrowForwardIt,
          typename = std::enable_if_t<
              std::is_execution_policy_v<std::decay_t<ExecutionPolicy>>>>
inline NoThrowForwardIt uninitialized_copy_n(ExecutionPolicy &&policy,
                                             ForwardIt first, Size count,
                                             NoThrowForwardIt d_first) {
  /*TODO*/
}   ///uninitialized_copy_n



} // namespace hy

#endif /// HY_UNINITIALIZED_COPY_HPP_