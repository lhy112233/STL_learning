#ifndef FUNCTIONAL_HPP_
#define FUNCTIONAL_HPP_

#include "Type_traits.hpp"
#include <type_traits>

namespace hy {

template <class T = void> 
struct less 
{
    using result_type = bool;
    using first_argument_type [[deprecated]] = T;
    using second_argument_type [[deprecated]] = T;

    constexpr bool operator()( const T& lhs, const T& rhs ) const{
        return lhs < rhs;
    }
};

template<>
struct less<void>
{
    using is_transpa
};

} // namespace hy

#endif // FUNCTIONAL_HPP_