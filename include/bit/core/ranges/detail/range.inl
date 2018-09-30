#ifndef BIT_CORE_RANGES_DETAIL_RANGE_INL
#define BIT_CORE_RANGES_DETAIL_RANGE_INL

//------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------

template<typename I, typename S>
constexpr bit::core::range<I,S>::range( I iterator, S sentinel )
  noexcept
  : m_first(std::move(iterator)),
    m_last(std::move(sentinel))
{

}

//------------------------------------------------------------------------
// Iterators
//------------------------------------------------------------------------

template<typename I, typename S>
constexpr typename bit::core::range<I,S>::iterator
  bit::core::range<I,S>::begin()
{
  return m_first;
}

template<typename I, typename S>
constexpr typename bit::core::range<I,S>::sentinel
  bit::core::range<I,S>::end()
{
  return m_last;
}

//------------------------------------------------------------------------
// Ranges
//------------------------------------------------------------------------

template<typename I, typename S>
inline constexpr bit::core::range<I,S>
  bit::core::make_range( I iterator, S sentinel )
{
  return { iterator, sentinel };
}


#endif /* BIT_CORE_RANGES_DETAIL_RANGE_INL */
