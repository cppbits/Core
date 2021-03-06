#ifndef BIT_CORE_CONTAINERS_DETAIL_SPAN_INL
#define BIT_CORE_CONTAINERS_DETAIL_SPAN_INL

//=============================================================================
// class : span
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors / Assignment
//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr bit::core::span<T,Extent>::span()
  noexcept
  : m_storage{nullptr,0}
{

}

template<typename T, std::ptrdiff_t Extent>
inline constexpr bit::core::span<T,Extent>::span( std::nullptr_t )
  noexcept
  : span{}
{

}

template<typename T, std::ptrdiff_t Extent>
inline constexpr bit::core::span<T,Extent>::span( pointer first, pointer last )
  noexcept
  : m_storage{ first, std::distance(first,last) }
{

}

template<typename T, std::ptrdiff_t Extent>
template<typename U, std::ptrdiff_t OtherExtent, typename>
inline constexpr bit::core::span<T,Extent>::
  span( const span<U,OtherExtent>& other )
  : m_storage{ other.m_storage.data(),
               bit::core::detail::span_extent_type<OtherExtent>(other.size()) }
{

}

template<typename T, std::ptrdiff_t Extent>
template<typename U>
inline constexpr bit::core::span<T,Extent>::span( U* ptr, size_type count )
  noexcept
  : m_storage{ ptr, count }
{

}

template<typename T, std::ptrdiff_t Extent>
template<typename U, std::size_t N>
inline constexpr bit::core::span<T,Extent>::span( U (&array)[N] )
  noexcept
  : m_storage{ &array[0], bit::core::detail::span_extent_type<N>() }
{
  static_assert( std::is_convertible<U*,T*>::value, "Type U being assigned is not convertible to T" );
}

template<typename T, std::ptrdiff_t Extent>
template<typename ContiguousContainer, typename>
inline constexpr bit::core::span<T,Extent>::span( ContiguousContainer& container )
  noexcept
  : m_storage{ container.data(), static_cast<size_type>(container.size()) }
{

}

template<typename T, std::ptrdiff_t Extent>
template<typename ContiguousContainer, typename>
inline constexpr bit::core::span<T,Extent>::span( const ContiguousContainer& container )
  noexcept
  : m_storage{ container.data(), static_cast<size_type>(container.size()) }
{

}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::size_type
  bit::core::span<T,Extent>::length()
  const noexcept
{
  return size();
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::size_type
  bit::core::span<T,Extent>::size()
  const noexcept
{
  return m_storage.size();
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::size_type
  bit::core::span<T,Extent>::max_size()
  const noexcept
{
  return m_storage.size();
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::size_type
  bit::core::span<T,Extent>::length_bytes()
  const noexcept
{
  return size_bytes();
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::size_type
  bit::core::span<T,Extent>::size_bytes()
  const noexcept
{
  return size() * sizeof(T);
}


template<typename T, std::ptrdiff_t Extent>
inline constexpr bool bit::core::span<T,Extent>::empty()
  const noexcept
{
  return m_storage.size() == 0;
}

//-----------------------------------------------------------------------------
// Element Access
//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::pointer
  bit::core::span<T,Extent>::data()
  noexcept
{
  return m_storage.data();
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::const_pointer
  bit::core::span<T,Extent>::data()
  const noexcept
{
  return m_storage.data();
}

//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::reference
  bit::core::span<T,Extent>::operator[] ( index_type pos )
  noexcept
{
  return m_storage.data()[pos];
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::const_reference
  bit::core::span<T,Extent>::operator[] ( index_type pos )
  const noexcept
{
  return m_storage.data()[pos];
}

//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::reference
  bit::core::span<T,Extent>::at( index_type pos )
{
  BIT_ASSERT_OR_THROW( pos >= 0, std::out_of_range, "span::at: position out of range" );
  BIT_ASSERT_OR_THROW( static_cast<size_type>(pos) < size(), std::out_of_range, "span::at: position out of range" );

  return *(m_storage.data() + pos);
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::const_reference
  bit::core::span<T,Extent>::at( index_type pos )
  const
{
  BIT_ASSERT_OR_THROW( pos >= 0, std::out_of_range, "span::at: position out of range" );
  BIT_ASSERT_OR_THROW( static_cast<size_type>(pos) < size(), std::out_of_range, "span::at: position out of range" );

  return *(m_storage.data() + pos);
}

//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::reference
  bit::core::span<T,Extent>::front()
  noexcept
{
  return *m_storage.data();
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::const_reference
  bit::core::span<T,Extent>::front()
  const noexcept
{
  return *m_storage.data();
}

//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::reference
  bit::core::span<T,Extent>::back()
  noexcept
{
  return *(m_storage.data() + (size() - 1));
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::const_reference
  bit::core::span<T,Extent>::back()
  const noexcept
{
  return *(m_storage.data() + (size() - 1));
}

//-----------------------------------------------------------------------------
// Modifiers
//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline void bit::core::span<T,Extent>::swap( span& other )
  noexcept
{
  using std::swap;

  swap(m_storage,other.m_storage);
}

//-----------------------------------------------------------------------------
// Operations
//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline typename bit::core::span<T,Extent>::size_type
  bit::core::span<T,Extent>::copy( value_type* dest,
                                  size_type count,
                                  size_type pos )
  const
{
  BIT_ASSERT_OR_THROW( pos < size(), std::out_of_range, "span::at: index out of range" );

  const size_type rcount = std::min(size() - pos,count+1);
  std::copy( m_storage.data() + pos, m_storage.data() + pos + rcount, dest);
  return rcount;
}


template<typename T, std::ptrdiff_t Extent>
inline constexpr bit::core::span<T,bit::core::dynamic_extent>
  bit::core::span<T,Extent>::subspan( size_type offset, size_type count )
  const
{
  BIT_ASSERT_OR_THROW( offset < size(), std::out_of_range, "span::at: index out of range" );

  const size_type max_length = offset > size() ? 0 : size() - offset;
  const size_type length     = count == dynamic_extent ? max_length : std::min(count,max_length);

  return span<T,dynamic_extent>( m_storage.data() + offset, length );
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr bit::core::span<T,bit::core::dynamic_extent>
  bit::core::span<T,Extent>::first( size_type n )
  const
{
  return subspan( 0, n );
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr bit::core::span<T,bit::core::dynamic_extent>
  bit::core::span<T,Extent>::last( size_type n )
  const
{
  return (n > size())
         ? *this
         : subspan( size() - n );
}

//-----------------------------------------------------------------------------
// Iterators
//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::iterator
  bit::core::span<T,Extent>::begin()
  const noexcept
{
  return iterator( m_storage.data() );
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::const_iterator
  bit::core::span<T,Extent>::cbegin()
  const noexcept
{
  return const_iterator( m_storage.data() );
}

//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::iterator
  bit::core::span<T,Extent>::end()
  const noexcept
{
  return iterator( m_storage.data() ) + size();
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::const_iterator
  bit::core::span<T,Extent>::cend()
  const noexcept
{
  return const_iterator( m_storage.data() ) + size();
}

//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::reverse_iterator
  bit::core::span<T,Extent>::rbegin()
  const noexcept
{
  if( empty() ) {
    return reverse_iterator( nullptr );
  }
  return reverse_iterator( m_storage.data() ) + (size() - 1);
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::const_reverse_iterator
  bit::core::span<T,Extent>::crbegin()
  const noexcept
{
  if( empty() ) {
    return const_reverse_iterator( nullptr );
  }
  return const_reverse_iterator( m_storage.data() ) + (size() - 1);
}

//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::reverse_iterator
  bit::core::span<T,Extent>::rend()
  const noexcept
{
  if( empty() ) {
    return reverse_iterator( nullptr );
  }
  return reverse_iterator( m_storage.data() ) - 1;
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr typename bit::core::span<T,Extent>::const_reverse_iterator
  bit::core::span<T,Extent>::crend()
  const noexcept
{
  if( empty() ) {
    return const_reverse_iterator( nullptr );
  }
  return const_reverse_iterator( m_storage.data() ) - 1;
}

//=============================================================================
// non-member functions : class : span
//=============================================================================

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline void bit::core::swap( span<T,Extent>& lhs, span<T,Extent>& rhs )
  noexcept
{
  lhs.swap(rhs);
}

//-----------------------------------------------------------------------------

template<std::size_t N, typename T, std::ptrdiff_t Extent>
inline constexpr T& bit::core::get( span<T,Extent>& x )
  noexcept
{
  static_assert( Extent >= 0, "Extent can't be negative" );
  static_assert( static_cast<std::size_t>(Extent) >= N, "N is out of range");

  return x[N];
}

template<std::size_t N, typename T, std::ptrdiff_t Extent>
inline constexpr const T& bit::core::get( const span<T,Extent>& x )
  noexcept
{
  static_assert( Extent >= 0, "Extent can't be negative" );
  static_assert( static_cast<std::size_t>(Extent) >= N, "N is out of range");

  return x[N];
}

template<std::size_t N, typename T, std::ptrdiff_t Extent>
inline constexpr T&& bit::core::get( span<T,Extent>&& x )
  noexcept
{
  static_assert( Extent >= 0, "Extent can't be negative" );
  static_assert( static_cast<std::size_t>(Extent) >= N, "N is out of range");

  return std::move(x[N]);
}

template<std::size_t N, typename T, std::ptrdiff_t Extent>
inline constexpr const T&& bit::core::get( const span<T,Extent>&& x )
  noexcept
{
  static_assert( Extent >= 0, "Extent can't be negative" );
  static_assert( static_cast<std::size_t>(Extent) >= N, "N is out of range");

  return std::move(x[N]);
}

//-----------------------------------------------------------------------------

template<std::size_t N, typename T>
inline constexpr T& bit::core::get( span<T>& x )
{
  return x.at(static_cast<typename span<T>::index_type>(N));
}

template<std::size_t N, typename T>
inline constexpr const T& bit::core::get( const span<T>& x )
{
  return x.at(static_cast<typename span<T>::index_type>(N));
}

template<std::size_t N, typename T>
inline constexpr T&& bit::core::get( span<T>&& x )
{
  return x.at(static_cast<typename span<T>::index_type>(N));
}

template<std::size_t N, typename T>
inline constexpr const T&& bit::core::get( const span<T>&& x )
{
  return x.at(static_cast<typename span<T>::index_type>(N));
}

//----------------------------------------------------------------------------
// Comparison Operators
//----------------------------------------------------------------------------

template<typename T, std::ptrdiff_t Extent>
inline constexpr bool
  bit::core::operator==( const span<T,Extent>& lhs,
                         const span<T,Extent>& rhs )
  noexcept
{
  return std::equal(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr bool
  bit::core::operator!=( const span<T,Extent>& lhs,
                        const span<T,Extent>& rhs )
  noexcept
{
  return !(lhs==rhs);
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr bool
  bit::core::operator<( const span<T,Extent>& lhs,
                        const span<T,Extent>& rhs )
  noexcept
{
  return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr bool
  bit::core::operator<=( const span<T,Extent>& lhs,
                         const span<T,Extent>& rhs )
  noexcept
{
  return !(lhs > rhs);
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr bool
  bit::core::operator>( const span<T,Extent>& lhs,
                        const span<T,Extent>& rhs )
  noexcept
{
  return (rhs < lhs);
}

template<typename T, std::ptrdiff_t Extent>
inline constexpr bool
  bit::core::operator>=( const span<T,Extent>& lhs,
                         const span<T,Extent>& rhs )
  noexcept
{
  return !(lhs < rhs);
}

#endif /* BIT_CORE_CONTAINERS_DETAIL_SPAN_INL */
