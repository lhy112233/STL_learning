# [std::exception](https://zh.cppreference.com/w/cpp/error/exception)

## 一、复制构造函数。以 other 的内容初始化它的内容。如果 *this 与 other 均拥有动态类型 std::exception，那么 std::strcmp(what(), other.what()) == 0  

```cpp
    exception( const exception& other ) noexcept;

```  

  答:其中的动态类型很关键,std::exception并不是动态类型,这里指代的是派生于std::exception的类型。例如:std::runtime_error、bad_cast等。

* [动态类型](https://zh.cppreference.com/w/cpp/language/type)的解释：如果某个泛左值表达式指代某个多态对象，那么它的最终派生对象的类型被称为它的动态类型。  

```cpp
// 给定
struct B { virtual ~B() {} }; // 多态类型
struct D: B {};               // 多态类型
 
D d; // 最终派生对象
B* ptr = &d;
 
// (*ptr) 的静态类型是 B
// (*ptr) 的动态类型是 D
```

## 二、为什么hy::bad_exception和hy::exception的实现如此粗暴？不需要私有数据成员？万一what()返回的字符串不一样怎么办？

* 有接口知道，这两个类型根本就没给用户提供“解释性字符串”的接口，“解释性字符串完全由实现定义”，故每一个对象都是一样的(左值右值性质除外);