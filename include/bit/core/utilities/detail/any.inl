#ifndef BIT_CORE_UTILITIES_DETAIL_ANY_INL
#define BIT_CORE_UTILITIES_DETAIL_ANY_INL

//=============================================================================
// definitions : class : bad_any_cast
//=============================================================================

inline const char* bit::core::bad_any_cast::what()
  const noexcept
{
  return "bad_any_cast";
}

//=============================================================================
// class : any::internal_storage_handler
//=============================================================================

template<typename T>
struct bit::core::any::internal_storage_handler
{
  template<typename...Args>
  static T* construct( storage& s, Args&&...args );

  template<typename U, typename...Args>
  static T* construct( storage& s, std::initializer_list<U> il, Args&&...args );

  static void destroy( storage& s );

  static const void* handle( operation op,
                             const storage* self,
                             const storage* other );
};

//=============================================================================
// definition : class : any::internal_storage_handler
//=============================================================================

template<typename T>
template<typename...Args>
inline T* bit::core::any::internal_storage_handler<T>
  ::construct( storage& s, Args&&...args )
{
  return ::new(&s.internal) T( std::forward<Args>(args)... );
}

template<typename T>
template<typename U, typename...Args>
inline T* bit::core::any::internal_storage_handler<T>
  ::construct( storage& s, std::initializer_list<U> il, Args&&...args )
{
  return ::new(&s.internal) T( il, std::forward<Args>(args)... );
}

template<typename T>
inline void bit::core::any::internal_storage_handler<T>
  ::destroy( storage& s )
{
  auto* t = static_cast<T*>(static_cast<void*>(&s.internal));
  t->~T();
}

template<typename T>
inline const void* bit::core::any::internal_storage_handler<T>
  ::handle( operation op,
            const storage* self,
            const storage* other )
{
  switch (op)
  {
    case operation::destroy:
    {
      BIT_ASSERT_AND_ASSUME( self != nullptr );
      BIT_UNUSED( other );

      destroy( const_cast<storage&>(*self) );
      break;
    }

    case operation::copy:
    {
      BIT_ASSERT_AND_ASSUME( self != nullptr );
      BIT_ASSERT_AND_ASSUME( other != nullptr );

      // Copy construct from the internal storage
      const auto* p = reinterpret_cast<const T*>(&other->internal);
      construct( const_cast<storage&>(*self), *p);
      break;
    }

    case operation::move:
    {
      BIT_ASSERT_AND_ASSUME( self != nullptr );
      BIT_ASSERT_AND_ASSUME( other != nullptr );

      // Move construct from the internal storage. '
      const auto* p = reinterpret_cast<const T*>(&other->internal);
      construct( const_cast<storage&>(*self), std::move(*const_cast<T*>(p)) );
      break;
    }

    case operation::value:
    {
      BIT_ASSERT_AND_ASSUME( self != nullptr );
      BIT_UNUSED( other );

      // NOTE(bitwize): This seemingly arbitrary conversion is for proper
      //   type-safety/correctness. Otherwise, converting an aligned_storage_t*
      //   to void* and then to T* would violate strict-aliasing -- which
      //   would be undefined-behavior. Behavior is only well-defined for
      //   casts from void* to T* if the the void* originated from a T*.
      const auto* p = reinterpret_cast<const T*>(&self->internal);
      return static_cast<const void*>(p);
    }

    case operation::type:
    {
      BIT_UNUSED( self );
      BIT_UNUSED( other );

      return static_cast<const void*>( &typeid(T) );
    }
    default:
      BIT_UNREACHABLE();
  }
  return nullptr;
}

//=============================================================================
// class : any::external_storage_handler
//=============================================================================

template<typename T>
struct bit::core::any::external_storage_handler
{
  template<typename...Args>
  static T* construct( storage& s, Args&&...args );

  template<typename U, typename...Args>
  static T* construct( storage& s, std::initializer_list<U> il, Args&&...args );

  static void destroy( storage& s );

  static const void* handle( operation op,
                             const storage* self,
                             const storage* other );
};


//=============================================================================
// definition : class : any::external_storage_handler
//=============================================================================

template<typename T>
template<typename...Args>
inline T* bit::core::any::external_storage_handler<T>
  ::construct( storage& s, Args&&...args )
{
  s.external = new T( std::forward<Args>(args)... );
  return static_cast<T*>(s.external);
}

template<typename T>
template<typename U, typename...Args>
inline T* bit::core::any::external_storage_handler<T>
  ::construct( storage& s, std::initializer_list<U> il, Args&&...args )
{
  s.external = new T( il, std::forward<Args>(args)... );
  return static_cast<T*>(s.external);
}

template<typename T>
inline void bit::core::any::external_storage_handler<T>
  ::destroy( storage& s )
{
  delete static_cast<T*>(s.external);
}

template<typename T>
inline const void* bit::core::any::external_storage_handler<T>
  ::handle( operation op,
            const storage* self,
            const storage* other )
{
  switch (op)
  {
    case operation::destroy:
    {
      BIT_ASSERT_AND_ASSUME( self != nullptr );
      BIT_UNUSED( other );

      destroy( const_cast<storage&>(*self) );
      break;
    }

    case operation::copy:
    {
      BIT_ASSERT_AND_ASSUME( self != nullptr );
      BIT_ASSERT_AND_ASSUME( other != nullptr );

      // Copy construct from the internal storage
      construct( const_cast<storage&>(*self),
                 *static_cast<const T*>(other->external));
      break;
    }

    case operation::move:
    {
      BIT_UNUSED( self != nullptr );
      BIT_ASSERT_AND_ASSUME( other != nullptr );

      const auto p = static_cast<const T*>(other->external);
      // Move construct from the internal storage. '
      construct( const_cast<storage&>(*self), std::move(*const_cast<T*>(p)) );
      break;
    }

    case operation::value:
    {
      BIT_ASSERT_AND_ASSUME( self != nullptr );
      BIT_UNUSED( other );

      // self->external was already created as a T*; no need to cast like in
      // internal.
      return self->external;
    }

    case operation::type:
    {
      BIT_UNUSED( self );
      BIT_UNUSED( other );

      return &typeid(T);
    }
    default:
      BIT_UNREACHABLE();
  }
  return nullptr;
}

//=============================================================================
// definitions : class : any
//=============================================================================

//-----------------------------------------------------------------------------
// Constructors / Destructor / Assignment
//-----------------------------------------------------------------------------

inline bit::core::any::any()
  noexcept
  : m_storage{},
    m_storage_handler{nullptr}
{

}

inline bit::core::any::any( any&& other )
  noexcept
  : m_storage{},
    m_storage_handler{other.m_storage_handler}
{
  if( m_storage_handler != nullptr ) {
    m_storage_handler( operation::move, &m_storage, &other.m_storage );
  }
}

inline bit::core::any::any( const any& other )
  : m_storage{},
    m_storage_handler{nullptr}
{

  if( other.m_storage_handler != nullptr ) {
    // Set handler after constructing, in case of exception
    const auto handler = other.m_storage_handler;

    handler( operation::copy, &m_storage, &other.m_storage );
    m_storage_handler = handler;
  }
}

template<typename ValueType, typename>
inline bit::core::any::any( ValueType&& value )
  : m_storage{},
    m_storage_handler{nullptr}
{
  // Set handler after constructing, in case of exception
  using handler_type = storage_handler<std::decay_t<ValueType>>;

  handler_type::construct( m_storage, std::forward<ValueType>(value) );
  m_storage_handler = &handler_type::handle;
}

template<typename ValueType, typename...Args, typename>
inline bit::core::any::any( in_place_type_t<ValueType>, Args&&...args )
  : m_storage{},
    m_storage_handler{nullptr}
{
  // Set handler after constructing, in case of exception
  using handler_type = storage_handler<std::decay_t<ValueType>>;

  handler_type::construct( m_storage, std::forward<Args>(args)... );
  m_storage_handler = &handler_type::handle;
}

template<typename ValueType, typename U, typename...Args, typename>
inline bit::core::any::any( in_place_type_t<ValueType>,
                            std::initializer_list<U> il,
                            Args&&...args )
  : m_storage{},
    m_storage_handler{nullptr}
{
  // Set handler after constructing, in case of exception
  using handler_type = storage_handler<std::decay_t<ValueType>>;

  handler_type::construct( m_storage, il, std::forward<Args>(args)... );
  m_storage_handler = &handler_type::handle;
}

//-----------------------------------------------------------------------------

inline bit::core::any::~any()
{
  reset();
}

//-----------------------------------------------------------------------------

inline bit::core::any& bit::core::any::operator=( any&& other )
  noexcept
{
  reset();

  if( other.m_storage_handler != nullptr ) {
    m_storage_handler = other.m_storage_handler;
    m_storage_handler( operation::move, &m_storage, &other.m_storage );
  }

  return (*this);
}

inline bit::core::any& bit::core::any::operator=( const any& other )
{
  reset();

  if( other.m_storage_handler != nullptr ) {
    // Set handler after constructing, in case of exception
    const auto handler = other.m_storage_handler;

    handler( operation::copy, &m_storage, &other.m_storage );
    m_storage_handler = handler;
  }

  return (*this);
}

template<typename ValueType, typename>
inline bit::core::any& bit::core::any::operator=( ValueType&& value )
{
  using handler_type = storage_handler<std::decay_t<ValueType>>;

  reset();

  handler_type::construct( m_storage, std::forward<ValueType>(value) );
  m_storage_handler = &handler_type::handle;

  return (*this);
}

//-----------------------------------------------------------------------------
// Modifiers
//-----------------------------------------------------------------------------

template<typename ValueType, typename...Args, typename>
inline std::decay_t<ValueType>&
  bit::core::any::emplace( Args&&...args )
{
  using handler_type = storage_handler<std::decay_t<ValueType>>;

  reset();

  auto& result = *handler_type::construct( m_storage,
                                           std::forward<Args>(args)... );
  m_storage_handler = &handler_type::handle;

  return result;
}

template<typename ValueType, typename U, typename...Args, typename>
inline std::decay_t<ValueType>&
  bit::core::any::emplace( std::initializer_list<U> il,
                           Args&&...args )
{
  using handler_type = storage_handler<std::decay_t<ValueType>>;

  reset();

  auto& result = *handler_type::construct( m_storage,
                                           il,
                                           std::forward<Args>(args)... );
  m_storage_handler = &handler_type::handle;

  return result;
}

inline void bit::core::any::reset()
  noexcept
{
  if( m_storage_handler != nullptr ) {
    m_storage_handler( operation::destroy, &m_storage, nullptr );
    m_storage_handler = nullptr;
  }
}

inline void bit::core::any::swap( any& other )
  noexcept
{
  using std::swap;

  if(m_storage_handler != nullptr && other.m_storage_handler != nullptr)
  {
    auto tmp = any{};

    // tmp := self
    tmp.m_storage_handler = m_storage_handler;
    m_storage_handler( operation::move, &tmp.m_storage, &m_storage );

    // self := other
    m_storage_handler = other.m_storage_handler;
    m_storage_handler( operation::move, &m_storage, &other.m_storage );

    // other := tmp
    other.m_storage_handler = tmp.m_storage_handler;
    other.m_storage_handler( operation::move, &other.m_storage, &tmp.m_storage );
  }
  else if( other.m_storage_handler != nullptr )
  {
    swap(m_storage_handler, other.m_storage_handler);

    // self := other
    m_storage_handler( operation::move, &m_storage, &other.m_storage );
    m_storage_handler( operation::destroy, &other.m_storage, nullptr );
  }
  else if( m_storage_handler != nullptr )
  {
    swap(m_storage_handler, other.m_storage_handler);

    // other := self
    other.m_storage_handler( operation::move, &other.m_storage, &m_storage );
    other.m_storage_handler( operation::destroy, &m_storage, nullptr );
  }
}

//-----------------------------------------------------------------------------
// Observers
//-----------------------------------------------------------------------------

inline bool bit::core::any::has_value()
  const noexcept
{
  return m_storage_handler != nullptr;
}

inline const std::type_info& bit::core::any::type()
  const noexcept
{
  if( has_value() ) {
    auto* p = m_storage_handler( operation::type, nullptr, nullptr );
    return (*static_cast<const std::type_info*>(p));
  }
  return typeid(void);
}

//=============================================================================
// definition : non-member functions : class : any
//=============================================================================

//-----------------------------------------------------------------------------
// utilities
//-----------------------------------------------------------------------------

inline void bit::core::swap( any& lhs, any& rhs )
  noexcept
{
  lhs.swap(rhs);
}

//-----------------------------------------------------------------------------
// casts
//-----------------------------------------------------------------------------

template<typename T>
inline T bit::core::casts::any_cast( any& operand )
{
  auto* p = any_cast<T>(&operand);
  if(!p) throw bad_any_cast{};
  return static_cast<T>(*p);
}

template<typename T>
inline T bit::core::casts::any_cast( any&& operand )
{
  auto* p = any_cast<T>(&operand);
  if(!p) throw bad_any_cast{};
  return static_cast<T>(std::move(*p));
}

template<typename T>
inline T bit::core::casts::any_cast( const any& operand )
{
  auto* p = any_cast<T>(&operand);
  if(!p) throw bad_any_cast{};
  return static_cast<T>(*p);
}

template<typename T>
inline T* bit::core::casts::any_cast( any* operand )
  noexcept
{
  if(!operand) return nullptr;
  if(operand->type() != typeid(T)) return nullptr;

  auto p = operand->m_storage_handler( any::operation::value,
                                       &operand->m_storage,
                                       nullptr );
  return const_cast<T*>(static_cast<const T*>(p));
}

template<typename T>
inline const T* bit::core::casts::any_cast( const any* operand )
  noexcept
{
  if(!operand) return nullptr;
  if(operand->type() != typeid(T)) return nullptr;

  auto p = operand->m_storage_handler( any::operation::value,
                                       &operand->m_storage,
                                       nullptr );
  return static_cast<const T*>(p);
}

#endif /* BIT_CORE_UTILITIES_DETAIL_ANY_INL */
