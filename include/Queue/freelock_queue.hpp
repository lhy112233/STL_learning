#ifndef HY_FREELOCK_QUEUE_HPP_
#define HY_FREELOCK_QUEUE_HPP_
#include <atomic>
#include <concepts>
#include <cstddef>
#include <expected>
#include <memory>
#include <memory_resource>
#include <new>
#include <type_traits>
#include <utility>

namespace hy {
template <typename T, std::size_t N, typename Alloc = std::allocator<T>>
class FreelockQueue final : private Alloc {
  static_assert(N > 1, "N cannot be 0");
  static_assert(std::negation_v<std::is_reference<T>>,
                "value_type is unable to same with value_type&&");

  inline constexpr static auto REAL_CAPACITY = N + 1;

public:
  using value_type = T;
  using size_type = std::size_t;
  using allocator_type = Alloc;
  using allocator_traits = std::allocator_traits<Alloc>;

  FreelockQueue(const FreelockQueue &) = delete;
  FreelockQueue(FreelockQueue &&) = delete;
  FreelockQueue &operator=(const FreelockQueue &) = delete;
  FreelockQueue &operator=(FreelockQueue &&) = delete;

  constexpr FreelockQueue()
      : ring{allocator_traits::allocate(REAL_CAPACITY)}, read_index_{0},
        write_index_{0} {}

  constexpr ~FreelockQueue() noexcept(
      std::is_nothrow_destructible_v<value_type &>) {
    if constexpr (std::is_trivially_destructible_v<value_type &>) {
      auto read = read_index_.load(std::memory_order_acquire);
      auto write = write_index_.load(std::memory_order_acquire);
      while (read != write) {
        allocator_traits::destroy(*this, ring + read);
        ++read;
        if (read == REAL_CAPACITY) {
          read = 0;
        }
      }
    }
    allocator_traits::deallocate(ring, REAL_CAPACITY);
  }

  constexpr void push(auto &&...args) noexcept(noexcept(value_type{
      std::forward<decltype(args)>(args)...})) {
    auto current_write = write_index_.load(std::memory_order_relaxed);

    allocator_traits::construct(*this, ring + current_write,
                                std::forward<decltype(args)>(args)...);

    // 延迟修改，万一构造抛异常了就不需要执行此操作
    auto next_write = current_write + 1;
    if (next_write == REAL_CAPACITY) {
      next_write = 0;
    }
    while (next_write == read_index_.load(std::memory_order_acquire)) {
      read_index_.wait(next_write, std::memory_order_acquire);
    }

    write_index_.store(next_write, std::memory_order_release);
    write_index_.notify_one();
  }

  constexpr void pop(value_type &value) noexcept(
      std::disjunction_v<std::is_nothrow_move_assignable<value_type>,
                         std::is_nothrow_copy_assignable<value_type>>) {
    auto current_read = read_index_.load(std::memory_order_relaxed);
    while (current_read == write_index_.load(std::memory_order_relaxed)) {
      write_index_.wait(current_read, std::memory_order_acquire);
    }

    if constexpr (std::is_nothrow_move_assignable_v<value_type>) {
      value = std::move(ring[current_read]);
    } else {
      value = ring[current_read];
    }
    if constexpr (std::negation_v<std::is_trivially_destructible<value_type>>) {
      allocator_traits::destroy(*this, ring + current_read);
    }
    ++current_read;
    if (current_read == REAL_CAPACITY) {
      current_read = 0;
    }
    read_index_.store(current_read, std::memory_order_release);
    read_index_.notify_one();
  }

  [[nodiscard]] constexpr bool try_push(auto &&...args) noexcept(
      noexcept(value_type{std::forward<decltype(args)>(args)...})) {
    auto current_write = write_index_.load(std::memory_order_relaxed);
    auto next_write = current_write + 1;
    if (next_write == REAL_CAPACITY) {
      next_write = 0;
    }
    if (next_write == read_index_.load(std::memory_order_acquire)) {
      return false;
    }

    allocator_traits::construct(*this, ring + current_write,
                                std::forward<decltype(args)>(args)...);
    write_index_.store(next_write, std::memory_order_release);
    write_index_.notify_one();
    return true;
  }

  [[nodiscard]] constexpr bool try_pop(value_type &value) noexcept(
      std::disjunction_v<std::is_nothrow_move_assignable<value_type>,
                         std::is_nothrow_copy_assignable<value_type>>) {
    auto current_read = read_index_.load(std::memory_order_relaxed);
    if (current_read == write_index_.load(std::memory_order_acquire)) {
      return false;
    }
    if constexpr (std::is_nothrow_assignable_v<value_type>) {
      value = std::move(ring[current_read]);
    } else {
      value = ring[current_read]
    }
    if constexpr (std::negation_v<std::is_trivially_destructible<value_type>>) {
      allocator_traits::destroy(*this, ring + current_read);
    }
    ++current_read;
    if (current_read == REAL_CAPACITY) {
      current_read = 0;
    }
    read_index_.notify_one();
    return true;
  }

  constexpr std::size_t size() const noexcept {
    int ret = read_index_.load(std::memory_order_acquire) -
              write_index_.load(std::memory_order_acquire);
    if (ret < 0) {
      ret += REAL_CAPACITY;
    }
    return ret;
  }

  constexpr bool empty() const noexcept {
    return read_index_.load(std::memory_order_acquire) ==
           write_index_.load(std::memory_order_acquire);
  }

  constexpr std::size_t capacity() const noexcept { return N; }

private:
  using AtomicIndex = std::atomic_size_t;

  char pad0[std::hardware_destructive_interference_size];
  T *const ring;

  alignas(std::hardware_destructive_interference_size) AtomicIndex read_index_;
  alignas(std::hardware_destructive_interference_size) AtomicIndex write_index_;

  char pad1[std::hardware_destructive_interference_size];
};

} // namespace hy

#endif // HY_FREELOCK_QUEUE_HPP_