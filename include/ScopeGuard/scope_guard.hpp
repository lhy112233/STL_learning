#ifndef HY_SCOPEGUARD_HPP_
#define HY_SCOPEGUARD_HPP_
#include <cstddef>
#include <exception>
#include <functional>
#include <ios>
#include <iostream>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace hy {
namespace thrid_party {
namespace detail {
struct ScopeGuardDismissed {};

class ScopeGuardImplBase {
public:
  void dismiss() noexcept { dismissed_ = true; }
  void rehire() noexcept { dismissed_ = false; }

protected:
  ScopeGuardImplBase(bool dismissed = false) noexcept : dismissed_(dismissed) {}

  [[noreturn]] static void terminate() noexcept;
  static ScopeGuardImplBase makeEmptyScopeGuard() noexcept {
    return ScopeGuardImplBase{};
  }

  bool dismissed_;
};

template <typename FunctionType, bool InvokeNoexcept>
class ScopeGuardImpl : public ScopeGuardImplBase {
public:
  explicit ScopeGuardImpl(FunctionType &fn) noexcept(
      std::is_nothrow_copy_constructible_v<FunctionType>)
      : ScopeGuardImpl{
            //委托构造
            std::as_const(fn),
            makeFailsafe(std::is_nothrow_copy_constructible<FunctionType>{},
                         std::addressof(fn))} {}

  explicit ScopeGuardImpl(const FunctionType &fn) noexcept(
      std::is_nothrow_copy_constructible_v<FunctionType>)
      : ScopeGuardImpl( //委托构造
            fn, makeFailsafe(std::is_nothrow_copy_constructible<FunctionType>{},
                             std::addressof(fn))) {}

  explicit ScopeGuardImpl(FunctionType &&fn) noexcept(
      std::is_move_constructible_v<FunctionType>)
      : ScopeGuardImpl( //委托构造
            std::move_if_noexcept(fn),
            makeFailsafe(std::is_nothrow_move_constructible<FunctionType>{},
                         std::addressof(fn))) {}

  explicit ScopeGuardImpl(FunctionType &&fn, ScopeGuardDismissed) noexcept(
      std::is_nothrow_move_constructible_v<FunctionType>)
      : ScopeGuardImplBase{true}, function_{std::forward<FunctionType>(fn)} {}

  ScopeGuardImpl(ScopeGuardImpl &&rhs) noexcept( //移动构造
      std::is_nothrow_move_constructible_v<FunctionType>)
      : function_(std::move_if_noexcept(rhs.function_)) {
    dismissed_ = std::exchange(rhs.dismissed_, true);
  }

  ~ScopeGuardImpl() {
    if (!dismissed_) {
      execute();
    }
  }

private:
  /*makeFailsafe是决定构造形式的关键*/
  /*两个makeFailsafe重载的返回值是不一样的，
   */

  static ScopeGuardImplBase makeFailsafe(std::true_type,
                                         const void *) noexcept {
    return makeEmptyScopeGuard();
  }

  template <typename Fn>
  static auto makeFailsafe(std::false_type, Fn *fn) noexcept
      -> ScopeGuardImpl<decltype(std::ref(*fn)), InvokeNoexcept> {
    return ScopeGuardImpl<decltype(std::ref(*fn)), InvokeNoexcept>{
        std::ref(*fn)};
  }
  /*************************** */

  /*最终作用到的构造函数*/
  /*关键是这个failsafe实参*/
  template <typename Fn>
  explicit ScopeGuardImpl(Fn &&fn, ScopeGuardImplBase &&failsafe)
      : ScopeGuardImplBase{}, function_(std::forward<Fn>(fn)) {
    failsafe.dismiss();
  }

  void *operator new(std::size_t) = delete;
  void operator delete(void *) = delete;

  void execute() noexcept(InvokeNoexcept) {
    if constexpr (InvokeNoexcept) {
      static_assert(std::is_same_v<void, decltype(function_())>);
      try {
        function_();
      } catch (...) {
        std::ios_base::Init ioInit;
        std::cerr << "This program will now terminate because a hy::ScopeGuard "
                     "callback";
        // std::terminate();
        throw;
      }
    } else {
      function_();
    }
  }

  FunctionType function_;
};

template <typename F, bool INE>
using ScopeGuardImplDecay = ScopeGuardImpl<typename std::decay_t<F>, INE>;

} // namespace detail

template <typename F>
[[nodiscard]] detail::ScopeGuardImplDecay<F, true> makeGuard(F &&f) noexcept(
    noexcept(detail::ScopeGuardImplDecay<F, true>(static_cast<F &&>(f)))) {
  return detail::ScopeGuardImplDecay<F, true>(static_cast<F &&>(f));
}

template <typename F>
[[nodiscard]] detail::ScopeGuardImplDecay<F, true>
makeDismissedGuard(F &&f) noexcept(noexcept(
    detail::ScopeGuardImplDecay<F, true>(static_cast<F &&>(f),
                                         detail::ScopeGuardDismissed{}))) {
  return detail::ScopeGuardImplDecay<F, true>(static_cast<F &&>(f),
                                              detail::ScopeGuardDismissed{});
}

/*****************************************************************************************************
 */

namespace detail {
template <typename FunctionType, bool ExecuteOnException>
class ScopeGuardForNewException {
public:
  explicit ScopeGuardForNewException(const FunctionType &fn) : guard_{fn} {}

  explicit ScopeGuardForNewException(FunctionType &&fn)
      : guard_(std::move(fn)) {}

  ScopeGuardForNewException(ScopeGuardForNewException &&rhs) = default;

  ~ScopeGuardForNewException() noexcept(ExecuteOnException) {
    if (ExecuteOnException !=
        (exceptionCounter_ < std::uncaught_exceptions())) {
      guard_.dismiss();
    }
  }

private:
  void *operator new(std::size_t) = delete;
  void operator delete(void *) = delete;

  ScopeGuardImpl<FunctionType, ExecuteOnException> guard_;
  int exceptionCounter_{std::uncaught_exceptions()};
};

enum class ScopeGuardOnFail {};

template <typename FunctionType>
ScopeGuardForNewException<typename std::decay<FunctionType>::type, true>
operator+(detail::ScopeGuardOnFail, FunctionType &&fn) {
  return ScopeGuardForNewException<typename std::decay<FunctionType>::type,
                                   true>(std::forward<FunctionType>(fn));
}

enum class ScopeGuardOnSuccess {};

template <typename FunctionType>
ScopeGuardForNewException<typename std::decay<FunctionType>::type, false>
operator+(ScopeGuardOnSuccess, FunctionType &&fn) {
  return ScopeGuardForNewException<typename std::decay<FunctionType>::type,
                                   false>(std::forward<FunctionType>(fn));
}

enum class ScopeGuardOnExit {};

template <typename FunctionType>
ScopeGuardImpl<typename std::decay<FunctionType>::type, true>
operator+(detail::ScopeGuardOnExit, FunctionType &&fn) {
  return ScopeGuardImpl<typename std::decay<FunctionType>::type, true>(
      std::forward<FunctionType>(fn));
}

} // namespace detail

#define SCOPE_EXIT                                                             \
  auto FB_ANONYMOUS_VARIABLE(SCOPE_EXIT_STATE) =                               \
      ::folly::detail::ScopeGuardOnExit() + [&]() noexcept

#define SCOPE_FAIL                                                             \
  auto FB_ANONYMOUS_VARIABLE(SCOPE_FAIL_STATE) =                               \
      ::folly::detail::ScopeGuardOnFail() + [&]() noexcept

#define SCOPE_SUCCESS                                                          \
  auto FB_ANONYMOUS_VARIABLE(SCOPE_SUCCESS_STATE) =                            \
      ::folly::detail::ScopeGuardOnSuccess() + [&]()

} // namespace thrid_party
} // namespace hy

#endif // HY_SCOPEGUARD_HPP_