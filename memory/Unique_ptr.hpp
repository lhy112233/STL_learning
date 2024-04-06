#pragma once
#ifndef UNIQUE_PTR_HPP_
#define UNIQUE_PTR_HPP_

#include "Utility.hpp"
#include <type_traits>

namespace hy
{
    template <class T>
    struct default_delete
    {
        constexpr default_delete() noexcept = default;

        template <class U, typename = std::enable_if_t<std::is_convertible_v<U *, T *>>>
        default_delete(const default_delete<U> &d) noexcept {}

        void operator()(T *ptr) const
        {
            static_assert(!std::is_void_v<T>, "can't delete pointer to incomplete type");
            static_assert(sizeof(T) > 0, "can't delete pointer to incomplete type");
            delete ptr;
        }
    };

    // hy::unique_ptr implement
    template <typename Tp, typename Dp = hy::default_delete<Tp>>
    class unique_ptr
    {
        template <typename Up, typename Ep, typename = void>
        struct Ptr
        {
            using type = Up*;
        };

        template <typename Up, typename Ep>
        struct Ptr<Up, Ep, typename std::remove_reference_t<Ep>::pointer>
        {
            using type = typename std::remove_reference_t<Ep>::pointer;
        };

        static_assert(!std::is_rvalue_reference_v<Dp>,
        "unique_ptr's deleter type must be a function object type"
		" or an lvalue reference type");

    public:
        using pointer = typename Ptr<Tp, Dp>::type;
        using element_type = Tp;
        using deleter_type = Dp;

    private:
        pointer pointer_ = nullptr;
        Dp deleter_ = Dp{};

    public:
        pointer get() const noexcept
        {
            return pointer_;
        }

        Dp &get_deleter() noexcept
        {
            return deleter_;
        }

        const Dp &get_deleter() const noexcept
        {
            return deleter_;
        }

        explicit operator bool() const noexcept
        {
            return get() != nullptr;
        }

        pointer release() noexcept
        {
            auto ret = get();
            pointer_ = nullptr;
            return ret;
        }

        void reset(pointer ptr = pointer()) noexcept
        {
            // assert(ptr == get());
            static_assert(std::is_invocable_v<deleter_type &, pointer>,
                          "unique_ptr's deleter must be invocable with a pointer");
            auto old_ptr = get();
            pointer_ = ptr;
            if (old_ptr)
                get_deleter()(old_ptr);
        }

        void swap(unique_ptr &other) noexcept
        {
            auto ptemp = other.get();
            auto dtemp = hy::forward<Dp>(other.get_deleter());
            other.pointer_ = get();
            other.deleter_ = hy::forward<Dp>(get_deleter());
            pointer_ = ptemp;
            deleter_ = hy::forward<Dp>(dtemp);
        }

        std::add_lvalue_reference_t<Tp> operator*() const
            noexcept(noexcept(*std::declval<pointer>()))
        {
            return *get();
        }

        pointer operator->() const noexcept
        {
            return get();
        }

        /*
         *构造不拥有对象的 std::unique_ptr。值初始化存储的指针和存储的删除器。
         *要求 Deleter 可默认构造且构造不抛异常。
         *这些重载只有在 std::is_default_constructible<Deleter>::value 为 true
         *且 Deleter 不是指针类型时才会参与重载决议。
         */
        constexpr unique_ptr() noexcept = default;
        constexpr unique_ptr(std::nullptr_t) noexcept {}

        /*
         *构造拥有 p 的 std::unique_ptr，以 p 初始化存储的指针，并值初始化存储的删除器。
         *要求 Deleter 可复制构造且构造不抛异常。
         *这些重载只有在 std::is_default_constructible<Deleter>::value 为 true 且 Deleter 不是指针类型时才会参与重载决议。
         *类模板实参推导不选择此构造函数。(C++17 起)
         */
        explicit unique_ptr(pointer p) noexcept : pointer_{p} {}

        /*
         *构造拥有 p 的 std::unique_ptr 对象，以 p 初始化存储的指针，并按下列方式初始化删除器 D（依赖于 D 是否为引用类型）。
         *a) 若 D 是非引用类型 A，则签名是：
         *unique_ptr(pointer p, const A& d) noexcept;    (1)	（要求 Deleter 为不抛出可复制构造 (CopyConstructible) ）
         *unique_ptr(pointer p, A&& d) noexcept;         (2)	（要求 Deleter 为不抛出可移动构造 (MoveConstructible) ）
         *b) 若 D 是左值引用类型 A&，则签名是：
         *unique_ptr(pointer p, A& d) noexcept;          (1)
         *unique_ptr(pointer p, A&& d) = delete;         (2)
         *c) 若 D 是左值引用类型 const A&，则签名是：
         *unique_ptr(pointer p, const A& d) noexcept;    (1)
         *unique_ptr(pointer p, const A&& d) = delete;   (2)
         *所有情况下删除器从 std::forward<decltype(d)>(d) 初始化。
         这些重载只有在 std::is_constructible<D, decltype(d)>::value 为 true 时才会参与重载决议。
         *类模板实参推导不选择这两个构造函数。(C++17 起)
         */
        template <typename = std::enable_if_t<std::is_nothrow_copy_constructible_v<Dp>>>
        unique_ptr(pointer p,const deleter_type& d)noexcept :pointer_{p},deleter_{d} {}

        template <typename = std::enable_if_t<std::is_nothrow_move_constructible_v<Dp>>>
        unique_ptr(pointer p,std::enable_if_t<!std::is_lvalue_reference_v<Dp>,Dp&&> d)noexcept:pointer_{p},deleter_{hy::move(d)}{}

        template <typename = std::remove_reference_t<Dp>>
        unique_ptr(pointer , std::enable_if_t<std::is_lvalue_reference_v<Dp>,Dp&&>) = delete;

        /*
         *通过从 u 转移所有权给 *this 构造 unique_ptr 并存储空指针于 u。
         *此构造函数仅若 std::is_move_constructible<Deleter>::value 为 true 才参与重载决议。
         *若 Deleter 不是引用类型，则要求它为不抛出可移动构造 (MoveConstructible)
         *（若 Deleter 是引用，则 get_deleter() 和 u.get_deleter() 在移动构造后引用相同值）。
         */
        template <typename = std::enable_if_t<std::is_nothrow_move_constructible_v<Dp>>,
                  typename = std::enable_if_t<!std::is_reference_v<Dp>>>
        unique_ptr(unique_ptr &&u) noexcept
        {
            pointer_ = u.get();
            u.pointer_ = nullptr;
            deleter_ = hy::move(u.get_deleter());
        }

        template <typename = std::enable_if_t<std::is_move_constructible_v<Dp>>>
        unique_ptr(unique_ptr &&u)
        {
            pointer_ = u.get();
            u.pointer_ = nullptr;
            deleter_ = u.get_deleter();
        }

        /*
         *通过从 u 转移所有权给 *this 构造 unique_ptr，其中 u 以指定的删除器（E）构造。它依赖于 E 是否为引用类型，如下：
         *a) 若 E 是引用类型，则从 u 的删除器复制构造此删除器（要求此构造不抛出）
         *b) 若 E 不是引用类型，则从 u 的删除器移动构造此删除器（要求此构造不抛出）
         *此构造函数仅若下列皆为真才参与重载决议：
         *a) unique_ptr<U, E>::pointer 可隐式转换为 pointer
         *b) U 不是数组类型
         *c) Deleter 是引用类型且 E 与 D 为同一类型，或 Deleter 不是引用类型且 E 可隐式转换为 D
         */
        template <class U, class E,
                  typename = std::enable_if_t<std::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer>>, // unique_ptr<U, E>::pointer 可隐式转换为 pointer
                  typename = std::enable_if_t<!std::is_array_v<U>>,                                                     // U 不是数组类型
                  typename = std::enable_if_t<std::is_reference_v<E>>,                                                  // Deleter 是引用类型、若 E 是引用类型
                  typename = std::enable_if_t<std::is_same_v<E, Dp>>,                                                   // E 与 D 为同一类型
                  typename = std::enable_if_t<std::is_nothrow_copy_constructible_v<Dp>>>                                // 若 E 是引用类型，则从 u 的删除器复制构造此删除器（要求此构造不抛出）
        unique_ptr(unique_ptr<U, E> &&u) noexcept : pointer_{u.get()}, deleter_{u.get_deleter()}
        {
            u.pointer_ = nullptr;
        }

        template <class U, class E,
                  typename = std::enable_if_t<std::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer>>, // //unique_ptr<U, E>::pointer 可隐式转换为 pointer
                  typename = std::enable_if_t<!std::is_array_v<U>>,                                                     // U 不是数组类型
                  typename = std::enable_if_t<!std::is_reference_v<E>>,                                                 // Deleter 不是引用类型、若 E 不是引用类型
                  typename = std::enable_if_t<std::is_convertible_v<E, Dp>>,                                            // E 可隐式转换为 D
                  typename = std::enable_if_t<std::is_nothrow_move_constructible_v<Dp>>>                                // 若 E 不是引用类型，则从 u 的删除器移动构造此删除器（要求此构造不抛出）
        unique_ptr(unique_ptr<U, std::remove_reference_t<E>> &&u) noexcept : pointer_{u.get()}, deleter_{hy::move(u.get_deleter())}
        {
            u.pointer_ = nullptr;
        }

        unique_ptr(const unique_ptr &) = delete;

        ~unique_ptr()
        {
            if (!get() == nullptr)
                get_deleter()(get());
        }


        /*
        移动赋值运算符。
        *从 r 转移所有权到 *this，如同在调用 reset(r.release()) 后立即将 std::forward<Deleter>(r.get_deleter()) 赋给 get_deleter()。
        *此重载只有在 std::is_move_assignable<Deleter>::value 是 true 时才会参与重载决议。
        *如果 Deleter 不是引用类型，那么在以下情况下行为未定义：
        *Deleter 不可移动赋值 (MoveAssignable) 。
        *将 Deleter 类型的右值赋给 get_deleter() 会抛出异常。
        *否则（Deleter 是引用类型），那么在以下情况下行为未定义：
        *std::remove_reference<Deleter>::type 不可复制赋值 (CopyAssignable) 。
        *将 Deleter 类型的左值赋给 get_deleter() 会抛出异常。
        */
       template <typename = std::enable_if_t<std::is_move_constructible_v<Dp>>>
        unique_ptr& operator=( unique_ptr&& r ) noexcept{
            reset(r.release());
            get_deleter() = hy::forward<Dp>(r.get_deleter());
            return *this;
        }

        /*
        *转换赋值运算符。从 r 转移所有权到 *this，如同在调用 reset(r.release()) 后立即将 std::forward<E>(r.get_deleter()) 赋给 get_deleter()。
        *对于主模板，此重载只有在满足以下所有条件时才会参与重载决议：
        *U 不是数组类型。
        *unique_ptr<U, E>::pointer 可以隐式转换到 pointer。
        *std::is_assignable<Deleter&, E&&>::value 是 true。
        *如果 E 不是引用类型，那么在将 E 类型的右值赋给 get_deleter() 会非良构或者抛出异常时行为未定义。
        *否则（E 是引用类型），那么在将 E 类型的左值赋给 get_deleter() 会非良构或者抛出异常时行为未定义。
        */
        template< class U, class E ,
        typename = std::enable_if_t<std::is_convertible_v<unique_ptr<U, E>::pointer,pointer>>,
        typename = std::enable_if_t<std::is_assignable_v<Dp&,E&&>>>
        unique_ptr& operator=( unique_ptr<U, E>&& r ) noexcept{
            reset(r.release());
            get_deleter() = hy::forward<E>(r.get_deleter());
            return *this;
        }

        unique_ptr& operator=( std::nullptr_t ) noexcept{
            reset();
            return *this;
        }

        unique_ptr& operator=( const unique_ptr& ) = delete;

    };

}

#endif
