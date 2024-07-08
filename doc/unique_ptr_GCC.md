```C++
  template<typename _Tp>
    struct default_delete
    {
      constexpr default_delete() noexcept = default;

      /** @brief Converting constructor.
       *
       * Allows conversion from a deleter for objects of another type, `_Up`,
       * only if `_Up*` is convertible to `_Tp*`.
       */
      template<typename _Up,
	       typename = std::enable_if_t<std::__and_<std::is_convertible_v<_Up *, _Tp *>>, void>>
        default_delete(const default_delete<_Up>&) noexcept { }

      void
      operator()(_Tp* ptr) const
      {
	static_assert(!is_void_v<_Tp>,
		      "can't delete pointer to incomplete type");
	static_assert(sizeof(_Tp)>0,
		      "can't delete pointer to incomplete type");
	delete ptr;
      }
    };

```