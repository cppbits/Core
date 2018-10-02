#ifndef BIT_CORE_RANGES_DETAIL_REVERSE_RANGE_INL
#define BIT_CORE_RANGES_DETAIL_REVERSE_RANGE_INL


template<typename Range>
inline constexpr bit::core::detail::reverse_range_t<Range>
  bit::core::make_reverse_range( Range&& range )
{
  return make_range( std::forward<Range>(range).rbegin(),
                     std::forward<Range>(range).rend() );
}


#endif /* BIT_CORE_RANGES_DETAIL_REVERSE_RANGE_INL */
