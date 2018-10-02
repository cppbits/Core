#ifndef BIT_CORE_UTILITIES_DETAIL_ASSERT_INL
#define BIT_CORE_UTILITIES_DETAIL_ASSERT_INL

namespace bit { namespace core { namespace detail {
  BIT_NO_RETURN
  inline void assert_internal( const char* message, source_location source )
  {
    std::fprintf(stderr, "[assertion] %s (%zu)::%s\n"
                         "            %s\n", source.file_name(),
                                             source.line(),
                                             source.function_name(),
                                             message );

    BIT_BREAKPOINT();
    ::std::terminate();
  }

} } } // namespace bit::core::detail

#endif /* BIT_CORE_UTILITIES_DETAIL_ASSERT_INL */
