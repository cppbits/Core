#ifndef BIT_CORE_UTILITIES_DETAIL_INVOKE_INL
#define BIT_CORE_UTILITIES_DETAIL_INVOKE_INL

namespace bit {
  namespace core {
    template<typename Func, typename... Args>
    inline constexpr auto invoke(Func&& func, Args&&... args)
#ifndef _MSC_VER
      noexcept( noexcept( bit::core::detail::invoke_impl(std::forward<Func>(func), std::forward<Args>(args)...) ) )
#endif
      -> decltype( detail::invoke_impl(std::forward<Func>(func), std::forward<Args>(args)...) )
    {
      return detail::invoke_impl(std::forward<Func>(func), std::forward<Args>(args)...);
    }
  } // namespace core
} // namespace bit

#endif /* BIT_CORE_UTILITIES_DETAIL_INVOKE_INL */
