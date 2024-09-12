#ifndef HY_UNINITIALIZED_COPY_HPP_
#define HY_UNINITIALIZED_COPY_HPP_
#include <execution>
#include <memory.h>
#include <type_traits>
namespace hy {

template <typename InputIt, typename NoThrowForwardIt>
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

template <typename ExecutionPolicy, typename ForwardIt, typename NoThrowForwardIt,
          typename = std::enable_if_t<
              std::is_execution_policy_v<std::decay_t<ExecutionPolicy>>>>
inline NoThrowForwardIt uninitialized_copy(ExecutionPolicy &&policy,
                                           ForwardIt first, ForwardIt last,
                                           NoThrowForwardIt d_first) {
  /*TODO*/
} /// uninitialized_copy

template <typename InputIt, typename Size, typename NoThrowForwardIt>
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
} /// uninitialized_copy_n

template <typename ExecutionPolicy, typename ForwardIt, typename Size,
          typename NoThrowForwardIt,
          typename = std::enable_if_t<
              std::is_execution_policy_v<std::decay_t<ExecutionPolicy>>>>
inline NoThrowForwardIt uninitialized_copy_n(ExecutionPolicy &&policy,
                                             ForwardIt first, Size count,
                                             NoThrowForwardIt d_first) {
  /*TODO*/
} /// uninitialized_copy_n

template <typename ForwardIt, typename T>
void uninitialized_fill(ForwardIt first, ForwardIt last, const T &value) {
  using V = typename std::iterator_traits<ForwardIt>::value_type;
  ForwardIt current = first;
  try {
    for (; current != last; ++current)
      ::new (static_cast<void *>(std::addressof(*current))) V(value);
  } catch (...) {
    for (; first != current; ++first)
      first->~V();
    throw;
  }
}

template <typename ExecutionPolicy, typename ForwardIt, typename T>
void uninitialized_fill(ExecutionPolicy &&policy, ForwardIt first,
                        ForwardIt last, const T &value) {
  /*TODO*/
}

template <typename ForwardIt, typename Size, typename T>
ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T &value) {
  using V = typename std::iterator_traits<ForwardIt>::value_type;
  ForwardIt current = first;
  try {
    for (; count > 0; ++current, (void)--count)
      ::new (static_cast<void *>(std::addressof(*current))) V(value);
    return current;
  } catch (...) {
    for (; first != current; ++first)
      first->~V();
    throw;
  }
}

template <typename ExecutionPolicy, typename ForwardIt, typename Size, typename T>
ForwardIt uninitialized_fill_n(ExecutionPolicy &&policy, ForwardIt first,
                               Size count, const T &value) {
  /*TODO*/
}

template <typename InputIt, typename NoThrowForwardIt>
NoThrowForwardIt uninitialized_move(InputIt first, InputIt last,
                                    NoThrowForwardIt d_first) {
  using Value = typename std::iterator_traits<NoThrowForwardIt>::value_type;
  NoThrowForwardIt current = d_first;
  try {
    for (; first != last; ++first, (void)++current)
      ::new (static_cast<void *>(std::addressof(*current)))
          Value(std::move(*first));
    return current;
  } catch (...) {
    std::destroy(d_first, current);
    throw;
  }
}

template <typename ExecutionPolicy, typename ForwardIt, typename NoThrowForwardIt>
NoThrowForwardIt uninitialized_move(ExecutionPolicy &&policy, ForwardIt first,
                                    ForwardIt last, NoThrowForwardIt d_first) {
  /*TODO*/
}

template <typename InputIt, typename Size, typename NoThrowForwardIt>
std::pair<InputIt, NoThrowForwardIt>
uninitialized_move_n(InputIt first, Size count, NoThrowForwardIt d_first) {
  using Value = typename std::iterator_traits<NoThrowForwardIt>::value_type;
  NoThrowForwardIt current = d_first;
  try {
    for (; count > 0; ++first, (void)++current, --count)
      ::new (static_cast<void *>(std::addressof(*current)))
          Value(std::move(*first));
  } catch (...) {
    std::destroy(d_first, current);
    throw;
  }
  return {first, current};
}

template <typename ExecutionPolicy, typename ForwardIt, typename Size,
          typename NoThrowForwardIt>
std::pair<ForwardIt, NoThrowForwardIt>
uninitialized_move_n(ExecutionPolicy &&policy, ForwardIt first, Size count,
                     NoThrowForwardIt d_first) {
  /*TODO*/
}

template <typename ForwardIt>
void uninitialized_default_construct(ForwardIt first, ForwardIt last) {
  using Value = typename std::iterator_traits<ForwardIt>::value_type;
  ForwardIt current = first;
  try {
    for (; current != last; ++current) {
      ::new (const_cast<void *>(
          static_cast<const volatile void *>(std::addressof(*current)))) Value;
    }
  } catch (...) {
    std::destroy(first, current);
    throw;
  }
}

template <typename ExecutionPolicy, typename ForwardIt>
void uninitialized_default_construct(ExecutionPolicy &&policy, ForwardIt first,
                                     ForwardIt last) {
  /*TODO*/
}

template <typename ForwardIt, typename Size>
ForwardIt uninitialized_default_construct_n(ForwardIt first, Size n) {
  using T = typename std::iterator_traits<ForwardIt>::value_type;
  ForwardIt current = first;

  try {
    for (; n > 0; (void)++current, --n)
      ::new (const_cast<void *>(
          static_cast<const volatile void *>(std::addressof(*current)))) T;
    return current;
  } catch (...) {
    std::destroy(first, current);
    throw;
  }
}

template <typename ExecutionPolicy, typename ForwardIt, typename Size>
ForwardIt uninitialized_default_construct_n(ExecutionPolicy &&policy,
                                            ForwardIt first, Size n) {
  /*TODO*/
}

template <typename ForwardIt>
void uninitialized_value_construct(ForwardIt first, ForwardIt last) {
  using Value = typename std::iterator_traits<ForwardIt>::value_type;
  ForwardIt current = first;
  try {
    for (; current != last; ++current)
      ::new (const_cast<void *>(static_cast<const volatile void *>(
          std::addressof(*current)))) Value();
  } catch (...) {
    std::destroy(first, current);
    throw;
  }
}

template <typename ExecutionPolicy, typename ForwardIt>
void uninitialized_value_construct(ExecutionPolicy &&policy, ForwardIt first,
                                   ForwardIt last) {
  /*TODO*/
}

template <typename ForwardIt, typename Size>
ForwardIt uninitialized_value_construct_n(ForwardIt first, Size n) {
  using T = typename std::iterator_traits<ForwardIt>::value_type;
  ForwardIt current = first;
  try {
    for (; n > 0; (void)++current, --n)
      ::new (const_cast<void *>(
          static_cast<const volatile void *>(std::addressof(*current)))) T();
    return current;
  } catch (...) {
    std::destroy(first, current);
    throw;
  }
}

template <typename ExecutionPolicy, typename ForwardIt, typename Size>
ForwardIt uninitialized_value_construct_n(ExecutionPolicy &&policy,
                                          ForwardIt first, Size n) {
  /*TODO*/
}

template <typename T> void destroy_at(T *p) { p->~T(); }

template <typename ForwardIt> void destroy(ForwardIt first, ForwardIt last) {
  for (; first != last; ++first)
    std::destroy_at(std::addressof(*first));
}

template <typename ExecutionPolicy, typename ForwardIt>
void destroy(ExecutionPolicy &&policy, ForwardIt first, ForwardIt last) {
  /*TODO*/
}

template <typename ForwardIt, typename Size>
ForwardIt destroy_n(ForwardIt first, Size n) {
  for (; n > 0; (void)++first, --n)
    std::destroy_at(std::addressof(*first));
  return first;
}

template <typename ExecutionPolicy, typename ForwardIt, typename Size>
ForwardIt destroy_n(ExecutionPolicy &&policy, ForwardIt first, Size n) {
  /*TODO*/
}

} // namespace hy

#endif /// HY_UNINITIALIZED_COPY_HPP_