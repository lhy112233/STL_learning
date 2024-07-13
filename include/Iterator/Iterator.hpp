#ifndef HY_ITERATOR_HPP_
#define HY_ITERATOR_HPP_
#include <type_traits>
namespace hy {
    namespace detail {
        template<typename Iter, typename = void>
        struct iterator_traits_difference_type{
            using type = typename Iter::difference_type;
        };

        template<typename Iter>
        struct iterator_traits_difference_type<Iter, std::void_t<typename Iter::difference_type>>{
            using type = typename Iter::difference_type;
        };

        template<typename Iter, typename = void>
        struct iterator_traits_value_type{
            using type = typename Iter::value_type;
        };

        template<typename Iter>
        struct iterator_traits_value_type<Iter, std::void_t<typename Iter::value_type>>{
            using type = typename Iter::value_type;
        };

        template<typename Iter, typename = void>
        struct iterator_traits_pointer{
            using type = typename Iter::pointer;
        };

        template<typename Iter>
        struct iterator_traits_pointer<Iter, std::void_t<typename Iter::pointer>>{
            using type = typename Iter::pointer;
        };

        template<typename Iter, typename = void>
        struct iterator_traits_reference{
            using type = typename Iter::reference;
        };

        template<typename Iter>
        struct iterator_traits_reference<Iter, std::void_t<typename Iter::reference>>{
            using type = typename Iter::reference;
        };

        template<typename Iter, typename = void>
        struct iterator_traits_iterator_category{
            using type = typename Iter::iterator_category;
        };

        template<typename Iter>
        struct iterator_traits_iterator_category<Iter, std::void_t<typename Iter::iterator_category>>{
            using type = typename Iter::iterator_category;
        };        

    }   ///namespace detail


    template< class Iter >
struct iterator_traits{
    public:
    /*Member types*/
    using difference_type = typename Iter::difference_type;
    using value_type = typename Iter::value_type;
    using pointer = typename Iter::pointer;
    using reference = typename Iter::reference;
    using iterator_category = typename Iter::iterator_category;


};  ///struct iterator_traits

}   ///namespace hy




#endif  ///HY_ITERATOR_HPP_