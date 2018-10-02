#ifndef BIT_CORE_MEMORY_DETAIL_ALLOCATOR_DESTRUCTOR_INL
#define BIT_CORE_MEMORY_DETAIL_ALLOCATOR_DESTRUCTOR_INL

//=============================================================================
// allocator_destructor
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

template<typename Allocator>
bit::core::allocator_deleter<Allocator>
  ::allocator_deleter( Allocator& alloc, size_type size )
  noexcept
  : m_allocator(alloc),
    m_size(size)
{

}

//-----------------------------------------------------------------------------
// Modifiers
//-----------------------------------------------------------------------------

template<typename Allocator>
void bit::core::allocator_deleter<Allocator>::operator()( pointer p )
  noexcept
{
  alloc_traits::destroy( m_allocator, p );
  alloc_traits::deallocate( m_allocator, p, m_size );
}

#endif /* BIT_CORE_MEMORY_DETAIL_ALLOCATOR_DESTRUCTOR_INL */
