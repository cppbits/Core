#ifndef BIT_CORE_RANGES_DETAIL_ZIP_RANGE_INL
#define BIT_CORE_RANGES_DETAIL_ZIP_RANGE_INL

template<typename Range0, typename...RangeN>
inline constexpr auto
  bit::core::make_zip_range( Range0&& r0, RangeN&&...rn )
  -> bit::core::range<bit::core::zip_iterator<decltype( r0.begin() ), decltype( rn.begin() )...>,
                     bit::core::zip_iterator<decltype( r0.end() ), decltype( rn.end() )...>>
{
  using begin_iterator = zip_iterator<decltype( r0.begin() ), decltype( rn.begin() )...>;
  using end_iterator   = zip_iterator<decltype( r0.end() ), decltype( rn.end() )...>;

  return { begin_iterator{r0.begin(), rn.begin()...}, end_iterator{r0.end(), rn.end()...} };
}

#endif /* BIT_CORE_RANGES_DETAIL_ZIP_RANGE_INL */
