#include "Array.hpp"
#include <typeinfo>
#include <iostream>

template class hy::array<int,0>;
template class hy::array<int,1024>;
using namespace hy;
using std::cout;

array<double,3> d0{1,2};

int main(){
    
}