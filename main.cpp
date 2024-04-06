#include <iostream>
#include <functional>
#include <utility>
#include "Unique_ptr.hpp"
#include <memory>
#include <array>
#include "Array.hpp"

// template class hy::unique_ptr<int>;
template class hy::array<int,0>;
template class hy::array<char,1024>;

int main(int, char**){
    std::cout << "Hello, from STL_learning!\n";
    // std::unique_ptr<int> i;
    int i[0];
}


