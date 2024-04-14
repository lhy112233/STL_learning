# [std::unique_ptr](https://zh.cppreference.com/w/cpp/memory/unique_ptr)模板类

## Tips
* 在本项目你可以把hy命名空间替换为std命名空间，hy命名空间只是为了提示这是本项目独立编写的。
---
---

## 简介
* std::unique_ptr 是一种智能指针，它通过指针持有并管理另一对象，并在 unique_ptr 离开作用域时释放该对象。<br>
---
---
## **std::unique_ptr** 模板声明
```C++
template<
    class T,
    class Deleter = std::default_delete<T>      (C++11起)(1)
> class unique_ptr;
```

## 类型要求
Deleter 必须是**函数对象 (FunctionObject)** 或**到函数对象 (FunctionObject) 的左值引用或到函数的左值引用**，且可以用 **unique_ptr<T, Deleter>::pointer** 类型的实参予以调用。
* 总结:
    * 仅对模板形参 "**Deleter**" 作出了类型要求(故模板形参 "**T**" 几乎可以是任何类型)。
    * **Deleter允许的类型**
        * 1、可以是[函数对象 (FunctionObject)](https://zh.cppreference.com/w/cpp/named_req/FunctionObject)
        * 2、可以是 函数对象 (FunctionObject) 的**左值引用**或**到函数的左值引用**，且 可以用 unique_ptr<T, Deleter>::pointer 类型的实参予以调用。

---
---


## 好了让我跳转到形参std::unique_ptr的默认模板形参上!!!
## **[std::default_delete](https://zh.cppreference.com/w/cpp/memory/default_delete)** 模板类

### 疑问：
* 什么是**std::default_delete**?
    * **std::default_delete** 是不指定删除器时 **std::unique_ptr** 所用的默认删除策略。
* 大致实现?
    * 定义一个重载了operator( )的类，重载函数内调用了delete去删除传如的形参。
    * 无数据成员，default_delete 的特化在典型实现上为空类，并且用于[空基类优化](https://zh.cppreference.com/w/cpp/language/ebo)。本项目实现为 **std::default_delete** 类的全体全员均为*public*访问控制。
---


### **std::default_delete** 模板声明
``` C++
template< typename T > struct default_delete;          (C++11起)(1)
```
* 非特化的 default_delete 用 delete 解分配单个对象的内存。
* 模板形参*T*是此删除器类型需要删除的对象的类型。<br>简单来说就是此类要处理的类型。
---


## **std::default_delete** 实现分析
### 1、默认构造函数
```C++
constexpr default_delete() noexcept = default;
```
* 此函数很简单，直接使用编译器实现就行。因为此类无数据成员，也不需要使用其他的资源进行初始化。
---

### 2、(另一构造函数)---从另一 std::default_delete 对象构造 std::default_delete 对象。
```C++
template <typename U, 
typename = std::enable_if_t<std::is_convertible_v<U *, T *>>>     (C++11起) 	 
default_delete(const default_delete<U> &d) noexcept {}            (C++23起 constexpr)    
```         



* 此模板函数是在干什么？
    * 答: 以上模板函数其实是一个构造函数。观察上面的模板形参，其中第一个模板形参 *U* 为默认删除器需要处理(删除)的对象的类型。<br>一般来说 U 是 T 的派生类( U 是 T 的子类)，并且两者都拥有虚析构函数。因为其实这个转化大多都是在配合 "动态多态" 这个特性使用的(感觉上就像是为了这个多态特性而写的这个构造函数)。<br>e.g：
    ```C++
    using namespace std;
    class base{public: virtual ~base();};
    class derived:public base{};    //为什么不声明虚析构函数?因为C++11后只要继承了拥有虚析构函数的类就会默认生成虚析构函数。
    default_delete<derived> d;
    default_delete<base> b; //true
    ```
* 此重载只有在 U* 可隐式转换到 T* 时才会参与重载决议。
* 问:如何实现对 U* 的类型限制呢？
    * 答：使用[SFINAE](https://zh.cppreference.com/w/cpp/language/sfinae)。<br>现在开始分析第二个模板形参。[std::enable_if_t](https://zh.cppreference.com/w/cpp/types/enable_if)是一个模板类，主要用来配合[SFINAE](https://zh.cppreference.com/w/cpp/language/sfinae)使用，这东西是一个[变量模板](https://zh.cppreference.com/w/cpp/language/variable_template)(C++14加入的)，当其第一个模板形参为true时返回第二个模板形参(第二模板形参默认为void)。<br>enable_if_v的第一个模板形参为[std::is_convertible_v]()，是对

---


## 参考链接
* [cppreference](https://zh.cppreference.com/w/cpp/memory/unique_ptr)<br>
* [microsoft](https://learn.microsoft.com/zh-cn/cpp/standard-library/unique-ptr-class)

