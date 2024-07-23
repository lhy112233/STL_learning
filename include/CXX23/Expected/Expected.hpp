#ifndef HY_EXPECTED_HPP_
#define HY_EXPECTED_HPP_
#include "Type_traits.hpp"
#include <algorithm>
#include <exception>
#include <type_traits>
#include <utility>

namespace hy {
struct unexpect_t {
  explicit unexpect_t() = default;
};
inline constexpr hy::unexpect_t unexpect{};

/*bad_expected_access*/
/*Forward declear*/
template <typename> class bad_expected_access;

template <> class bad_expected_access<void> : public std::exception {
protected:
  // 构造
  bad_expected_access() noexcept = default;
  bad_expected_access(const bad_expected_access &) = default;
  bad_expected_access(bad_expected_access &&) = default;
  bad_expected_access &operator=(const bad_expected_access &) = default;
  bad_expected_access &operator=(bad_expected_access &&) = default;

  ~bad_expected_access() = default;

public:
  const char *what() const noexcept override {
    return "bad_expected_access<void>";
  };
};

template <class E>
class bad_expected_access : public bad_expected_access<void> {
public:
  // 显式构造
  explicit bad_expected_access(E e) : unex{e} {};

  // 观察器
  const char *what() const noexcept override { return "bad_expected_access;"; };
  E &error() & noexcept { return unex; };
  const E &error() const & noexcept { return unex; };
  E &&error() && noexcept { std::move(unex); };
  const E &&error() const && noexcept { return std::move(unex); };

private:
  E unex; // 仅用于阐释
};

/*class unexpected*/
template <class E> class unexpected {
public:
  // 构造
  constexpr unexpected(const unexpected &) = default;
  constexpr unexpected(unexpected &&) = default;
  template <class... Args,
            typename = std::enable_if_t<std::is_constructible_v<E, Args...>>>
  constexpr explicit unexpected(std::in_place_t, Args &&...args)
      : unex{std::forward<Args>(args)...} {}
  template <class U, class... Args,
            typename = std::enable_if_t<std::is_constructible_v<
                E, std::initializer_list<U> &, Args...>>>
  constexpr explicit unexpected(std::in_place_t, std::initializer_list<U> il,
                                Args &&...args)
      : unex{il, std::forward<Args>(args)...} {}
  template <class Err = E,
            typename = std::enable_if_t<
                !std::is_same_v<hy::remove_cvref_t<Err>, unexpected> &&
                !std::is_same_v<hy::remove_cvref_t<Err>, std::in_place_t> &&
                std::is_constructible_v<E, Err>>>
  constexpr explicit unexpected(Err &&e) : unex{std::move(e)} {}

  // 赋值
  constexpr unexpected &operator=(const unexpected &) = default;
  constexpr unexpected &operator=(unexpected &&) = default;

  // 观察器
  constexpr const E &error() const & noexcept { return unex; };
  constexpr E &error() & noexcept { return unex; };
  constexpr const E &&error() const && noexcept { return std::move(unex); };
  constexpr E &&error() && noexcept { return std::move(unex); };

  // 交换
  constexpr void
  swap(unexpected &other) noexcept(std::is_nothrow_swappable_v<E>) {
    static_assert(std::is_swappable_v<E>, "E must is a sswappable type");
    using std::swap;
    swap(error(), other.error());
  }

  friend constexpr void swap(unexpected &x,
                             unexpected &y) noexcept(noexcept(x.swap(y))) {
    x.swap(y);
    return;
  }

  // 相等运算
  template <class E2>
  friend constexpr bool operator==(const unexpected &x,
                                   const unexpected<E2> &y) {
    return x.error() == y.error();
  }

private:
  E unex; // 仅用于阐释
};

template <class E> unexpected(E) -> unexpected<E>;

/*class expected_stroage*/
namespace detail {
enum class ExpectedStroageTag {
  ePODUnion,
  ePODStruct,
  eUnion,
};

template <typename T, typename E> constexpr ExpectedStroageTag getStroageTag() {
  return std::conjunction_v<std::is_pod<T>, std::is_pod<E>>
             ? (((sizeof(T) > sizeof(E) ? sizeof(T) : sizeof(E)) >
                 sizeof(void *[2]))
                    ? ExpectedStroageTag::ePODUnion
                    : ExpectedStroageTag::ePODStruct)
             : ExpectedStroageTag::eUnion;
}

template <typename T, typename E, ExpectedStroageTag = getStroageTag<T, E>(), bool TriviallyCopyConstructor , bool TriviallyMoveConstructor, bool TriviallyDestroy>   //ePODUnion
struct ExpectedStroage{
    
};

template<typename T,typename E>
struct ExpectedStroage<T, E, , , , ExpectedStroageTag::ePODStruct>{
    public:




    private:
    bool has_val_;
    T val_;
    E err_;
};

template<typename T, typename E>
struct ExpectedStroage<T, E, ExpectedStroageTag::eUnion>{

};



} // namespace detail

/*class expected*/

template <class T, class E> class expected {
public:
  using value_type = T;
  using error_type = E;
  using unexpected_type = unexpected<E>;

  template <class U> using rebind = expected<U, error_type>;

  // 构造
  constexpr expected();
  constexpr explicit(/* 见说明 */) expected(const expected &);
  constexpr explicit(/* 见说明 */) expected(expected &&) noexcept(/* 见说明 */);
  template <class U, class G>
  constexpr explicit(/* 见说明 */) expected(const expected<U, G> &);
  template <class U, class G>
  constexpr explicit(/* 见说明 */) expected(expected<U, G> &&);

  template <class U = T> constexpr explicit(/* 见说明 */) expected(U &&v);

  template <class G> constexpr expected(const unexpected<G> &);
  template <class G> constexpr expected(unexpected<G> &&);

  template <class... Args>
  constexpr explicit expected(std::in_place_t, Args &&...);
  template <class U, class... Args>
  constexpr explicit expected(std::in_place_t, std::initializer_list<U>,
                              Args &&...);
  template <class... Args> constexpr explicit expected(unexpect_t, Args &&...);
  template <class U, class... Args>
  constexpr explicit expected(unexpect_t, std::initializer_list<U>, Args &&...);

  // 析构
  ~expected();

  // 赋值
  constexpr expected &operator=(const expected &);
  constexpr expected &operator=(expected &&) noexcept(/* 见说明 */);
  template <class U = T> constexpr expected &operator=(U &&);
  template <class G> constexpr expected &operator=(const unexpected<G> &);
  template <class G> constexpr expected &operator=(unexpected<G> &&);

  template <class... Args> constexpr T &emplace(Args &&...) noexcept;
  template <class U, class... Args>
  constexpr T &emplace(std::initializer_list<U>, Args &&...) noexcept;

  // 交换
  constexpr void swap(expected &) noexcept(/* 见说明 */);
  friend constexpr void swap(expected &, expected &) noexcept(/* 见说明 */);

  // 观察器
  constexpr const T *operator->() const noexcept;
  constexpr T *operator->() noexcept;
  constexpr const T &operator*() const & noexcept;
  constexpr T &operator*() & noexcept;
  constexpr const T &&operator*() const && noexcept;
  constexpr T &&operator*() && noexcept;
  constexpr explicit operator bool() const noexcept;
  constexpr bool has_value() const noexcept;
  constexpr const T &value() const &;
  constexpr T &value() &;
  constexpr const T &&value() const &&;
  constexpr T &&value() &&;
  constexpr const E &error() const &;
  constexpr E &error() &;
  constexpr const E &&error() const &&;
  constexpr E &&error() &&;
  template <class U> constexpr T value_or(U &&) const &;
  template <class U> constexpr T value_or(U &&) &&;

  // 单子操作
  template <class F> constexpr auto and_then(F &&f) &;
  template <class F> constexpr auto and_then(F &&f) &&;
  template <class F> constexpr auto and_then(F &&f) const &;
  template <class F> constexpr auto and_then(F &&f) const &&;
  template <class F> constexpr auto or_else(F &&f) &;
  template <class F> constexpr auto or_else(F &&f) &&;
  template <class F> constexpr auto or_else(F &&f) const &;
  template <class F> constexpr auto or_else(F &&f) const &&;
  template <class F> constexpr auto transform(F &&f) &;
  template <class F> constexpr auto transform(F &&f) &&;
  template <class F> constexpr auto transform(F &&f) const &;
  template <class F> constexpr auto transform(F &&f) const &&;
  template <class F> constexpr auto transform_error(F &&f) &;
  template <class F> constexpr auto transform_error(F &&f) &&;
  template <class F> constexpr auto transform_error(F &&f) const &;
  template <class F> constexpr auto transform_error(F &&f) const &&;

  // 相等运算
  template <class T2, class E2,
            typename = std::enable_if_t<!std::is_void_v<T2>>>
  friend constexpr bool operator==(const expected &x,
                                   const expected<T2, E2> &y) {
    return x.has_value() ? (y.has_value() && *x == *y)
                         : (!y.has_value() && x.error() == y.error());
  }

  template <class T2>
  friend constexpr bool operator==(const expected &x, const T2 &val) {
    return x.has_value() && static_cast<bool>(*x == val);
  }

  template <class E2>
  friend constexpr bool operator==(const expected &x, const unexpected<E2> &e) {
    return !x.has_value() && static_cast<bool>(x.error() == e.error());
  }

private:
  bool has_val; // 仅用于解释
  union {
    T val;  // 仅用于解释
    E unex; // 仅用于解释
  };
};
} // namespace hy

#endif // HY_EXPECTED_HPP_