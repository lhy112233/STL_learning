#ifndef HY_FREELOCK_QUEUE_HPP_
#define HY_FREELOCK_QUEUE_HPP_
#include <atomic>
#include <concepts>
#include <cstddef>
#include <expected>
#include <memory>
#include <memory_resource>
#include <new>
#include <thread>
#include <type_traits>
#include <utility>

namespace hy {
template <typename T, std::size_t N, typename Alloc = std::allocator<T>,
          std::size_t SpinCount = 10>
class FreelockQueue final : private Alloc {
  static_assert(N > 1, "N cannot be 0");
  static_assert(std::negation_v<std::is_reference<T>>,
                "value_type is unable to same with value_type&&");

  inline constexpr static std::size_t REAL_CAPACITY = N + 1;
  inline constexpr static std::size_t SPIN_COUNT = SpinCount;

 public:
  using value_type = T;
  using size_type = typename std::allocator_traits<Alloc>::size_type;
  using allocator_type = Alloc;
  using allocator_traits = std::allocator_traits<Alloc>;
  using AtomicIndex = std::atomic<size_type>;

  FreelockQueue(const FreelockQueue &) = delete;
  FreelockQueue(FreelockQueue &&) = delete;
  FreelockQueue &operator=(const FreelockQueue &) = delete;
  FreelockQueue &operator=(FreelockQueue &&) = delete;

  constexpr FreelockQueue()
      : Alloc{},
        ring{allocator_traits::allocate(*this, REAL_CAPACITY)},
        read_index_{0},
        write_index_{0},
        cache_read_index_{0},
        cache_write_index_{0} {}

  constexpr ~FreelockQueue() noexcept(
      std::is_nothrow_destructible_v<value_type &>) {
    if constexpr (std::is_trivially_destructible_v<value_type>) {
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
    allocator_traits::deallocate(*this, ring, REAL_CAPACITY);
  }

  constexpr void push(auto &&...args) noexcept(noexcept(value_type{
      std::forward<decltype(args)>(args)...})) {
    allocator_traits::construct(*this, ring + cache_write_index_,
                                std::forward<decltype(args)>(args)...);

    // 延迟修改，万一构造抛异常了就不需要执行此操作
    auto next_write = cache_write_index_ + 1;
    if (next_write == REAL_CAPACITY) {
      next_write = 0;
    }
    for (int spin_count = SPIN_COUNT;
         next_write == read_index_.load(std::memory_order_acquire);
         --spin_count) {
      if (spin_count == 0) {
        read_index_.wait(next_write, std::memory_order_acquire);
        spin_count = SPIN_COUNT;
      } else {
        std::this_thread::yield();
      }
    }

    write_index_.store(next_write, std::memory_order_release);

    if (cache_write_index_ == read_index_.load(std::memory_order_acquire)) {
      write_index_.notify_one();
    }
    cache_write_index_ = next_write;
  }

  constexpr void pop(value_type &value) noexcept(
      std::disjunction_v<std::is_nothrow_move_assignable<value_type>,
                         std::is_nothrow_copy_assignable<value_type>>) {
    for (int spin_count = SPIN_COUNT;
         cache_read_index_ == write_index_.load(std::memory_order_acquire);
         --spin_count) {
      if (spin_count == 0) {
        write_index_.wait(cache_read_index_, std::memory_order_acquire);
        spin_count = SPIN_COUNT;
      } else {
        std::this_thread::yield();
      }
    }

    if constexpr (std::is_nothrow_move_assignable_v<value_type>) {
      value = std::move(ring[cache_read_index_]);
    } else {
      value = ring[cache_read_index_];
    }
    if constexpr (std::negation_v<std::is_trivially_destructible<value_type>>) {
      allocator_traits::destroy(*this, ring + cache_read_index_);
    }
    auto current_write = write_index_.load(std::memory_order_acquire) + 1;
    if (current_write == REAL_CAPACITY) {
      current_write = 0;
    }
    bool call_tag = current_write == cache_read_index_;

    ++cache_read_index_;
    if (cache_read_index_ == REAL_CAPACITY) {
      cache_read_index_ = 0;
    }
    read_index_.store(cache_read_index_, std::memory_order_release);
    if (call_tag) {
      read_index_.notify_one();
    }
  }

  [[nodiscard]] constexpr bool try_push(auto &&...args) noexcept(
      noexcept(value_type{std::forward<decltype(args)>(args)...})) {
    auto next_write = cache_write_index_ + 1;
    if (next_write == REAL_CAPACITY) {
      next_write = 0;
    }

    auto current_read = read_index_.load(std::memory_order_relaxed);
    if (next_write == current_read) {
      current_read = read_index_.load(std::memory_order_acquire);
      if (next_write == current_read) {
        return false;
      }
    }

    allocator_traits::construct(*this, ring + cache_write_index_,
                                std::forward<decltype(args)>(args)...);
    write_index_.store(next_write, std::memory_order_release);
    // if (cache_write_index_ == current_read)
    // {
    //   write_index_.notify_one();
    // }
    cache_write_index_ = next_write;
    return true;
  }

  [[nodiscard]] constexpr bool try_pop(value_type &value) noexcept(
      std::disjunction_v<std::is_nothrow_move_assignable<value_type>,
                         std::is_nothrow_copy_assignable<value_type>>) {
    auto current_write = write_index_.load(std::memory_order_relaxed);
    if (cache_read_index_ == current_write) {
      current_write = write_index_.load(std::memory_order_acquire);
      if (cache_read_index_ == current_write) {
        return false;
      }
    }

    if constexpr (std::is_nothrow_move_assignable_v<value_type> &&
                  !std::is_trivially_assignable_v<value_type>) {
      value = std::move(ring[cache_read_index_]);
    } else {
      value = ring[cache_read_index_];
    }
    if constexpr (std::negation_v<std::is_trivially_destructible<value_type>>) {
      allocator_traits::destroy(*this, ring + cache_read_index_);
    }

    ++cache_read_index_;
    if (cache_read_index_ == REAL_CAPACITY) {
      cache_read_index_ = 0;
    }
    read_index_.store(cache_read_index_, std::memory_order_release);
    // if (cache_read_index_ == current_write)
    // {
    //   read_index_.notify_one();
    // }
    return true;
  }

  constexpr std::size_t size() const noexcept {
    int ret = read_index_.load(std::memory_order_relaxed) -
              write_index_.load(std::memory_order_relaxed);
    if (ret < 0) {
      ret += REAL_CAPACITY;
    }
    return ret;
  }

  constexpr bool empty() const noexcept {
    return read_index_.load(std::memory_order_relaxed) ==
           write_index_.load(std::memory_order_relaxed);
  }

  constexpr std::size_t capacity() const noexcept { return N; }

 private:
  char pad0[std::hardware_destructive_interference_size];

  T *const ring;
  alignas(std::hardware_destructive_interference_size) AtomicIndex read_index_;
  alignas(std::hardware_destructive_interference_size) AtomicIndex write_index_;
  alignas(std::hardware_destructive_interference_size) size_type
      cache_read_index_;
  alignas(std::hardware_destructive_interference_size) size_type
      cache_write_index_;

  char pad1[std::hardware_destructive_interference_size];
};

}  // namespace hy

#endif  // HY_FREELOCK_QUEUE_HPP_