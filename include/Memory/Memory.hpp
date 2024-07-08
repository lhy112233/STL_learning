#ifndef MEMORY_HPP_
#define MEMORY_HPP_

#include "Allocator.hpp"
#include "Allocator_arg.hpp"
#include "Allocator_traits.hpp"
#include "Pointer_safety.hpp"
#include "Pointer_traits.hpp"
#include "Scoped_allocator.hpp"
#include "Unique_ptr.hpp"
#include "Uses_allocator.hpp"
#include "Raw_storage_iterator.hpp"
#include "Align.hpp"
#include "Uninitialized_copy.hpp"


/*
std::addressof无法使用现有标准工具实现，因为 reinterpret_cast
不能用于常量表达式(constexpr)。需要编译器支持。 即是需要使用编译器开洞实现。
https://zh.cppreference.com/w/cpp/memory/addressof
*/

#endif // MEMORY_HPP_