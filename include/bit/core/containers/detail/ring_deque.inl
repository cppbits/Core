#ifndef BIT_CORE_CONTAINERS_DETAIL_RING_DEQUE_INL
#define BIT_CORE_CONTAINERS_DETAIL_RING_DEQUE_INL

//=============================================================================
// ring_deque
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
bit::core::ring_deque<T,Allocator>::ring_deque()
  : ring_deque( Allocator() )
{

}

template<typename T, typename Allocator>
bit::core::ring_deque<T,Allocator>::ring_deque( const Allocator& alloc )
  : ring_deque( 0, alloc )
{

}

template<typename T, typename Allocator>
bit::core::ring_deque<T,Allocator>
  ::ring_deque( size_type count, const T& value, const Allocator& alloc )
  : ring_deque( count, alloc )
{
  while(!full()) {
    m_storage.buffer().emplace_back(value);
  }
}

template<typename T, typename Allocator>
bit::core::ring_deque<T,Allocator>
  ::ring_deque( size_type count, const Allocator& alloc )
  : m_storage( count, alloc )
{

}

template<typename T, typename Allocator>
bit::core::ring_deque<T,Allocator>
  ::ring_deque( const ring_deque& other )
  : ring_deque( other.capacity(), other.get_allocator() )
{
  for( auto const& v : other.m_storage.buffer() ) {
    m_storage.buffer().emplace_back(v);
  }
}


template<typename T, typename Allocator>
bit::core::ring_deque<T,Allocator>
  ::ring_deque( const ring_deque& other, const Allocator& alloc )
  : ring_deque( other.capacity(), alloc )
{
  for( auto const& v : other.m_storage.buffer() ) {
    m_storage.buffer().emplace_back(v);
  }
}


template<typename T, typename Allocator>
bit::core::ring_deque<T,Allocator>
  ::ring_deque( ring_deque&& other )
  : ring_deque( std::move(other), other.get_allocator() )
{

}

template<typename T, typename Allocator>
bit::core::ring_deque<T,Allocator>
  ::ring_deque( ring_deque&& other, const Allocator& alloc )
  : m_storage( std::move(other.m_storage), alloc )
{

}

//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
bit::core::ring_deque<T,Allocator>&
  bit::core::ring_deque<T,Allocator>::operator=( ring_deque other )
{
 swap(*this,other);

  return (*this);
}

//-----------------------------------------------------------------------------
// Element Access
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::allocator_type
  bit::core::ring_deque<T,Allocator>::get_allocator()
  const
{
  return m_storage.get_allocator();
}

//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::reference
  bit::core::ring_deque<T,Allocator>::front()
  noexcept
{
  return m_storage.buffer().front();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::const_reference
  bit::core::ring_deque<T,Allocator>::front()
  const noexcept
{
  return m_storage.buffer().front();
}

//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::reference
  bit::core::ring_deque<T,Allocator>::back()
  noexcept
{
  return m_storage.buffer().back();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::const_reference
  bit::core::ring_deque<T,Allocator>::back()
  const noexcept
{
  return m_storage.buffer().back();
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
bool bit::core::ring_deque<T,Allocator>::empty()
  const noexcept
{
  return m_storage.buffer().empty();
}

template<typename T, typename Allocator>
bool bit::core::ring_deque<T,Allocator>::full()
  const noexcept
{
  return m_storage.buffer().full();
}

//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::size_type
  bit::core::ring_deque<T,Allocator>::size()
  const noexcept
{
  return m_storage.buffer().size();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::size_type
  bit::core::ring_deque<T,Allocator>::max_size()
  const noexcept
{
  return m_storage.buffer().max_size();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::size_type
  bit::core::ring_deque<T,Allocator>::capacity()
  const noexcept
{
  return m_storage.buffer().capacity();
}

//-----------------------------------------------------------------------------
// Modifiers
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline void bit::core::ring_deque<T,Allocator>::resize( size_type n )
{
  if( m_storage.buffer().capacity() > n ) {
    return;
  }

  resize( std::is_nothrow_move_constructible<T>{}, n );
}

template<typename T, typename Allocator>
template<typename...Args,typename>
typename bit::core::ring_deque<T,Allocator>::reference
  bit::core::ring_deque<T,Allocator>::emplace_back( Args&&...args )
{
  return m_storage.buffer().emplace_back( std::forward<Args>(args)... );
}

template<typename T, typename Allocator>
template<typename...Args,typename>
typename bit::core::ring_deque<T,Allocator>::reference
  bit::core::ring_deque<T,Allocator>::emplace_front( Args&&...args )
{
  return m_storage.buffer().emplace_front( std::forward<Args>(args)... );
}

//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
template<typename U, typename>
void bit::core::ring_deque<T,Allocator>::push_back( const value_type& value )
{
  m_storage.buffer().push_back(value);
}

template<typename T, typename Allocator>
template<typename U, typename>
void bit::core::ring_deque<T,Allocator>::push_back( value_type&& value )
{
  m_storage.buffer().push_back( std::move(value) );
}

//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
template<typename U, typename>
void bit::core::ring_deque<T,Allocator>::push_front( const value_type& value )
{
  m_storage.buffer().push_front(value);
}

template<typename T, typename Allocator>
template<typename U, typename>
void bit::core::ring_deque<T,Allocator>::push_front( value_type&& value )
{
  m_storage.buffer().push_front( std::move(value) );
}

//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
void bit::core::ring_deque<T,Allocator>::pop_front()
{
  m_storage.buffer().pop_front();
}

template<typename T, typename Allocator>
void bit::core::ring_deque<T,Allocator>::pop_back()
{
  m_storage.buffer().pop_back();
}

//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
void bit::core::ring_deque<T,Allocator>::swap( ring_deque& other )
  noexcept
{
  m_storage.swap( other.m_storage );
}

//-----------------------------------------------------------------------------
// Iterators
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::iterator
  bit::core::ring_deque<T,Allocator>::begin()
  noexcept
{
  return m_storage.buffer().begin();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::const_iterator
  bit::core::ring_deque<T,Allocator>::begin()
  const noexcept
{
  return m_storage.buffer().begin();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::const_iterator
  bit::core::ring_deque<T,Allocator>::cbegin()
  const noexcept
{
  return m_storage.buffer().cbegin();
}

//-----------------------------------------------------------------------------


template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::iterator
  bit::core::ring_deque<T,Allocator>::end()
  noexcept
{
  return m_storage.buffer().end();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::const_iterator
  bit::core::ring_deque<T,Allocator>::end()
  const noexcept
{
  return m_storage.buffer().end();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::const_iterator
  bit::core::ring_deque<T,Allocator>::cend()
  const noexcept
{
  return m_storage.buffer().cend();
}

//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::reverse_iterator
  bit::core::ring_deque<T,Allocator>::rbegin()
  noexcept
{
  return m_storage.buffer().rbegin();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::const_reverse_iterator
  bit::core::ring_deque<T,Allocator>::rbegin()
  const noexcept
{
  return m_storage.buffer().rbegin();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::const_reverse_iterator
  bit::core::ring_deque<T,Allocator>::crbegin()
  const noexcept
{
  return m_storage.buffer().crbegin();
}

//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::reverse_iterator
  bit::core::ring_deque<T,Allocator>::rend()
  noexcept
{
  return m_storage.buffer().rend();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::const_reverse_iterator
  bit::core::ring_deque<T,Allocator>::rend()
  const noexcept
{
  return m_storage.buffer().rend();
}

template<typename T, typename Allocator>
typename bit::core::ring_deque<T,Allocator>::const_reverse_iterator
  bit::core::ring_deque<T,Allocator>::crend()
  const noexcept
{
  return m_storage.buffer().crend();
}

//-----------------------------------------------------------------------------
// Private Modifiers
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline void bit::core::ring_deque<T,Allocator>::resize( std::true_type,
                                                           size_type n )
{
  auto storage = storage_type{ n, m_storage.get_allocator() };

  for( auto&& v : m_storage.buffer() ) {
    storage.buffer().emplace_back( std::move(v) );
  }

  // Swap the active storage
  m_storage.buffer().swap( storage.buffer() );
}

template<typename T, typename Allocator>
inline void bit::core::ring_deque<T,Allocator>::resize( std::false_type,
                                                           size_type n )
{
  auto storage = storage_type{ n, m_storage.get_allocator() };

  for( const auto& v : m_storage.buffer() ) {
    storage.buffer().emplace_back( v );
  }

  // Swap the active storage
  m_storage.buffer().swap( storage.buffer() );
}

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline void bit::core::swap( ring_deque<T,Allocator>& lhs,
                            ring_deque<T,Allocator>& rhs )
  noexcept
{
  lhs.swap(rhs);
}

//-----------------------------------------------------------------------------
// Equality
//-----------------------------------------------------------------------------

template<typename T, typename Allocator>
inline bool bit::core::operator==( const ring_deque<T,Allocator>& lhs,
                                  const ring_deque<T,Allocator>& rhs )
  noexcept
{
  return std::equal(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

template<typename T, typename Allocator>
inline bool bit::core::operator!=( const ring_deque<T,Allocator>& lhs,
                                  const ring_deque<T,Allocator>& rhs )
  noexcept
{
  return !(lhs==rhs);
}

#endif /* BIT_CORE_CONTAINERS_DETAIL_RING_DEQUE_INL */
