#ifndef FUNCTIONAL_HPP_
#define FUNCTIONAL_HPP_

#include "Type_traits.hpp"
#include <functional>
#include <type_traits>
#include <function.hpp>
std::function<int()> a;

namespace hy {

template <typename>
class function;

template <typename Res,typename...Args>
class function<Res(Args...)>{

    public:
    using result_type = Res;

    template

    using argument_type [[deprecated]] = ;
    using first_argument_type [[deprecated]] = ;
    using second_argument_type [[deprecated]] = ;

};














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


} // namespace hy

#endif // FUNCTIONAL_HPP_