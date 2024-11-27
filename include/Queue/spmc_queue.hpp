#ifndef HY_SPMC_QUEUE_HPP_
#define HY_SPMC_QUEUE_HPP_
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <memory_resource>
#include <new>
#include <thread>
#include <type_traits>
#include <utility>

namespace hy {
  /*最多执行2^64次push，执行次数超过后未定义行为*/
template <typename T, std::size_t N, typename Alloc = std::allocator<T>,
          int Spin_count = -1>
class SPMC_QueueBase final : private Alloc {
  static_assert(std::negation_v<std::is_reference<T>>,
                "value_type is unable to same with value_type&&");
  static_assert(N > 1, "N must bigger than 1");

  inline constexpr static std::size_t REAL_CAPACITY = N + 1;
  inline constexpr static std::size_t MASK = N;
  inline constexpr static int SPIN_COUNT = Spin_count;

 public:
  using value_type = T;
  using allocator_type = Alloc;
  using allocator_traits = std::allocator_traits<allocator_type>;
  // using size_type = std::conditional_t<
  //     std::is_unsigned_v<typename allocator_traits::size_type>,
  //     typename allocator_traits::size_type, std::size_t>;
  using size_type = std::uint64_t;
  using pointer = typename allocator_traits::pointer;
  using const_pointer = typename allocator_traits::const_pointer;
  using difference_type = typename allocator_traits::difference_type;
  using reference = T&;
  using const_reference = const T&;

  constexpr SPMC_QueueBase() noexcept(noexcept(Alloc{}) && noexcept(
      allocator_traits::allocate(std::declval<Alloc&>(), REAL_CAPACITY)))
      : Alloc{},
        ring_{allocator_traits::allocate(*this, REAL_CAPACITY)},
        push_index_{0},
        pop_index_{0},
        push_cache_push_index_{0},
        push_cache_pop_index_{0},
        pop_cache_push_index_{0},
        pop_cache_pop_cache_{0} {}

  constexpr ~SPMC_QueueBase() {
    if constexpr (std::negation_v<std::is_trivially_destructible<value_type>>) {
      auto push_index = push_index_.load(std::memory_order_acquire);
      auto pop_index = pop_index_.load(std::memory_order_acquire);
      while (push_index != pop_index) {
        element(pop_index)->~value_type();
        ++pop_index;
      }
      allocator_traits::deallocate(*this, ring_, REAL_CAPACITY);
    }
  }

  SPMC_QueueBase(const SPMC_QueueBase&) = delete;
  SPMC_QueueBase(SPMC_QueueBase&&) =delete;
  SPMC_QueueBase& operator=(const SPMC_QueueBase&) = delete;
  SPMC_QueueBase& operator=(SPMC_QueueBase&&) = delete;

  constexpr bool try_push(auto&&... args) noexcept(noexcept(value_type{
      std::forward<decltype(args)>(args)...})) {
    if (full(push_cache_push_index_, push_cache_pop_index_)) {
      push_cache_pop_index_ = pop_index_.load(std::memory_order_acquire);
      if (full(push_cache_push_index_, push_cache_pop_index_)) {
        return false;
      }
    }

    new (std::to_address(element(push_cache_push_index_)))
        value_type(std::forward<decltype(args)>(args)...);
    push_index_.store(++push_cache_push_index_, std::memory_order_release);
    return true;
  }

  constexpr bool try_pop(reference value) noexcept(
      std::is_nothrow_move_assignable_v<value_type>) {
    if (empty(pop_cache_push_index_, pop_cache_pop_cache_)) {
      pop_cache_push_index_ = push_index_.load(std::memory_order_acquire);
      if (empty(pop_cache_push_index_, pop_cache_pop_cache_)) {
        return false;
      }
    }

    if constexpr (std::is_nothrow_move_assignable_v<value_type> &&
                  std::negation_v<std::is_scalar<value_type>>) {
      value = std::move(*element(pop_cache_pop_cache_));
    } else {
      value = *element(pop_cache_pop_cache_);
    }

    if constexpr (std::negation_v<std::is_trivially_destructible<value_type>>) {
      element(pop_cache_pop_cache_)->~value_type();
    }

    pop_index_.store(++pop_cache_pop_cache_, std::memory_order_release);
    return true;
  }



  constexpr pointer data() noexcept { return ring_; }
  constexpr const_pointer data() const noexcept { return ring_; }

  /*e.g: */
  /*1 0000 0001 - 1111 1111 = -1(1111 1111) - 1111 1111 + 1(截断的) + 1 = 2*/
  constexpr size_type size() const noexcept {
    auto push_index = push_index_.load(std::memory_order_relaxed);
    auto pop_index = pop_index_.load(std::memory_order_relaxed);
    return (push_index >= pop_index)
               ? (push_index - pop_index)
               : (static_cast<size_type>(-1) - pop_index + push_index + 1);
  }

  constexpr size_type capacity() const noexcept { return N; }

 private:
  constexpr bool full(size_type push_index, size_type pop_index) noexcept {
    /*SAFE*/
    // if (push_index >= pop_index) [[likely]] {
    //   return (push_index - pop_index) == capacity();
    // } else [[unlikely]] {
    //   auto difference_num =
    //       (static_cast<size_type>(-1) - pop_index + push_index + 1);
    //   return difference_num == capacity();
    // }
  return (push_index - pop_index) == REAL_CAPACITY;
  }

  constexpr bool empty(size_type push_index, size_type pop_index) noexcept {
    return push_index == pop_index;
  }

  pointer element(size_type index) noexcept { return ring_ + (index & MASK); }

 private:
  using AtomicIndex = std::atomic<size_type>;
  char pad1[std::hardware_destructive_interference_size];
  pointer const ring_;
  alignas(std::hardware_destructive_interference_size) AtomicIndex push_index_;
  alignas(std::hardware_destructive_interference_size) AtomicIndex pop_index_;
  alignas(std::hardware_destructive_interference_size) size_type
      push_cache_push_index_;
  alignas(std::hardware_destructive_interference_size) size_type
      push_cache_pop_index_;
  alignas(std::hardware_destructive_interference_size) size_type
      pop_cache_push_index_;
  alignas(std::hardware_destructive_interference_size) size_type
      pop_cache_pop_cache_;

  char pad2[std::hardware_destructive_interference_size];
};
}  // namespace hy

#endif  // HY_SPMC_QUEUE_HPP_