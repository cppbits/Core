#ifndef BIT_CORE_UTILITIES_DETAIL_CONTAINER_UTILITIES_INL
#define BIT_CORE_UTILITIES_DETAIL_CONTAINER_UTILITIES_INL

//=============================================================================
// non-member functions
//=============================================================================

template<typename T, std::size_t N>
inline T& bit::core::at( T(&array)[N], std::size_t n )
{
  BIT_ASSERT_OR_THROW(n<N,std::out_of_range,"at: out of range");

  return array[n];
}

template<typename T>
inline T& bit::core::at( std::initializer_list<T> il, std::size_t n )
{
  BIT_ASSERT_OR_THROW(n<il.size(),std::out_of_range,"at: out of range");

  return *(il.begin() + n);
}

template<typename C, typename Key>
inline auto bit::core::at( C& c, Key&& key )
  -> decltype(c.at(std::forward<Key>(key)))
{
  return c.at( std::forward<Key>(key) );
}

template<typename C, typename Key>
inline auto bit::core::at( const C& c, Key&& key )
  -> decltype(c.at(std::forward<Key>(key)))
{
  return c.at( std::forward<Key>(key) );
}

//-----------------------------------------------------------------------------

template<typename T, std::size_t N>
inline constexpr T* bit::core::data( T(&array)[N] )
  noexcept
{
  return array;
}

template<typename T>
inline constexpr const T* bit::core::data( std::initializer_list<T> il )
  noexcept
{
  return il.begin();
}

template<typename C>
inline constexpr auto bit::core::data( C& c ) -> decltype(c.data())
{
    return c.data();
}

template<typename C>
inline constexpr auto bit::core::data( const C& c ) -> decltype(c.data())
{
    return c.data();
}

//-----------------------------------------------------------------------------

template<typename T, std::size_t N>
inline constexpr T& bit::core::front( T(&array)[N] )
  noexcept
{
  return array[0];
}

template<typename T>
inline constexpr T& bit::core::front( std::initializer_list<T> il )
  noexcept
{
  return *il.begin();
}

template<typename C>
inline constexpr auto bit::core::front( C& c )
  noexcept -> decltype(c.front())
{
    return c.front();
}

template<typename C>
inline constexpr auto bit::core::front( const C& c )
  noexcept -> decltype(c.front())
{
    return c.front();
}

//-----------------------------------------------------------------------------

template<typename T, std::size_t N>
inline constexpr T& bit::core::back( T(&array)[N] )
  noexcept
{
  return array[N-1];
}

template<typename T>
inline constexpr T& bit::core::back( std::initializer_list<T> il )
  noexcept
{
  return *(il.begin() + il.size() - 1);
}

template<typename C>
inline constexpr auto bit::core::back( C& c )
  noexcept -> decltype(c.front())
{
  return c.back();
}

template<typename C>
inline constexpr auto bit::core::back( const C& c )
  noexcept -> decltype(c.back())
{
  return c.back();
}


//-------------------------------------------------------------------------

template<typename T, std::size_t N>
inline constexpr std::size_t bit::core::size( T(&array)[N] )
  noexcept
{
  return N;
}

template<typename C>
inline constexpr auto bit::core::size( const C& c )
  noexcept -> decltype(c.size())
{
  return c.size();
}

//-------------------------------------------------------------------------

template<typename T, std::size_t N>
inline constexpr bool bit::core::empty( T(&array)[N] )
  noexcept
{
  return false;
}

template<typename T>
inline constexpr bool bit::core::empty( std::initializer_list<T> il )
  noexcept
{
  return il.size() == 0u;
}

template<typename C>
inline constexpr auto bit::core::empty( const C& c )
  noexcept -> decltype(c.empty())
{
  return c.empty();
}

//-------------------------------------------------------------------------

namespace bit { namespace core { namespace detail {

  template<typename C>
  struct underlying_container_extractor : private C
  {
    static underlying_container_type_t<C> get( C& container )
    {
      return (container.*&underlying_container_extractor::c);
    }
  };

}}} // namespace bit::core::detail

template<typename C>
inline bit::core::underlying_container_type_t<C>&
  bit::core::get_underlying_container( C& container )
{
  using extractor_type = detail::underlying_container_extractor<C>;
  return extractor_type::get( container );
}

template<typename C>
inline const bit::core::underlying_container_type_t<C>&
  bit::core::get_underlying_container( const C& container )
{
  using extractor_type = detail::underlying_container_extractor<C>;

  return extractor_type::get( container );
}

template<typename C>
inline bit::core::underlying_container_type_t<C>&&
  bit::core::get_underlying_container( C&& container )
{
  using extractor_type = detail::underlying_container_extractor<C>;

  return std::move( extractor_type::get( container ) );
}

template<typename C>
inline const bit::core::underlying_container_type_t<C>&&
  bit::core::get_underlying_container( const C&& container )
{
  using extractor_type = detail::underlying_container_extractor<C>;

  return std::move( extractor_type::get( container ) );
}

#endif //BIT_CORE_UTILITIES_DETAIL_CONTAINER_UTILITIES_INL
