#ifndef HY_EXCEPTION_HPP_
#define HY_EXCEPTION_HPP_

#include <corecrt_terminate.h>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <type_traits>
#include <utility>
namespace hy {
using terminate_handler = void (*)();

inline terminate_handler& global_terminate_handler() {
  static terminate_handler handler = std::abort;
  return handler;
}

/*Forward declaration*/
class exception;
class nested_exception;
class bad_exception;
class exception_ptr;
[[noreturn]] void terminate() noexcept;
exception_ptr current_exception() noexcept;

class exception {
 public:
  /*Constructors*/
  exception() noexcept {}
  exception(const exception& other) noexcept {}

  /*Assignment*/
  exception& operator=(const exception& other) noexcept { return *this; }

  /*Destructor*/
  virtual ~exception(){};

  virtual const char* what() const noexcept { return "Unknown exception"; }

};  ///class expection

class nested_exception {
 public:
  /*Constructors*/
  nested_exception() noexcept : poniter_exception{current_exception()} {}
  nested_exception(const nested_exception& other) noexcept = default;

  /*Assignment*/
  nested_exception& operator=(const nested_exception&) noexcept = default;

  /*Destructor*/
  virtual ~nested_exception() = default;

  /*Observers*/
  exception_ptr nested_ptr() const noexcept { return poniter_exception; }
  [[noreturn]] void rethrow_nested() const {
    if (nested_ptr()) {
      throw nested_ptr();
    } else {
      terminate();
    }
  }

 private:
  exception_ptr poniter_exception;

};  ///class nested_exception

template <class T>
[[noreturn]] void throw_with_nested(T&& t) {
  static_assert(std::is_copy_constructible_v<std::decay_t<T>>,
                "std::decay<T>::type must be able to copy constructible");
}

/*Begin void rethrow_if_nested(const T& t)*/
namespace details {
template <class E>
struct can_dynamic_cast
    : std::integral_constant<
          bool, std::is_polymorphic<E>::value &&
                    (!std::is_base_of<hy::nested_exception, E>::value ||
                     std::is_convertible<E*, hy::nested_exception*>::value)> {};

template <class T>
void rethrow_if_nested_impl(const T& e, std::true_type) {
  if (auto nep = dynamic_cast<const hy::nested_exception*>(std::addressof(e)))
    nep->rethrow_nested();
}

template <class T>
void rethrow_if_nested_impl(const T&, std::false_type) {}
}  // namespace details

template <class T>
void rethrow_if_nested(const T& t) {
  details::rethrow_if_nested_impl(t, details::can_dynamic_cast<T>());
}
/*End void rethrow_if_nested(const T& t)*/

class bad_exception : public exception {
 public:
  const char* what() const noexcept override { return "Bad exception"; }
};  ///class bad_exception

class exception_ptr {
 public:
  /*Constructors*/
  exception_ptr(void* ptr = nullptr) noexcept : ptr_{ptr} {}

  /*Swap*/
  void swap(exception_ptr& rhs) noexcept { std::swap(this->ptr_, rhs.ptr_); }

  /*Comapre*/
  bool operator==(hy::exception_ptr& rhs) const noexcept {
    return this->ptr_ == rhs.ptr_;
  }
    bool operator!=(hy::exception_ptr& rhs) const noexcept {
    return this->ptr_ != rhs.ptr_;
  }

  /*Conversion*/
  operator bool() noexcept { return ptr_ ? true : false; }

  /*Operators*/
   void* operator*() noexcept{
    return ptr_;
   }

 private:
 /*缺号共享所有权的设计*/
  void* ptr_;


};  ///class exception_ptr

// /*这两个东西根本上就不能凭空实现,记录抛出异常的数量只有在线程全局范围内记录并在throw时对计数进行更新才能实现*/
// int uncaught_exceptions() noexcept{}
// [[deprecated]] bool uncaught_exception() noexcept{return uncaught_exception() > 0;}

[[noreturn]] void rethrow_exception(hy::exception_ptr p) {
  throw *p;
}

/*缺少了同步操作*/
[[noreturn]] void terminate() noexcept {
  global_terminate_handler();
}

hy::terminate_handler get_terminate() noexcept {
  return global_terminate_handler();
}

hy::terminate_handler set_terminate(hy::terminate_handler f) noexcept {
  std::swap(global_terminate_handler(), f);
  return f;
}

}  // namespace hy

#endif  //HY_EXCEPTION_HPP_

#include "exception"
