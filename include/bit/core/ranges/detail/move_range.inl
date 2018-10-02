#ifndef BIT_CORE_RANGES_DETAIL_MOVE_RANGE_INL
#define BIT_CORE_RANGES_DETAIL_MOVE_RANGE_INL

template<typename Iterator, typename Sentinel>
inline constexpr bit::core::move_range<Iterator,Sentinel>
  bit::core::make_move_range( Iterator iterator, Sentinel sentinel )
{
  return { std::make_move_iterator(iterator),
           std::make_move_iterator(sentinel) };
}

template<typename Range>
inline constexpr auto
  bit::core::make_move_range( Range&& r )
#ifndef _MSC_VER
  -> decltype(::bit::core::make_move_range( r.begin(), r.end() ))
#endif // _MSC_VER
{
  return make_move_range( std::forward<Range>(r).begin(),
                          std::forward<Range>(r).end() );
}

#endif /* BIT_CORE_RANGES_DETAIL_MOVE_RANGE_INL */
