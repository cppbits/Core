#ifndef BIT_CORE_ITERATORS_DETAIL_ZIP_ITERATOR_INL
#define BIT_CORE_ITERATORS_DETAIL_ZIP_ITERATOR_INL

//============================================================================
// zip_iterator
//============================================================================

//----------------------------------------------------------------------------
// Constructors
//----------------------------------------------------------------------------

template<typename...InputIterators>
inline constexpr bit::core::zip_iterator<InputIterators...>
  ::zip_iterator( InputIterators...iterators )
  : m_storage( iterators... )
{

}

//----------------------------------------------------------------------------
// Iteration
//----------------------------------------------------------------------------

template<typename...InputIterators>
inline constexpr bit::core::zip_iterator<InputIterators...>&
  bit::core::zip_iterator<InputIterators...>::operator++()
  noexcept
{
  increment( std::index_sequence_for<InputIterators...>{} );
  return (*this);
}

template<typename...InputIterators>
inline constexpr bit::core::zip_iterator<InputIterators...>
  bit::core::zip_iterator<InputIterators...>::operator++(int)
  noexcept
{
  auto result = (*this);
  ++(*this);

  return result;
}

//----------------------------------------------------------------------------
// Observers
//----------------------------------------------------------------------------

template<typename...InputIterators>
inline constexpr typename bit::core::zip_iterator<InputIterators...>::reference
  bit::core::zip_iterator<InputIterators...>::operator*()
  const noexcept
{
  return unpack( std::index_sequence_for<InputIterators...>{} );
}

//----------------------------------------------------------------------------
// Comparison
//----------------------------------------------------------------------------

template<typename...InputIterators>
template<typename...Its>
constexpr bool bit::core::zip_iterator<InputIterators...>::operator==( const zip_iterator<Its...>& rhs )
  const noexcept
{
  return m_storage == rhs.m_storage;
}

template<typename...InputIterators>
template<typename...Its>
constexpr bool bit::core::zip_iterator<InputIterators...>::operator!=( const zip_iterator<Its...>& rhs )
  const noexcept
{
  return m_storage != rhs.m_storage;
}

template<typename...InputIterators>
template<typename...Its>
constexpr bool bit::core::zip_iterator<InputIterators...>::operator<( const zip_iterator<Its...>& rhs )
  const noexcept
{
  return m_storage < rhs.m_storage;
}

template<typename...InputIterators>
template<typename...Its>
constexpr bool bit::core::zip_iterator<InputIterators...>::operator<=( const zip_iterator<Its...>& rhs )
  const noexcept
{
  return m_storage <= rhs.m_storage;
}

template<typename...InputIterators>
template<typename...Its>
constexpr bool bit::core::zip_iterator<InputIterators...>::operator>( const zip_iterator<Its...>& rhs )
  const noexcept
{
  return m_storage >= rhs.m_storage;
}

template<typename...InputIterators>
template<typename...Its>
constexpr bool bit::core::zip_iterator<InputIterators...>::operator>=( const zip_iterator<Its...>& rhs )
  const noexcept
{
  return m_storage >= rhs.m_storage;
}

//----------------------------------------------------------------------------
// Private Implementation
//----------------------------------------------------------------------------

template<typename...InputIterators>
template<std::size_t...Idxs>
inline constexpr typename bit::core::zip_iterator<InputIterators...>::reference
  bit::core::zip_iterator<InputIterators...>::unpack( std::index_sequence<Idxs...> )
  const noexcept
{
  return { *std::get<Idxs>(m_storage)... };
}

//----------------------------------------------------------------------

template<typename...InputIterators>
template<std::size_t Idx0, std::size_t Idx1, std::size_t...Idxs>
inline constexpr void bit::core::zip_iterator<InputIterators...>
  ::increment( std::index_sequence<Idx0, Idx1, Idxs...> )
{
  ++std::get<Idx0>(m_storage);
  increment( std::index_sequence<Idx1, Idxs...>{} );
}

template<typename...InputIterators>
template<std::size_t Idx>
inline constexpr void bit::core::zip_iterator<InputIterators...>
  ::increment( std::index_sequence<Idx> )
{
  ++std::get<Idx>(m_storage);
}

#endif /* BIT_CORE_ITERATORS_DETAIL_ZIP_ITERATOR_INL */
