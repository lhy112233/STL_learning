#ifndef TUPLE_HPP_
#define TUPLE_HPP_
#include <cstddef>
#include <tuple>
#include <type_traits>
std::tuple<int> a;

namespace hy {
/*Helps*/
namespace details {
template <typename T, typename... Types>
inline constexpr auto decide_explicit_constructor(T t) noexcept
    -> decltype((void)(t = {}), bool{}) {
  return decide_explicit_constructor<Types...>();
}

template <typename T, typename... Types>
inline constexpr auto decide_explicit_constructor(T t, ...) noexcept
    -> decltype(bool{}) {
  return false;
}

template <typename... Types, typename = std::enable_if_t<sizeof...(Types) == 0>>
inline constexpr auto decide_explicit_constructor(Types... args) noexcept
    -> bool {
  return true;
}

} // namespace details

template <typename... Typse> class tuple {
public:
  tuple() = default;

  template <class UTypes,
            typename = std::enable_if_t<std::is_constructible_v<void, UTypes>>>
  tuple(const tuple<UTypes> &other) {}

  template <class UType,
            typename = std::enable_if_t<std::is_constructible_v<
                void, std::remove_reference_t<std::remove_cv_t<UType>> &&>>>
  tuple(tuple<UType> &&other) {}

  tuple(const tuple &other) = default;

  tuple(tuple &&other) = default;

  tuple &operator=(const tuple &other) = default;

  tuple &operator=(tuple &&other) noexcept = default;

  template <class... UTypes, typename = std::enable_if_t<
                                 std::is_assignable_v<void, const UTypes &...>>>
  tuple &operator=(const tuple<UTypes...> &other) {}

  template <class... UTypes,
            typename = std::enable_if_t<std::is_assignable_v<void, UTypes...>>>
  tuple &operator=(tuple<UTypes...> &&other) {}

  void swap(tuple &other) {}
};

template <typename T, typename... Types> class tuple<T, Types...> { public: };

/*Non-member-function*/

/*Helpers*/
template <typename...> struct tuple_size;

template <typename... Types>
struct tuple_size<hy::tuple<Types...>>
    : std::integral_constant<std::size_t, sizeof...(Types)> {};

template <typename T>
inline constexpr std::size_t tuple_size_v = tuple_size<T>::value;

template <std::size_t, typename...> struct tuple_element;

template <std::size_t I, typename Head, class... Types>
struct tuple_element<I, hy::tuple<Head, Types...>> {
  static_assert(I < sizeof...(Types), "");
  using type = typename tuple_element<I - 1, Types...>::type;
};

template <typename Head, class... Types>
struct tuple_element<0, hy::tuple<Head, Types...>> {
  using type = Head;
};

template <typename...> struct uses_allocator;

template <typename... Types, typename Alloc>
struct uses_allocator<hy::tuple<Types...>, Alloc> : std::true_type {};

struct ignore_type {
  template <typename... Types> constexpr ignore_type(Types...) noexcept {}

  template <typename T, typename = std::enable_if_t<!std::is_void_v<T>>>
  constexpr const ignore_type &operator=(const T &) {
    return *this;
  }

  ~ignore_type() = default;
};

inline constexpr ignore_type ignore;

} // namespace hy

#endif // TUPLE_HPP_