#ifndef TYPR_TRAITS_HPP_
#define TYPR_TRAITS_HPP_

#include <cstddef> //nullptr_t

namespace hy {
template <class T, T v> struct integral_constant {
  using value_type = T;
  using type = typename hy::integral_constant<T, v>;

  static constexpr T value = v;

  constexpr operator value_type() const noexcept { return value; }
  constexpr value_type operator()() const noexcept { return value; }

}; // hy::integral_constant

using true_type = hy::integral_constant<bool, true>;

using false_type = hy::integral_constant<bool, false>;

template <bool B> using bool_constant = integral_constant<bool, B>;

template <bool B, typename T, typename F> struct conditional {
  using type = T;
};

template <typename T, typename F> struct conditional<false, T, F> {
  using type = F;
};

template <bool B, typename T, typename F>
using conditional_t = typename hy::conditional<B, T, F>::type;

template <typename... B> struct conjunction : hy::true_type {};

template <typename B> struct conjunction<B> : B {};

template <typename B, typename... Bn>
struct conjunction<B, Bn...>
    : hy::conditional<static_cast<bool>(B::value), hy::conjunction<Bn...>, B> {
};

template <typename... B>
inline constexpr bool conjunction_v = conjunction<B...>::value;

template <typename... B> struct disjunction : hy::false_type {};

template <typename B1> struct disjunction<B1> : B1 {};

template <typename B1, typename... Bn>
struct disjunction<B1, Bn...>
    : hy::conditional_t<static_cast<bool>(B1::value), B1, disjunction<Bn...>> {
};

template <typename... B>
inline constexpr bool disjunction_v = disjunction<B...>::value;

template <class B>
struct negation : hy::bool_constant<!static_cast<bool>(B::value)> {};

template<typename... T>
using void_t = void;

template <typename T, typename U> struct is_same : hy::false_type {};

template <typename T> struct is_same<T, T> : hy::true_type {};

template <typename T, typename U>
inline constexpr bool is_same_v = is_same<T, U>::value;

template <class T> struct remove_cv {
  using type = T;
};

template <class T> struct remove_cv<const T> {
  using type = T;
};

template <class T> struct remove_cv<volatile T> {
  using type = T;
};

template <class T> struct remove_cv<const volatile T> {
  using type = T;
};

template <class T> struct remove_const {
  using type = T;
};

template <class T> struct remove_const<const T> {
  using type = T;
};

template <class T> struct remove_volatile {
  using type = T;
};

template <class T> struct remove_volatile<volatile T> {
  typedef T type;
};

template <typename T> using remove_cv_t = typename remove_cv<T>::type;

template <class T> using remove_const_t = typename remove_const<T>::type;

template <class T> using remove_volatile_t = typename remove_volatile<T>::type;

template <typename T> struct is_void : hy::is_same<void, hy::remove_cv_t<T>> {};

template <typename T> inline constexpr bool is_void_v = is_void<T>::value;

template <typename T>
struct is_null_pointer : hy::is_same<std::nullptr_t, hy::remove_cv_t<T>> {};

template <typename T>
inline constexpr bool is_null_pointer_v = is_null_pointer<T>::value;

// template <typename T>
// struct is_integral :

template <typename T> struct remove_reference {
  using type = T;
};

template <typename T> struct remove_reference<T &> {
  using type = T;
};

template <typename T> struct remove_reference<T &&> {
  using type = T;
};

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

template<typename T>
struct alignment_of : hy::integral_constant<std::size_t, alignof(T)>{};

template<typename T>
inline constexpr std::size_t alignment_of_v = hy::alignment_of<T>::value;



} // namespace hy

#endif // TYPR_TRAITS_HPP_