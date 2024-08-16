#ifndef TUPLE_HPP_
#define TUPLE_HPP_
#include <cstddef>
#include <type_traits>n

namespace hy {
template <typename T, typename... Types> class tuple : public tuple<Types...> {
public:
  ~tuple<Types...>() = default;

private:
  T data_;
};

template <typename T> class tuple<T> {
public:
  ~tuple<T>() = default;

private:
  T data_;
};

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

} // namespace hy

#endif // TUPLE_HPP_