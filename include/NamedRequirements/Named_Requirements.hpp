#ifndef HY_NAMED_REQUIREMENTS_HPP_
#define HY_NAMED_REQUIREMENTS_HPP_
#include <type_traits>

namespace hy {
template <typename, typename = void> struct has_value_type : std::false_type {};

template <typename T>
struct has_value_type<T, std::void_t<typename T::value_type>> : std::true_type {
};

template <typename, typename = void> struct has_pointer : std::false_type {};

template <typename T>
struct has_pointer<T, std::void_t<typename T::pointer>> : std::true_type {};

template <typename, typename = void>
struct has_const_pointer : std::false_type {};

template <typename T>
struct has_const_pointer<T, std::void_t<typename T::const_pointer>>
    : std::true_type {};

template <typename, typename = void> struct has_reference : std::false_type {};

template <typename T>
struct has_reference<T, std::void_t<typename T::reference>> : std::true_type {};

template <typename, typename = void>
struct has_const_reference : std::false_type {};

template <typename T>
struct has_const_reference<T, std::void_t<typename T::const_reference>>
    : std::true_type {};

template <typename, typename = void> struct has_size_type : std::false_type {};

template <typename T>
struct has_size_type<T, std::void_t<typename T::size_type>> : std::true_type {};

template <typename, typename = void>
struct has_difference_type : std::false_type {};

template <typename T>
struct has_difference_type<T, std::void_t<typename T::difference_type>>
    : std::true_type {};

template <typename, typename = void>
struct has_allocator_type : std::false_type {};

template <typename T>
struct has_allocator_type<T, std::void_t<typename T::allocator_type>>
    : std::true_type {};

template <typename, typename = void> struct has_iterator : std::false_type {};

template <typename T>
struct has_iterator<T, std::void_t<typename T::iterator>> : std::true_type {};

template <typename, typename = void>
struct has_const_iterator : std::false_type {};

template <typename T>
struct has_const_iterator<T, std::void_t<typename T::const_iterator>>
    : std::true_type {};

template <typename, typename = void>
struct has_reverse_iterator : std::false_type {};

template <typename T>
struct has_reverse_iterator<T, std::void_t<typename T::reverse_iterator>>
    : std::true_type {};

template <typename, typename = void>
struct has_const_reverse_iterator : std::false_type {};

template <typename T>
struct has_const_reverse_iterator<
    T, std::void_t<typename T::const_reverse_iterator>> : std::true_type {};

template <typename, typename = void> struct has_key_type : std::false_type {};

template <typename T>
struct has_key_type<T, std::void_t<typename T::key_type>> : std::true_type {};

template <typename, typename = void>
struct has_mapped_type : std::false_type {};

template <typename T>
struct has_mapped_type<T, std::void_t<typename T::mapped_type>>
    : std::true_type {};

template <typename, typename = void>
struct has_key_compare : std::false_type {};

template <typename T>
struct has_key_compare<T, std::void_t<typename T::key_compare>>
    : std::true_type {};

template <typename, typename = void> struct has_node_type : std::false_type {};

template <typename T>
struct has_node_type<T, std::void_t<typename T::node_type>> : std::true_type {};

template <typename, typename = void> struct has_hasher : std::false_type {};

template <typename T>
struct has_hasher<T, std::void_t<typename T::hasher>> : std::true_type {};

template <typename, typename = void> struct has_key_equal : std::false_type {};

template <typename T>
struct has_key_equal<T, std::void_t<typename T::key_equal>> : std::true_type {};

template <typename, typename = void>
struct has_local_iterator : std::false_type {};

template <typename T>
struct has_local_iterator<T, std::void_t<typename T::local_iterator>>
    : std::true_type {};

template <typename, typename = void>
struct has_const_local_iterator : std::false_type {};

template <typename T>
struct has_const_local_iterator<T,
                                std::void_t<typename T::const_local_iterator>>
    : std::true_type {};

template <typename, typename = void>
struct has_extents_type : std::false_type {};

template <typename T>
struct has_extents_type<T, std::void_t<typename T::extents_type>>
    : std::true_type {};

} // namespace hy

#endif /// HY_NAMED_REQUIREMENTS_HPP_