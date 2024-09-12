#ifndef HY_GET_TEMPORARY_BUFFER_HPP_
#define HY_GET_TEMPORARY_BUFFER_HPP_
#include <new>
#include <utility>
namespace hy {
template <class T>
[[deprecated]] std::pair<T *, std::ptrdiff_t>
get_temporary_buffer(std::ptrdiff_t count) noexcept {
  if (count <= 0) {
    return {nullptr, 0};
  } else {
    return {static_cast<T *>(::operator new(count * sizeof(T), alignof(T))), count};
  }
}
} // namespace hy

#endif /// HY_GET_TEMPORARY_BUFFER_HPP_