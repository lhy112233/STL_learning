#ifndef HY_MEMORY_RESOURCE_HPP_
#define HY_MEMORY_RESOURCE_HPP_
#include <cassert>
#include <cstddef>
#include <memory>
#include <new>

namespace hy {
namespace pmr {

/*class memory_resource*/
class memory_resource {
public:
  /*Member functions*/
  /*Constructors*/
  memory_resource() = default;
  memory_resource(const memory_resource &) = default;

  /*Destructor*/
  virtual ~memory_resource() {}

  /*Assignment*/
  memory_resource &operator=(memory_resource &) = default;

  /*Other functions*/
  void *allocate(std::size_t bytes,
                 std::size_t alignment = alignof(std::max_align_t)) {
    return do_allocate(bytes, alignment);
  }

  void deallocate(void *p, std::size_t bytes,
                  std::size_t alignment = alignof(std::max_align_t)) {
    do_deallocate(p, bytes, alignment);
  }

  bool is_equal(const memory_resource &other) const noexcept {
    return do_is_equal(other);
  }

private:
  virtual void *do_allocate(std::size_t bytes, std::size_t alignment) = 0;
  virtual void do_deallocate(void *p, std::size_t bytes,
                             std::size_t alignment) = 0;
  virtual bool
  do_is_equal(const hy::pmr::memory_resource &other) const noexcept = 0;
}; /// class memory_resource

/*Non-member-functions*/
inline bool operator==(const hy::pmr::memory_resource &a,
                       const hy::pmr::memory_resource &b) noexcept {
  return &a == &b || a.is_equal(b);
}

inline bool operator!=(const hy::pmr::memory_resource &a,
                       const hy::pmr::memory_resource &b) noexcept {
  return !(a == b);
}

/******************************/
/*Details*/
namespace detail {
class detail_new_delete_resource : public hy::pmr::memory_resource {
private:
  virtual void *do_allocate(std::size_t bytes, std::size_t alignment) {
    return ::operator new(bytes, static_cast<std::align_val_t>(alignment));
  }

  virtual void do_deallocate(void *p, std::size_t bytes,
                             std::size_t alignment) {
    ::operator delete(p, static_cast<std::align_val_t>(alignment));
  }

  virtual bool
  do_is_equal(const hy::pmr::memory_resource &other) const noexcept {
    return (std::addressof(other) == this);
  }
};

class detail_null_memory_resource : public hy::pmr::memory_resource {
private:
  [[noreturn]] virtual void *do_allocate(std::size_t bytes,
                                         std::size_t alignment) {
    throw std::bad_alloc{};
  }

  virtual void do_deallocate(void *p, std::size_t bytes,
                             std::size_t alignment) {}

  virtual bool
  do_is_equal(const hy::pmr::memory_resource &other) const noexcept {
    return (std::addressof(other) == this);
  }
};

} // namespace detail

inline hy::pmr::memory_resource *new_delete_resource() noexcept {
  static hy::pmr::detail::detail_new_delete_resource p;
  return std::addressof(p);
}

inline hy::pmr::memory_resource *null_memory_resource() noexcept {
  static hy::pmr::detail::detail_null_memory_resource p;
  return std::addressof(p);
}

inline hy::pmr::memory_resource *get_default_resource() noexcept { /*TODO*/ }

inline hy::pmr::memory_resource *
set_default_resource(hy::pmr::memory_resource *r) noexcept {
  /*TODO*/
}

struct pool_options {
  std::size_t max_blocks_per_chunk;
  std::size_t largest_required_pool_block;
};

class synchronized_pool_resource : public hy::pmr::memory_resource { /*TODO*/
};

class unsynchronized_pool_resource : public hy::pmr::memory_resource { /*TODO*/
};

/*Detail*/
namespace detail {
struct Chunk {

private:
  Chunk* tail_;
};
}; // namespace detail

class monotonic_buffer_resource : public hy::pmr::memory_resource {
  inline static constexpr std::size_t kGrowthFactor = 2;
  inline static constexpr std::size_t kInitBufferSize = 64 * sizeof(void *);

public:
  /*Member functions*/
  /*Constructors*/
  monotonic_buffer_resource()
      : upstream_rsrc_{hy::pmr::get_default_resource()} {}

  explicit monotonic_buffer_resource(hy::pmr::memory_resource *upstream)
      : upstream_rsrc_{upstream} {}

  explicit monotonic_buffer_resource(std::size_t initial_size)
      : upstream_rsrc_{hy::pmr::get_default_resource()},
        next_buffer_size_{initial_size > kInitBufferSize ? initial_size
                                                         : kInitBufferSize} {}

  monotonic_buffer_resource(std::size_t initial_size,
                            hy::pmr::memory_resource *upstream)
      : upstream_rsrc_{upstream},
        next_buffer_size_{initial_size > kInitBufferSize ? initial_size
                                                         : kInitBufferSize} {}

  monotonic_buffer_resource(void *buffer, std::size_t buffer_size)
      : upstream_rsrc_{hy::pmr::get_default_resource()},
        current_buffer_{buffer},
        next_buffer_size_{((buffer_size * kGrowthFactor) > kInitBufferSize)
                              ? (buffer_size * kGrowthFactor)
                              : kInitBufferSize},
        space_available_{buffer_size} {}

  monotonic_buffer_resource(void *buffer, std::size_t buffer_size,
                            hy::pmr::memory_resource *upstream)
      : upstream_rsrc_{upstream}, current_buffer_(buffer),
        next_buffer_size_{(buffer_size * kGrowthFactor) > kInitBufferSize
                              ? (buffer_size * kGrowthFactor)
                              : kInitBufferSize},
        space_available_{buffer_size} {}

  monotonic_buffer_resource(const hy::pmr::monotonic_buffer_resource &) =
      delete;

  /*Destructor*/
  virtual ~monotonic_buffer_resource() { release(); }

  /*Assignment*/
  monotonic_buffer_resource &operator=(monotonic_buffer_resource &) = delete;
  monotonic_buffer_resource &operator=(monotonic_buffer_resource &&) = delete;

  /*Public member functions*/
  void release() { /*TODO*/ }

  hy::pmr::memory_resource *upstream_resource() const { return upstream_rsrc_; }

  /*Protected member functions*/
protected:
  virtual void *do_allocate(std::size_t bytes,
                            std::size_t alignment = alignof(std::max_align_t)) {
    /*TODO*/
  }

  virtual void do_deallocate(void *p, std::size_t bytes,
                             std::size_t alignment) { /*nonbehavior*/ }

  virtual bool
  do_is_equal(const hy::pmr::memory_resource &other) const noexcept {
    return this == std::addressof(other);
  }

private:
  hy::pmr::memory_resource *upstream_rsrc_;
  void* const orig_buffer_ = nullptr;
  std::size_t orig_size_ = 0;

  void *current_buffer_ = nullptr;
  std::size_t space_available_ = 0;
  std::size_t next_buffer_size_ = kInitBufferSize;

  hy::pmr::detail::Chunk* chunk_ = nullptr;
};

} // namespace pmr
} // namespace hy

#endif /// HY_MEMORY_RESOURCE_HPP_

#include <memory_resource>
std::pmr::monotonic_buffer_resource a;