#ifndef BIT_CORE_UTILITIES_DETAIL_CASTS_INL
#define BIT_CORE_UTILITIES_DETAIL_CASTS_INL

//=============================================================================
// casts
//=============================================================================

template<typename To, typename From, typename>
inline constexpr To bit::core::casts::narrow_cast( From from ) noexcept
{
  To to = static_cast<To>(from);

  BIT_ASSERT( static_cast<From>(to) == from, "narrow_cast: narrowing error" );
  BIT_ASSERT( (is_same_sign<To, From>::value || (to < To{}) == (from < From{})),
              "narrow_cast: narrowing error" );

  return to;
}

//-----------------------------------------------------------------------------

template<typename To, typename From, typename>
inline To bit::core::casts::pointer_cast( From ptr ) noexcept
{
  BIT_ASSERT( static_cast<To>(ptr) == dynamic_cast<To>(ptr),
              "pointer_cast: destination type is not dynamically castable" );

  return static_cast<To>(ptr);
}

//-----------------------------------------------------------------------------

template<typename To, typename From, typename>
inline To bit::core::casts::copy_cast( const From& from )
  noexcept
{
  struct Unnamed{
    To   to;
  } value = {};

  std::memcpy(
    static_cast<void*>(&value.to),
    static_cast<const void*>(&from),
    (sizeof(From) < sizeof(To) ? sizeof(From) : sizeof(To))
  );
  return value.to;
}

//-----------------------------------------------------------------------------

template<typename To, typename From, typename>
inline To bit::core::casts::implicit_cast( From&& from )
{
  return std::forward<From>(from);
}

//-----------------------------------------------------------------------------

template<typename Integral, typename>
inline constexpr std::make_signed_t<Integral>
  bit::core::casts::as_signed( Integral from )
  noexcept
{
  using signed_type = std::make_signed_t<Integral>;

  return static_cast<signed_type>(from);
}

template<typename Enum, typename>
inline constexpr std::make_signed_t<std::underlying_type_t<Enum>>
  bit::core::casts::as_signed( Enum from )
  noexcept
{
  using integral_type = std::underlying_type_t<Enum>;
  using signed_type = std::make_signed_t<integral_type>;

  return static_cast<signed_type>(from);
}

//-----------------------------------------------------------------------------

template<typename Integral, typename>
inline constexpr std::make_unsigned_t<Integral>
  bit::core::casts::as_unsigned( Integral from )
  noexcept
{
  using unsigned_type = std::make_unsigned_t<Integral>;

  return static_cast<unsigned_type>(from);
}

template<typename Enum, typename>
inline constexpr std::make_unsigned_t<std::underlying_type_t<Enum>>
  bit::core::casts::as_unsigned( Enum from )
  noexcept
{
  using integral_type = std::underlying_type_t<Enum>;
  using unsigned_type = std::make_unsigned_t<integral_type>;

  return static_cast<unsigned_type>(from);
}


#endif /* BIT_CORE_UTILITIES_DETAIL_CASTS_INL */
