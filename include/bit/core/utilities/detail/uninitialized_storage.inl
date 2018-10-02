#ifndef BIT_CORE_UTILITY_DETAIL_UNINITIALIZED_STORAGE_INL
#define BIT_CORE_UTILITY_DETAIL_UNINITIALIZED_STORAGE_INL

//-----------------------------------------------------------------------------
// Uninitialized Construction
//-----------------------------------------------------------------------------

template<typename T>
inline T* bit::core::uninitialized_default_construct_at( void* ptr )
{
  return uninitialized_construct_at<T>( ptr );
}

template<typename T>
inline T* bit::core::uninitialized_copy_at( void* ptr, const T& other )
{
  return uninitialized_construct_at<T>( ptr, other );
}

template<typename T>
inline T* bit::core::uninitialized_move_at( void* ptr, T&& other )
{
  return uninitialized_construct_at<T>( ptr, std::move(other) );
}

template<typename T, typename...Args>
inline T* bit::core::uninitialized_construct_at( void* ptr, Args&&...args )
{
  new (ptr) T( std::forward<Args>(args)... );
  return static_cast<T*>(ptr);
}

//-----------------------------------------------------------------------------

namespace bit { namespace core { namespace detail {

template<typename T, typename Tuple, std::size_t... I>
inline T* uninitialized_tuple_construct_at_impl( void* ptr, Tuple&& t, std::index_sequence<I...> )
{
  using ::bit::core::adl::get;

  new (ptr) T( get<I>(std::forward<Tuple>(t))... );
  return static_cast<T*>(ptr);
}

} } } // namespace bit::core::detail

template<typename T, typename Tuple>
inline T* bit::core::uninitialized_tuple_construct_at( void* ptr, Tuple&& tuple )
{
  return detail::uninitialized_tuple_construct_at_impl<T>(
    ptr,
    std::forward<Tuple>(tuple),
    std::make_index_sequence<std::tuple_size<std::decay_t<Tuple>>::value>()
  );
}

//-----------------------------------------------------------------------------

namespace bit { namespace core { namespace detail {

template<typename ForwardIterator, typename...Args>
inline void uninitialized_construct_impl( std::true_type,
                                          ForwardIterator first,
                                          ForwardIterator last,
                                          Args&&...args )
{
  using type = typename std::iterator_traits<ForwardIterator>::value_type;

  auto current = first;
  for (; current != last; ++current) {
    new (static_cast<void*>(std::addressof(*current))) type( std::forward<Args>(args)... );
  }
}

template<typename ForwardIterator, typename...Args>
inline void uninitialized_construct_impl( std::false_type,
                                          ForwardIterator first,
                                          ForwardIterator last,
                                          Args&&...args  )
{
  using type = typename std::iterator_traits<ForwardIterator>::value_type;

  auto current = first;
  try {
    for (; current != last; ++current) {
      new (static_cast<void*>(std::addressof(*current))) type( std::forward<Args>(args)... );
    }
  } catch (...) {
    for (; first != current; ++first) {
      first->~type();
    }
    throw;
  }
}

} } } // namespace bit::core::detail

template<typename ForwardIterator>
inline void bit::core::uninitialized_construct( ForwardIterator first,
                                                ForwardIterator last )
{
  using type = typename std::iterator_traits<ForwardIterator>::value_type;
  using tag = std::is_nothrow_default_constructible<type>;

  return detail::uninitialized_construct_impl( tag{}, first, last );
}

template<typename ForwardIterator>
inline void bit::core::uninitialized_construct( ForwardIterator first,
                                                ForwardIterator last,
                                                const typename std::iterator_traits<ForwardIterator>::value_type& copy )
{
  using type = typename std::iterator_traits<ForwardIterator>::value_type;
  using tag = std::is_nothrow_copy_constructible<type>;

  return detail::uninitialized_construct_impl( tag{}, first, last, copy );
}

//-----------------------------------------------------------------------------

namespace bit { namespace core { namespace detail {

template<typename ForwardIterator, typename Size, typename...Args>
inline void uninitialized_construct_n_impl( std::true_type,
                                            ForwardIterator first,
                                            Size n,
                                            Args&&...args )
{
  using type = typename std::iterator_traits<ForwardIterator>::value_type;

  auto current = first;
  for (; n > 0; --n, ++current) {
    new (static_cast<void*>(std::addressof(*current))) type( std::forward<Args>(args)... );
  }
}

template<typename ForwardIterator, typename Size, typename...Args>
inline void uninitialized_construct_n_impl( std::false_type,
                                            ForwardIterator first,
                                            Size n,
                                            Args&&...args  )
{
  using type = typename std::iterator_traits<ForwardIterator>::value_type;

  auto current = first;
  try {
    for (; n > 0; --n, ++current) {
      new (static_cast<void*>(std::addressof(*current))) type( std::forward<Args>(args)... );
    }
  } catch (...) {
    for (; first != current; ++first) {
      first->~type();
    }
    throw;
  }
}

}}} // namespace bit::core::detail

template<typename ForwardIterator, typename Size>
inline ForwardIterator
  bit::core::uninitialized_construct_n( ForwardIterator first,
                                        Size n )
{
  using type = typename std::iterator_traits<ForwardIterator>::value_type;
  using tag = std::is_nothrow_default_constructible<type>;

  return detail::uninitialized_construct_n_impl( tag{}, first, n );
}

template<typename ForwardIterator, typename Size>
inline ForwardIterator
  bit::core::uninitialized_construct_n( ForwardIterator first,
                                        Size n,
                                        const typename std::iterator_traits<ForwardIterator>::value_type& copy )
{
  using type = typename std::iterator_traits<ForwardIterator>::value_type;
  using tag = std::is_nothrow_default_constructible<type>;

  return detail::uninitialized_construct_n_impl( tag{}, first, n );
}

//-----------------------------------------------------------------------------
// Destruction
//-----------------------------------------------------------------------------

template<typename T>
inline void bit::core::destroy_at( T* p )
{
  p->~T();
}

template<typename ForwardIterator>
inline void
  bit::core::destroy( ForwardIterator first, ForwardIterator last )
{
  for(; first != last; ++first) {
    destroy_at( std::addressof(*first) );
  }
}

template<typename ForwardIterator, typename Size>
inline ForwardIterator
  bit::core::destroy_n( ForwardIterator first, Size n )
{
  for (; n > 0; ++first, --n) {
    destroy_at( std::addressof(*first) );
  }
  return first;
}

#endif /* BIT_CORE_UTILITY_DETAIL_UNINITIALIZED_STORAGE_INL */
