#ifndef HY_NAMED_REQUIREMENTS_HPP_
#define HY_NAMED_REQUIREMENTS_HPP_
#include <iterator>
#include <type_traits>

namespace hy {
/*Has type member*/
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

template <typename, typename = void>
struct has_layout_type : std::false_type {};

template <typename T>
struct has_layout_type<T, std::void_t<typename T::layout_type>>
    : std::true_type {};

template <typename, typename = void>
struct has_accessor_type : std::false_type {};

template <typename T>
struct has_accessor_type<T, std::void_t<typename T::accessor_type>>
    : std::true_type {};

template <typename, typename = void>
struct has_mapping_type : std::false_type {};

template <typename T>
struct has_mapping_type<T, std::void_t<typename T::mapping_type>>
    : std::true_type {};

template <typename, typename = void>
struct has_element_type : std::false_type {};

template <typename T>
struct has_element_type<T, std::void_t<typename T::element_type>>
    : std::true_type {};

template <typename, typename = void> struct has_index_type : std::false_type {};

template <typename T>
struct has_index_type<T, std::void_t<typename T::index_type>> : std::true_type {
};

template <typename, typename = void> struct has_rank_type : std::false_type {};

template <typename T>
struct has_rank_type<T, std::void_t<typename T::rank_type>> : std::true_type {};

template <typename, typename = void>
struct has_data_handle_type : std::false_type {};

template <typename T>
struct has_data_handle_type<T, std::void_t<typename T::data_handle_type>>
    : std::true_type {};

template <typename, typename = void>
struct has_is_always_equal : std::false_type {};

template <typename T>
struct has_is_always_equal<T, std::void_t<typename T::is_always_equal>>
    : std::true_type {};

template <typename, typename = void>
struct has_propagate_on_container_move_assignment : std::false_type {};

template <typename T>
struct has_propagate_on_container_move_assignment<
    T, std::void_t<typename T::propagate_on_container_move_assignment>>
    : std::true_type {};

template <typename, typename = void>
struct has_propagate_on_container_copy_assignment : std::false_type {};

template <typename T>
struct has_propagate_on_container_copy_assignment<
    T, std::void_t<typename T::propagate_on_container_copy_assignment>>
    : std::true_type {};

template <typename, typename = void>
struct has_propagate_on_container_swap : std::false_type {};

template <typename T>
struct has_propagate_on_container_swap<
    T, std::void_t<typename T::propagate_on_container_swap>> : std::true_type {
};

template <typename, typename = void>
struct has_void_pointer : std::false_type {};

template <typename T>
struct has_void_pointer<T, std::void_t<typename T::void_pointer>>
    : std::true_type {};

template <typename, typename = void>
struct has_const_void_pointer : std::false_type {};

template <typename T>
struct has_const_void_pointer<T, std::void_t<typename T::const_void_pointer>>
    : std::true_type {};

template <typename, typename = void>
struct has_deleter_type : std::false_type {};

template <typename T>
struct has_deleter_type<T, std::void_t<typename T::deleter_type>>
    : std::true_type {};

template <typename, typename = void>
struct has_iterator_category : std::false_type {};

template <typename T>
struct has_iterator_category<T, std::void_t<typename T::iterator_category>>
    : std::true_type {};

/*Has data member*/
template <typename T, typename = void>
struct has_member_data_value : std::false_type {};

template <typename T>
struct has_member_data_value<T, std::void_t<decltype(std::declval<T>().value)>>
    : std::true_type {};

/*Has function member*/
// // 可左自增
// template <typename T, typename = void>
// struct has_member_func_left_self_increment : std::false_type {};

// template <typename T>
// struct has_member_func_left_self_increment<
//     T, std::void_t<decltype(std::declval<T>().operator++())>> : std::true_type {
// };

// // 可右自增
// template <typename T, typename = void>
// struct has_member_func_right_self_increment : std::false_type {};

// template <typename T>
// struct has_member_func_right_self_increment<
//     T, std::void_t<decltype(std::declval<T>().operator++(std::declval<int>()))>>
//     : std::true_type {};

/*Is*/
// 检测类型T是否可左值交换
template <typename T, typename = void>
struct is_lvalue_swappable : std::false_type {};

template <typename T>
struct is_lvalue_swappable<T, std::void_t<decltype(std::swap(
                                  std::declval<T &>(), std::declval<T &>()))>>
    : std::true_type {};

// 检测类型T是否可右值交换
template <typename T, typename = void>
struct is_rvalue_swappable : std::false_type {};

template <typename T>
struct is_rvalue_swappable<
    T,
    std::void_t<decltype(std::swap(std::declval<T &>(), std::declval<T &&>()),
                         std::swap(std::declval<T &&>(), std::declval<T &>()))>>
    : std::true_type {};

// 检测类型It是否为老式迭代器
template <typename It, typename = void>
struct is_legacy_iterator : std::false_type {};

template <typename It>
struct is_legacy_iterator<
    It,
    std::void_t<std::enable_if_t<std::conjunction_v<
        std::is_copy_constructible<It>, /// 可复制构造
        std::is_copy_assignable<It>,    /// 可复制赋值
        std::is_destructible<It>,       /// 可析构
        hy::is_lvalue_swappable<It>,    /// 左值可交换
        hy::has_value_type<std::iterator_traits<
            It>>, /// std::iterator_traits<It>拥有value_type
        hy::has_difference_type<std::iterator_traits<
            It>>, /// std::iterator_traits<It>拥有difference_type
        hy::has_reference<
            std::iterator_traits<It>>, /// std::iterator_traits<It>拥有reference
        hy::has_pointer<
            std::iterator_traits<It>>, /// std::iterator_traits<It>拥有pointer
        hy::has_iterator_category<std::iterator_traits<It>>>>>>
    : std::true_type {};

} // namespace hy

#endif /// HY_NAMED_REQUIREMENTS_HPP_