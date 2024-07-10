#ifndef HY_RETURN_TEMPORARY_BUFFER_HPP_
#define HY_RETURN_TEMPORARY_BUFFER_HPP_

#include <new>
namespace hy {
template <class T> [[deprecated]] void return_temporary_buffer(T *p) {
  ::operator delete(static_cast<void *>(p));
}
} // namespace hy

#endif /// HY_RETURN_TEMPORARY_BUFFER_HPP_