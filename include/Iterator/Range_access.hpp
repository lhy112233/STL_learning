#ifndef HY_RANGE_ACCESS_HPP_
#define HY_RANGE_ACCESS_HPP_
#include <cstddef>
#include <memory>

namespace hy {
template <class C> constexpr auto begin(C &c) -> decltype(c.begin()) {
  return c.begin();
}

template <class C> constexpr auto begin(const C &c) -> decltype(c.begin()) {
  return c.begin();
}

template <class T, std::size_t N> constexpr T *begin(T (&array)[N]) noexcept {
  return std::addressof(array);
}

template <class C>
constexpr auto cbegin(const C &c) noexcept(noexcept(hy::begin(c)))
    -> decltype(hy::begin(c)) {
  return hy::begin(c);
}

template< class C >
constexpr auto end( C& c ) -> decltype(c.end()){
return c.end();
}

template< class C >
auto end( const C& c ) -> decltype(c.end()){
    return c.end();
}



} // namespace hy

#endif // HY_RANGE_ACCESS_HPP_