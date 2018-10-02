#ifndef BIT_CORE_UTILITIES_DETAIL_MONOSTATE_INL
#define BIT_CORE_UTILITIES_DETAIL_MONOSTATE_INL

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

inline constexpr bool bit::core::operator<(monostate, monostate)
  noexcept
{
  return false;
}

inline constexpr bool bit::core::operator>(monostate, monostate)
  noexcept
{
  return false;
}

inline constexpr bool bit::core::operator<=(monostate, monostate)
  noexcept
{
  return true;
}

inline constexpr bool bit::core::operator>=(monostate, monostate)
  noexcept
{
  return true;
}

inline constexpr bool bit::core::operator==(monostate, monostate)
  noexcept
{
  return true;
}

inline constexpr bool bit::core::operator!=(monostate, monostate)
  noexcept
{
  return false;
}

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

inline constexpr bit::core::hash_t bit::core::hash_value( const monostate& )
{
  return static_cast<hash_t>(0);
}

#endif /* BIT_CORE_UTILITIES_DETAIL_MONOSTATE_INL */
