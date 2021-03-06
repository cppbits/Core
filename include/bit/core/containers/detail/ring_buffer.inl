#ifndef BIT_CORE_CONTAINERS_DETAIL_RING_BUFFER_INL
#define BIT_CORE_CONTAINERS_DETAIL_RING_BUFFER_INL

//=============================================================================
// class : ring_buffer_iterator
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

template<typename C, typename T>
inline bit::core::detail::ring_buffer_iterator<C,T>
  ::ring_buffer_iterator( C& container, T* ptr, bool compare_twice )
  : m_ptr{ptr},
    m_container{&container},
    m_compare_twice{compare_twice}
{

}

//-----------------------------------------------------------------------------
// Iteration
//-----------------------------------------------------------------------------

template<typename C, typename T>
inline bit::core::detail::ring_buffer_iterator<C,T>&
  bit::core::detail::ring_buffer_iterator<C,T>::operator++()
  noexcept
{
  m_compare_twice = false;
  m_container->increment( m_ptr );
  return (*this);
}

template<typename C, typename T>
inline bit::core::detail::ring_buffer_iterator<C,T>
  bit::core::detail::ring_buffer_iterator<C,T>::operator++(int)
  noexcept
{
  auto iter = (*this);
  m_compare_twice = false;
  m_container->increment( m_ptr );
  return iter;
}

//-----------------------------------------------------------------------------

template<typename C, typename T>
inline bit::core::detail::ring_buffer_iterator<C,T>&
  bit::core::detail::ring_buffer_iterator<C,T>::operator--()
  noexcept
{
  m_compare_twice = false;
  m_container->decrement( m_ptr );
  return (*this);
}

template<typename C, typename T>
inline bit::core::detail::ring_buffer_iterator<C,T>
  bit::core::detail::ring_buffer_iterator<C,T>::operator--(int)
  noexcept
{
  auto iter = (*this);
  m_compare_twice = false;
  m_container->decrement( m_ptr );
  return iter;
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

template<typename C, typename T>
inline typename bit::core::detail::ring_buffer_iterator<C,T>::reference
  bit::core::detail::ring_buffer_iterator<C,T>::operator*()
  noexcept
{
  return *m_ptr;
}

template<typename C, typename T>
inline typename bit::core::detail::ring_buffer_iterator<C,T>::pointer
  bit::core::detail::ring_buffer_iterator<C,T>::operator->()
  noexcept
{
  return m_ptr;
}

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

template<typename C, typename T>
inline bool
  bit::core::detail::operator==( const ring_buffer_iterator<C,T>& lhs,
                                const ring_buffer_iterator<C,T>& rhs )
  noexcept
{
  return lhs.m_compare_twice == rhs.m_compare_twice && lhs.m_ptr == rhs.m_ptr;
}

template<typename C, typename T>
inline bool
  bit::core::detail::operator!=( const ring_buffer_iterator<C,T>& lhs,
                                const ring_buffer_iterator<C,T>& rhs )
  noexcept
{
  return !(lhs==rhs);
}

//=============================================================================
// ring_buffer
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

template<typename T>
inline bit::core::ring_buffer<T>::ring_buffer()
  noexcept
  : ring_buffer( nullptr )
{

}

template<typename T>
inline bit::core::ring_buffer<T>::ring_buffer( std::nullptr_t )
  noexcept
  : ring_buffer(nullptr,0)
{

}

template<typename T>
inline bit::core::ring_buffer<T>::ring_buffer( void* buffer,
                                                      size_type size )
  noexcept
  : m_buffer(static_cast<T*>(buffer)),
    m_begin(m_buffer),
    m_end(m_buffer),
    m_capacity(size),
    m_size(0)
{

}

template<typename T>
inline bit::core::ring_buffer<T>
  ::ring_buffer( ring_buffer&& other )
  noexcept
  : m_buffer( other.m_buffer ),
    m_begin( other.m_begin ),
    m_end( other.m_end ),
    m_capacity( other.m_capacity ),
    m_size( other.m_size )
{
  other.m_size     = 0;
  other.m_capacity = 0;
  other.m_buffer   = nullptr;
}

//-----------------------------------------------------------------------------

template<typename T>
inline bit::core::ring_buffer<T>::~ring_buffer()
{
  clear();
}

//-----------------------------------------------------------------------------

template<typename T>
bit::core::ring_buffer<T>&
  bit::core::ring_buffer<T>::operator=( ring_buffer other )
  noexcept
{
  other.swap(*this);

  return (*this);
}

//-----------------------------------------------------------------------------
// Modifiers
//-----------------------------------------------------------------------------

template<typename T>
template<typename...Args,typename>
inline typename bit::core::ring_buffer<T>::reference
  bit::core::ring_buffer<T>::emplace_back( Args&&...args )
{
  if(full()) {
    destroy_at( m_begin );
    increment( m_begin );
  } else {
    ++m_size;
  }

#if BIT_COMPILER_EXCEPTIONS_ENABLED
  try {
#endif
    uninitialized_construct_at<T>(m_end, std::forward<Args>(args)... );
#if BIT_COMPILER_EXCEPTIONS_ENABLED
  } catch (...) {
    --m_size;
    throw;
  }
#endif
  auto result = m_end;
  increment( m_end );

  return (*result);
}

template<typename T>
template<typename...Args,typename>
inline typename bit::core::ring_buffer<T>::reference
  bit::core::ring_buffer<T>::emplace_front( Args&&...args )
{
  if(full()) {
    decrement( m_end );
    destroy_at( m_end );
  } else {
    ++m_size;
  }
  decrement( m_begin );

#if BIT_COMPILER_EXCEPTIONS_ENABLED
  try {
#endif
    uninitialized_construct_at<T>(m_begin, std::forward<Args>(args)... );
#if BIT_COMPILER_EXCEPTIONS_ENABLED
  } catch (...) {
    increment( m_begin );
    --m_size;
    throw;
  }
#endif
  return (*m_begin);
}

//----------------------------------------------------------------------

template<typename T>
template<typename U,typename>
inline void bit::core::ring_buffer<T>::push_back( const T& value )
{
  emplace_back( value );
}

template<typename T>
template<typename U,typename>
inline void bit::core::ring_buffer<T>::push_back( T&& value )
{
  emplace_back( std::move(value) );
}

//----------------------------------------------------------------------

template<typename T>
template<typename U,typename>
inline void bit::core::ring_buffer<T>::push_front( const T& value )
{
  emplace_front( value );
}

template<typename T>
template<typename U,typename>
inline void bit::core::ring_buffer<T>::push_front( T&& value )
{
  emplace_front( std::move(value) );
}

//----------------------------------------------------------------------

template<typename T>
inline void bit::core::ring_buffer<T>::pop_front()
{
  destroy_at( m_begin );
  increment( m_begin );
  --m_size;
}

template<typename T>
inline void bit::core::ring_buffer<T>::pop_back()
{
  destroy_at( m_end );
  decrement( m_end );
  --m_size;
}

//----------------------------------------------------------------------

template<typename T>
inline void bit::core::ring_buffer<T>::clear()
  noexcept
{
  while( !empty() ) {
    pop_back();
  }
}

template<typename T>
inline void bit::core::ring_buffer<T>::swap( ring_buffer& other )
  noexcept
{
  using std::swap;

  swap(m_buffer, other.m_buffer);
  swap(m_begin, other.m_begin);
  swap(m_end, other.m_end);
  swap(m_capacity, other.m_capacity);
  swap(m_size, other.m_size);
}

//-----------------------------------------------------------------------------
// Capacity
//-----------------------------------------------------------------------------

template<typename T>
inline bool bit::core::ring_buffer<T>::empty()
  const noexcept
{
  return m_size == 0u;
}

template<typename T>
inline bool bit::core::ring_buffer<T>::full()
  const noexcept
{
  return m_size == m_capacity;
}

//-----------------------------------------------------------------------------

template<typename T>
inline typename bit::core::ring_buffer<T>::size_type
  bit::core::ring_buffer<T>::size()
  const noexcept
{
  return m_size;
}

template<typename T>
inline typename bit::core::ring_buffer<T>::size_type
  bit::core::ring_buffer<T>::max_size()
  const noexcept
{
  return m_capacity;
}

template<typename T>
inline typename bit::core::ring_buffer<T>::size_type
  bit::core::ring_buffer<T>::capacity()
  const noexcept
{
  return m_capacity;
}

//-----------------------------------------------------------------------------
// Element Access
//-----------------------------------------------------------------------------

template<typename T>
inline typename bit::core::ring_buffer<T>::reference
  bit::core::ring_buffer<T>::front()
  noexcept
{
  return *m_begin;
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_reference
  bit::core::ring_buffer<T>::front()
  const noexcept
{
  return *m_begin;
}

//-----------------------------------------------------------------------------

template<typename T>
inline typename bit::core::ring_buffer<T>::reference
  bit::core::ring_buffer<T>::back()
  noexcept
{
  auto ptr = m_end;
  return *decrement( ptr );
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_reference
  bit::core::ring_buffer<T>::back()
  const noexcept
{
  auto ptr = m_end;
  return *decrement( ptr );
}

//-----------------------------------------------------------------------------

template<typename T>
inline typename bit::core::ring_buffer<T>::pointer
  bit::core::ring_buffer<T>::data()
  noexcept
{
  return m_buffer;
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_pointer
  bit::core::ring_buffer<T>::data()
  const noexcept
{
  return m_buffer;
}

//-----------------------------------------------------------------------------
// Iteration
//-----------------------------------------------------------------------------

template<typename T>
inline typename bit::core::ring_buffer<T>::iterator
  bit::core::ring_buffer<T>::begin()
  noexcept
{
  return iterator{ (*this), m_begin, (capacity()) > 0 && full() };
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_iterator
  bit::core::ring_buffer<T>::begin()
  const noexcept
{
  return const_iterator{ (*this), m_begin, (capacity()) > 0 && full() };
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_iterator
  bit::core::ring_buffer<T>::cbegin()
  const noexcept
{
  return const_iterator{ (*this), m_begin, (capacity()) > 0 && full() };
}

//-----------------------------------------------------------------------------

template<typename T>
inline typename bit::core::ring_buffer<T>::iterator
  bit::core::ring_buffer<T>::end()
  noexcept
{
  return iterator{ (*this), m_end, false };
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_iterator
  bit::core::ring_buffer<T>::end()
  const noexcept
{
  return const_iterator{ (*this), m_end, false };
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_iterator
  bit::core::ring_buffer<T>::cend()
  const noexcept
{
  return const_iterator{ (*this), m_end, false };
}

//-----------------------------------------------------------------------------

template<typename T>
inline typename bit::core::ring_buffer<T>::reverse_iterator
  bit::core::ring_buffer<T>::rbegin()
  noexcept
{
  return reverse_iterator{ iterator{(*this), m_end, (capacity()) > 0 && full()} };
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_reverse_iterator
  bit::core::ring_buffer<T>::rbegin()
  const noexcept
{
  return const_reverse_iterator{ const_iterator{(*this), m_end, (capacity()) > 0 && full()} };
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_reverse_iterator
  bit::core::ring_buffer<T>::crbegin()
  const noexcept
{
  return const_reverse_iterator{ const_iterator{(*this), m_end, (capacity()) > 0 && full()} };
}

//-----------------------------------------------------------------------------

template<typename T>
inline typename bit::core::ring_buffer<T>::reverse_iterator
  bit::core::ring_buffer<T>::rend()
  noexcept
{
  return reverse_iterator{ iterator{(*this), m_begin, false} };
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_reverse_iterator
  bit::core::ring_buffer<T>::rend()
  const noexcept
{
  return const_reverse_iterator{ const_iterator{(*this), m_begin, false} };
}

template<typename T>
inline typename bit::core::ring_buffer<T>::const_reverse_iterator
  bit::core::ring_buffer<T>::crend()
  const noexcept
{
  return const_reverse_iterator{ const_iterator{(*this), m_begin, false} };
}
//-----------------------------------------------------------------------------
// Private Members
//-----------------------------------------------------------------------------

template<typename T>
inline T*& bit::core::ring_buffer<T>::increment( T*& iter )
  noexcept
{
  if( iter == &m_buffer[m_capacity-1] ) {
    return (iter = &m_buffer[0]);
  }
  return ++iter;
}

template<typename T>
inline const T*& bit::core::ring_buffer<T>::increment( const T*& iter )
  const noexcept
{
  if( iter == &m_buffer[m_capacity-1] ) {
    return (iter = &m_buffer[0]);
  }
  return ++iter;
}

//-----------------------------------------------------------------------------

template<typename T>
inline T*& bit::core::ring_buffer<T>::decrement( T*& iter )
  noexcept
{
  if( iter == &m_buffer[0] ) {
    return (iter = &m_buffer[m_capacity-1]);
  }
  return --iter;
}

template<typename T>
inline const T*& bit::core::ring_buffer<T>::decrement( const T*& iter )
  const noexcept
{
  if( iter == &m_buffer[0] ) {
    return (iter = &m_buffer[m_capacity-1]);
  }
  return --iter;
}

//-----------------------------------------------------------------------------
// Free-functions
//-----------------------------------------------------------------------------

template<typename T>
void bit::core::swap( ring_buffer<T>& lhs, ring_buffer<T>& rhs )
  noexcept
{
  lhs.swap(rhs);
}

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

template<typename T>
bool bit::core::operator==( const ring_buffer<T>& lhs,
                            const ring_buffer<T>& rhs )
  noexcept
{
  return std::equal(lhs.begin(),lhs.end(),rhs.begin(),lhs.end());
}

template<typename T>
bool bit::core::operator!=( const ring_buffer<T>& lhs,
                            const ring_buffer<T>& rhs )
  noexcept
{
  return !(lhs==rhs);
}

template<typename T>
bool bit::core::operator<( const ring_buffer<T>& lhs,
                           const ring_buffer<T>& rhs )
  noexcept
{
  return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end());
}

template<typename T>
bool bit::core::operator>( const ring_buffer<T>& lhs,
                           const ring_buffer<T>& rhs )
  noexcept
{
  return (rhs<lhs);
}

template<typename T>
bool bit::core::operator<=( const ring_buffer<T>& lhs,
                            const ring_buffer<T>& rhs )
  noexcept
{
  return !(rhs<lhs);
}

template<typename T>
bool bit::core::operator>=( const ring_buffer<T>& lhs,
                            const ring_buffer<T>& rhs )
  noexcept
{
  return !(lhs<rhs);
}

#endif /* BIT_CORE_CONTAINERS_DETAIL_RING_BUFFER_INL */
