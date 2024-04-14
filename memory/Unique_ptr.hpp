#ifndef UNIQUE_PTR_HPP_
#define UNIQUE_PTR_HPP_
#include <memory>
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
            if (!ptr)
                delete ptr;
        }
    };

    template <typename T, typename D = default_delete<T>>
    class unique_ptr
    {
        template <typename Tp, typename Dp, typename = void>
        struct pointer_type
        {
            using pointer = T *;
        };

        template <typename Tp, typename Dp>
        struct pointer_type<Tp, Dp, typename std::remove_reference<Dp>::type::pointer>
        {
            using pointer = typename std::remove_reference<Dp>::type::pointer
        };

    public:
        using pointer = typename pointer_type<T, D>::pointer;
        using element_type = T;
        using deleter_type = D;

    private:
        // pointer要满足可空指针 (NullablePointer)  https://zh.cppreference.com/w/cpp/named_req/NullablePointer
        // pointer必须是可相等比较的，找不到，算了！
        static_assert(std::is_nothrow_default_constructible_v<pointer>, "Does not meet the requirements of NullablePointer...Default Constructible");
        static_assert(std::is_nothrow_copy_constructible_v<pointer>, "Does not meet the requirements of NullablePointer...Copy Constructible");
        static_assert(std::is_nothrow_copy_assignable_v<pointer>, "Does not meet the requirements of NullablePointer...Copy Assignable");
        static_assert(std::is_nothrow_swappable_v<pointer>, "Does not meet the requirements of NullablePointer...lvalues of the type are Swappable"); // 疑问
        static_assert(std::is_nothrow_destructible_v<pointer>, "Does not meet the requirements of NullablePointer...Destructible");
        // 还有一些要求，但是在找不到检测办法了，不过后面的操作也会使用到相关的方法，从而侧面检测了要求。

        static_assert(!std::is_rvalue_reference_v<D>, "Unique_ptr's deleter type must be a function object type or an lvalue reference type");
        static_assert(std::is_invocable_v<D &, pointer>, "D must invocate T");

        pointer data_ = nullptr; // 无管理默认为空
        D deleter_;

    public:
        pointer get() const noexcept
        {
            return data_;
        }

        D &get_deleter() noexcept
        {
            return deleter_;
        }

        const D &get_deleter() const noexcept
        {
            return deleter_;
        }

        explicit operator bool() const noexcept
        {
            static_assert(sizeof(T) > 0, "can't compare type to incomplete type");
            return get() != nullptr;
        }

        pointer release() noexcept
        {
            pointer ret = get();
            data_ = nullptr;
            return ret;
        }

        void reset(pointer ptr = pointer{}) noexcept
        {
            // assert(ptr != get());
            pointer old_ptr = data_;
            data_ = ptr;
            if (old_ptr)
                get_deleter()(old_ptr);
        }

        void swap(unique_ptr &other) noexcept
        {
            static_assert(std::is_swappable_v<D>, "deleter must be swappable");
            std::swap(hy::move(this->data_), hy::move(other.data_));                   // 其动作是无异常的
            std::swap(hy::forward<D>(this->deleter_), hy::forward<D>(other.deleter_)); // 其动作是不确定是否抛异常的
        }

        typename std::add_lvalue_reference<T>::type operator*() const
            noexcept(noexcept(*std::declval<pointer>())) { return *get(); }

        pointer operator->() const noexcept { return get(); }

        constexpr unique_ptr() noexcept = default;

        constexpr unique_ptr(std::nullptr_t) noexcept {}

        explicit unique_ptr(pointer p) noexcept : data_{p} {}

        template <typename Dp, typename = std::enable_if_t<std::is_constructible_v<D, Dp>>>
        unique_ptr(pointer p, const Dp &d)
        {
            static_assert(std::is_nothrow_copy_constructible_v<D>, "D must nothrow copy constructible to D!");
            data_ = hy::move(p);
            deleter_ = d;
        }

        template <typename Dp = D, typename = std::enable_if_t<!std::is_lvalue_reference_v<Dp>>>
        unique_ptr(pointer p, Dp &&d)
        {
            static_assert(std::is_nothrow_move_constructible_v<D>, "D must nothrow move constructible to D!");
            data_ = hy::move(p);
            deleter_ = hy::move(d);
        }

        template <typename Dp = D, typename = std::enable_if_t<std::is_lvalue_reference_v<Dp>>>
        unique_ptr(pointer p, Dp &&d) = delete;

        template <typename U, typename E,
                  typename = std::enable_if_t<std::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer> && !std::is_array_v<U> && std::is_reference_v<E> && std::is_same_v<E, D>>>
        unique_ptr(unique_ptr<U, E> &&u) noexcept : data_{hy::move(u.data_)}, deleter_{u.get_deleter()}
        {
            static_assert(std::is_nothrow_copy_constructible_v<E>, "E must nothrow copy constructible to D!");
            u.data_ = nullptr;
        }

        template <typename U, typename E,
                  typename = std::enable_if_t<std::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer> && !std::is_array_v<U> && !std::is_reference_v<E> && std::is_convertible_v<E, D>>>
        unique_ptr(unique_ptr<U, E> &&u) noexcept : data_{hy::move(u.data_)}, deleter_{hy::move(u.get_deleter())}
        {
            static_assert(std::is_nothrow_move_constructible_v<D>, "E must nothrow move constructible to D!");
            u.data_ = nullptr;
        }

        unique_ptr(const unique_ptr &) = delete;

        ~unique_ptr()
        {
            if (get() == nullptr)
                get_deleter()(get());
        }

        template <typename = std::enable_if_t<std::is_move_assignable_v<D>>>
        unique_ptr &operator=(unique_ptr &&r) noexcept
        {
            reset(r.release());
            get_deleter() = std::forward<D>(r.get_deleter());
        }

        template <class U, class E,
                  std::enable_if_t<std::is_array_v<U> && std::is_convertible_v<typename unique_ptr<U, E>::pointer, pointer> && std::is_assignable_v<D &, E &&>>>
        unique_ptr &operator=(unique_ptr<U, E> &&r) noexcept
        {
            reset(r.release());
            get_deleter() = hy::forward<E>(r.get_deleter());
        }

        unique_ptr &operator=(std::nullptr_t) noexcept
        {
            reset();
        }

        unique_ptr &operator=(const unique_ptr &) = delete;

    }; // class unique_ptr

    template <class T, class... Args>
    std::enable_if_t<!std::is_array<T>::value, hy::unique_ptr<T>> make_unique(Args &&...args)
    {
        return hy::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template <class T1, class D1, class T2, class D2>
    bool operator==(const unique_ptr<T1, D1> &x, const unique_ptr<T2, D2> &y)
    {
        return x.get() == y.get();
    }

    template <class T1, class D1, class T2, class D2>
    bool operator!=(const unique_ptr<T1, D1> &x, const unique_ptr<T2, D2> &y)
    {
        return x.get() != y.get();
    }

    template <class T1, class D1, class T2, class D2>
    bool operator<(const unique_ptr<T1, D1> &x, const unique_ptr<T2, D2> &y)
    {
        using CT = typename std::common_type<typename unique_ptr<T1, D1>::pointer, typename unique_ptr<T2, D2>::pointer>::type;
        return std::less<CT>()(x.get(), y.get());
    }

    template <class T1, class D1, class T2, class D2>
    bool operator<=(const unique_ptr<T1, D1> &x, const unique_ptr<T2, D2> &y)
    {
        return !(y < x);
    }

    template <class T1, class D1, class T2, class D2>
    bool operator>(const unique_ptr<T1, D1> &x, const unique_ptr<T2, D2> &y)
    {
        return y < x;
    }

    template <class T1, class D1, class T2, class D2>
    bool operator>=(const unique_ptr<T1, D1> &x, const unique_ptr<T2, D2> &y)
    {
        return !(x < y);
    }

    template <class T, class D>
    bool operator==(const unique_ptr<T, D> &x, std::nullptr_t) noexcept
    {
        return !x;
    }

    template <class T, class D>
    bool operator==(std::nullptr_t, const unique_ptr<T, D> &x) noexcept
    {
        return !x;
    }

    template <class T, class D>
    bool operator!=(const unique_ptr<T, D> &x, std::nullptr_t) noexcept
    {
        return (bool)x;
    }

    template <class T, class D>
    bool operator!=(std::nullptr_t, const unique_ptr<T, D> &x) noexcept
    {
        return (bool)x;
    }

    template <class T, class D>
    bool operator<(const unique_ptr<T, D> &x, std::nullptr_t)
    {
        return std::less<unique_ptr<T, D>::pointer>()(x.get(), nullptr);
    }

    template <class T, class D>
    bool operator<(std::nullptr_t, const unique_ptr<T, D> &y)
    {
        return std::less<unique_ptr<T, D>::pointer>()(nullptr, y.get());
    }

    template <class T, class D>
    bool operator<=(const unique_ptr<T, D> &x, std::nullptr_t)
    {
        return !(nullptr < x);
    }

    template <class T, class D>
    bool operator<=(std::nullptr_t, const unique_ptr<T, D> &y)
    {
        return !(y < nullptr);
    }

    template <class T, class D>
    bool operator>(const unique_ptr<T, D> &x, std::nullptr_t)
    {
        return nullptr < x;
    }

    template <class T, class D>
    bool operator>(std::nullptr_t, const unique_ptr<T, D> &y)
    {
        return y < nullptr;
    }

    template <class T, class D>
    bool operator>=(const unique_ptr<T, D> &x, std::nullptr_t)
    {
        return !(x < nullptr);
    }

    template <class T, class D>
    bool operator>=(std::nullptr_t, const unique_ptr<T, D> &y)
    {
        return !(nullptr < y);
    }

    template <class T, class D, typename = std::enable_if_t<std::is_swappable_v<D>>>
    void swap(hy::unique_ptr<T, D> &lhs, hy::unique_ptr<T, D> &rhs) noexcept
    {
        lhs.swap(rhs);
    }

} // namespace hy

#endif // UNIQUE_HPP_
