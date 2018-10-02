#ifndef BIT_CORE_UTILITIES_DETAIL_BYTE_INL
#define BIT_CORE_UTILITIES_DETAIL_BYTE_INL

//=============================================================================
// definitions : non-member operators : enum class : byte
//=============================================================================

//-----------------------------------------------------------------------------
// Operators
//-----------------------------------------------------------------------------

template<typename IntT, typename>
inline constexpr bit::core::byte bit::core::operator<<( byte lhs, IntT shift )
  noexcept
{
  return byte(static_cast<unsigned char>(lhs) << shift);
}

template<typename IntT, typename>
inline constexpr bit::core::byte bit::core::operator>>( byte lhs, IntT shift )
  noexcept
{
  return byte(static_cast<unsigned char>(lhs) >> shift);
}

//-----------------------------------------------------------------------------

inline constexpr bit::core::byte bit::core::operator|( byte lhs, byte rhs )
  noexcept
{
  return byte(static_cast<unsigned char>(lhs) | static_cast<unsigned char>(rhs));
}

inline constexpr bit::core::byte bit::core::operator&( byte lhs, byte rhs )
  noexcept
{
  return byte(static_cast<unsigned char>(lhs) & static_cast<unsigned char>(rhs));
}

inline constexpr bit::core::byte bit::core::operator^( byte lhs, byte rhs )
  noexcept
{
  return byte(static_cast<unsigned char>(lhs) ^ static_cast<unsigned char>(rhs));
}

//-----------------------------------------------------------------------------

inline constexpr bit::core::byte bit::core::operator~( byte lhs )
  noexcept
{
  return byte(~static_cast<unsigned char>(lhs));
}

//-----------------------------------------------------------------------------
// Compound Assignment Operators
//-----------------------------------------------------------------------------

template<typename IntT, typename>
inline constexpr bit::core::byte& bit::core::operator<<=( byte& lhs, IntT shift )
  noexcept
{
  return lhs = byte(static_cast<unsigned char>(lhs) << shift);
}

template<typename IntT, typename>
inline constexpr bit::core::byte& bit::core::operator>>=( byte& lhs, IntT shift )
  noexcept
{
  return lhs = byte(static_cast<unsigned char>(lhs) >> shift);
}

//-----------------------------------------------------------------------------

inline bit::core::byte& bit::core::operator|=( byte& lhs, byte rhs )
  noexcept
{
  return lhs = byte(static_cast<unsigned char>(lhs) | static_cast<unsigned char>(rhs));
}

inline bit::core::byte& bit::core::operator&=( byte& lhs, byte rhs )
  noexcept
{
  return lhs = byte(static_cast<unsigned char>(lhs) & static_cast<unsigned char>(rhs));
}

inline bit::core::byte& bit::core::operator^=( byte& lhs, byte rhs )
  noexcept
{
  return lhs = byte(static_cast<unsigned char>(lhs) ^ static_cast<unsigned char>(rhs));
}

#endif /* BIT_CORE_UTILITIES_DETAIL_BYTE_INL */
