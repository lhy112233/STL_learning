#ifndef HY_ALLOCATOR_ARG_HPP_
#define HY_ALLOCATOR_ARG_HPP_

namespace hy {
struct allocator_arg_t {
  explicit allocator_arg_t() = default;
};

inline constexpr hy::allocator_arg_t allocator_arg = hy::allocator_arg_t();
} // namespace hy

#endif /// HY_ALLOCATOR_ARG_HPP_