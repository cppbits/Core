#ifndef BIT_CORE_UTILITIES_DETAIL_DELEGATE_INL
#define BIT_CORE_UTILITIES_DETAIL_DELEGATE_INL

//=============================================================================
// class : delegate
//=============================================================================

//-----------------------------------------------------------------------------
// Constructor
//-----------------------------------------------------------------------------

template<typename R, typename ... Types>
constexpr bit::core::delegate<R(Types...)>::delegate()
  noexcept
  : m_instance{nullptr},
    m_function{nullptr}
{

}

//-----------------------------------------------------------------------------
// Function Binding
//-----------------------------------------------------------------------------

template<typename R, typename ... Types>
template<bit::core::function_pointer<R(Types...)> Function>
inline constexpr void
  bit::core::delegate<R(Types...)>::bind()
  noexcept
{
  const auto callback = [](void*, Types...args )
  {
    return invoke( Function, std::forward<Types>(args)... );
  };
  m_instance = nullptr;
  m_function = callback;
}

//-----------------------------------------------------------------------------

template<typename R, typename ... Types>
template<typename C, bit::core::member_function_pointer<C,R(Types...)> MemberFunction>
inline constexpr void
  bit::core::delegate<R(Types...)>::bind( C& instance )
  noexcept
{
  const auto callback = [](void* ptr, Types...args )
  {
    return invoke( MemberFunction, static_cast<C*>(ptr), std::forward<Types>(args)... );
  };
  m_instance = static_cast<void*>( std::addressof(instance) );
  m_function = callback;
}

//-----------------------------------------------------------------------------

template<typename R, typename ... Types>
template<typename C, bit::core::member_function_pointer<const C,R(Types...)> MemberFunction>
inline constexpr void
  bit::core::delegate<R(Types...)>::bind( const C& instance )
  noexcept
{
  cbind<C,MemberFunction>( instance );
}

template<typename R, typename ... Types>
template<typename C, bit::core::member_function_pointer<const C,R(Types...)> MemberFunction>
inline constexpr void
  bit::core::delegate<R(Types...)>::cbind( const C& instance )
  noexcept
{
  const auto callback = [](void* ptr, Types...args )
  {
    return invoke( MemberFunction, static_cast<const C*>(ptr), std::forward<Types>(args)... );
  };
  m_instance = static_cast<void*>( const_cast<C*>( std::addressof(instance) ) ); // needed for const
  m_function = callback;
}

//-----------------------------------------------------------------------------
// Queries
//-----------------------------------------------------------------------------

template<typename R, typename ... Types>
inline constexpr bool
  bit::core::delegate<R(Types...)>::is_bound()
  const noexcept
{
  return m_function != nullptr;
}

template<typename R, typename...Types>
inline constexpr bit::core::delegate<R(Types...)>::operator bool()
  const noexcept
{
  return m_function != nullptr;
}

//-----------------------------------------------------------------------------
// Invocation
//-----------------------------------------------------------------------------

template<typename R, typename ... Types>
template<typename...Args,typename>
inline constexpr typename bit::core::delegate<R(Types...)>::return_type
  bit::core::delegate<R(Types...)>::operator()( Args&&... args )
  const
{
  BIT_ASSERT( m_function != nullptr,
              "delegate::operator(): cannot invoke unbound delegate");

  return invoke( m_function, m_instance, std::forward<Args>(args)... );
}

//=============================================================================
// non-member functions : class : delegate
//=============================================================================

//-----------------------------------------------------------------------------
// Comparisons
//-----------------------------------------------------------------------------

template<typename Fn>
inline bool bit::core::operator==( const delegate<Fn>& lhs,
                                   const delegate<Fn>& rhs )
  noexcept
{
  return lhs.m_instance == rhs.m_instance &&
         lhs.m_function == rhs.m_function;
}

template<typename Fn>
inline bool bit::core::operator!=( const delegate<Fn>& lhs,
                                   const delegate<Fn>& rhs )
  noexcept
{
  return !(lhs==rhs);
}

template<typename Fn>
inline bool bit::core::operator<( const delegate<Fn>& lhs,
                                  const delegate<Fn>& rhs )
  noexcept
{
  return lhs.m_instance == rhs.m_instance
       ? lhs.m_function < rhs.m_function
       : lhs.m_instance < rhs.m_instance;
}

template<typename Fn>
inline bool bit::core::operator>( const delegate<Fn>& lhs,
                                  const delegate<Fn>& rhs )
  noexcept
{
  return (rhs < lhs);
}

template<typename Fn>
inline bool bit::core::operator<=( const delegate<Fn>& lhs,
                                   const delegate<Fn>& rhs )
  noexcept
{
  return !(rhs < lhs);
}

template<typename Fn>
inline bool bit::core::operator>=( const delegate<Fn>& lhs,
                                   const delegate<Fn>& rhs )
  noexcept
{
  return !(lhs < rhs);
}

//-----------------------------------------------------------------------------
// Utilities
//-----------------------------------------------------------------------------

template<typename Fn>
inline bit::core::hash_t bit::core::hash_value( const delegate<Fn>& fn )
  noexcept
{
  return hash_values( fn.m_function, fn.m_instance );
}


#endif /* BIT_CORE_UTILITIES_DETAIL_DELEGATE_INL */
