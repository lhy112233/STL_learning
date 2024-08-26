# Hy's C++ Tiny Standard Library

这是hy实现的C++标准库(STL)的存储库，它是模拟GCC和MSVC的STL进行编写的，能够在使用在支持标准C++的编译器平台上使用。

* 此项目只支持C++14及以上的标准。
* 此项目将尽量避免使用特定平台的扩展，尽量以标准C++提供的功能替代之。
* 此项目将把STL的命名空间替换为"hy"以提示为独立实现。
* 每个部件都将提供README.md对主要代码进行解释并作出感想，想要详细介绍者者请移步至对应README.md。

## Introduction
    TinySTL是我空闲时间实现的一个项目，只为了交流学习。这也是我第一个较为规范实现的项目，如有不规范的或者错误的地方敬请指正。


## Required
    * 支持C++14及以上标准的平台 (必选)
    * Cmake (可选，主要用于构建测试)
    * Git   (可选，主要用于管理代码)

## Goals
    此项目将按照[cppreference](https://zh.cppreference.com)提供的接口规范进行编写(没钱买ISO，笑)，以尽量还原标准STL，不过只是实现我常用的部件(冷门的部件感觉没必要去实现)。
    该项目使用的是C++17标准，但在C++14及以上的平台能编译通过，但是可能存在部分部件的实现规范不符合标准(如constexpr、弃用、移除等的存在)。
    在完成组件后将对组件进行C++11~C++20标准的适配更新(新建文件进行更改适配)。

## Complete schedule

* array             100%
* memory 20%
    * allocator     100%
    * unique_ptr    95%
    * shared_ptr    0%
    * weak_ptr      0%
* type_traits       5%(只能完成一部分，因有的代码需要编译器支持)
* utility           5%
* iterator          5%
* vector            90%
* list              0%
* deque             0%
* stack             100%
* queue             100%
* map               0%
* set               0%
* pair              0%
* tuple             50%
* initializer_list  无法实现，由编译器底层调用
* thread            看情况实现
* mutex             看情况实现
* future            0%
* unordered_map     0%
* unordered_set     0%
* functional        0%
* algorithm         0%
    * for_each      0%
    * swap          0%
    * sort          0%
    * max           0%
    * min           0%
    * equal         0%
    * reverse       0%
    * ...

## Tips
    对C++常用部件不熟悉的用户请看"保姆级介绍.md"，里面详细讲述了该项目用到的所有工具。
