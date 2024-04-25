#ifndef ALLOCATOR_TRAITS_HPP_
#define ALLOCATOR_TRAITS_HPP_
#include <type_traits>
#include "Pointer_traits.hpp"

namespace hy {
template <class Alloc> 
class allocator_traits {
    //typename pointer
    template <typename Tp,typename = void>
    struct pointer_type{using pointer =typename  Alloc::value_type*;};

    template<typename Tp>
    struct pointer_type<Tp,typename Alloc::pointer>{using pointer = typename Alloc::pointer;};
    

    //const_pointer




    public:
    using allocator_type = Alloc;
    using value_type = typename Alloc::value_type;
    using pointer = typename pointer_type<Alloc>::pointer;



};
} // namespace hy

#endif // ALLOCATOR_TRAITS_HPP_