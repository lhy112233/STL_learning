#include <iostream>
#include <scoped_allocator>
#include <type_traits>
#include <utility>

namespace detail {
struct father{
    int a;
};
struct son : father {
    int a;
    son(son& rhs) : father(rhs), a(2) {}
};

} // namespace detail


int main(){
    std::cout << VERSION;
}