#ifndef BIT_CORE_UTILITIES_DETAIL_SOURCE_LOCATION_INL
#define BIT_CORE_UTILITIES_DETAIL_SOURCE_LOCATION_INL

//=============================================================================
// class : source_location
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

inline constexpr bit::core::source_location::
  source_location( const char* file_name,
                   const char* function_name,
                   std::size_t line )
  : m_file{file_name},
    m_function{function_name},
    m_line{line}
{

}

//-----------------------------------------------------------------------------
// Access
//-----------------------------------------------------------------------------

inline constexpr std::size_t bit::core::source_location::line()
  const noexcept
{
  return m_line;
}

inline constexpr const char* bit::core::source_location::function_name()
  const noexcept
{
  return m_function;
}

inline constexpr const char* bit::core::source_location::file_name()
  const noexcept
{
  return m_file;
}

//=============================================================================
// non-member functions : class : source_location
//=============================================================================

//-----------------------------------------------------------------------------
// Equality
//-----------------------------------------------------------------------------

inline bool bit::core::operator==( const source_location& lhs,
                                   const source_location& rhs )
  noexcept
{
  return lhs.file_name() == rhs.file_name() &&
         lhs.function_name() == rhs.function_name() &&
         lhs.line() == rhs.line();
}

inline bool bit::core::operator!=( const source_location& lhs,
                                   const source_location& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_CORE_UTILITIES_DETAIL_SOURCE_LOCATION_INL */
