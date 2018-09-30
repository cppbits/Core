cmake_minimum_required(VERSION 3.1)

##############################################################################
# Test Options
##############################################################################

option(BIT_CORE_ENABLE_SANITIZER "Compile and run with sanitizers" OFF)
option(BIT_CORE_ENABLE_COVERAGE "Compile with coverage information" OFF)
option(BIT_CORE_ENABLE_VALGRIND "Compile and run with valgrind" OFF)

##############################################################################
# Address Sanitizer
##############################################################################

if( BIT_CORE_ENABLE_SANITIZER AND NOT MSVC )
  add_compile_options(
    -g
    -O2
    -fno-omit-frame-pointer
    -fsanitize=address
    -fsanitize=leak
    -fsanitize=undefined
  )
endif()

##############################################################################
# Unit Tests
##############################################################################

find_package(Catch2 REQUIRED)

set(sources
      # utilities
      bit/core/utilities/compressed_pair.test.cpp
      bit/core/utilities/delegate.test.cpp
      bit/core/utilities/lazy.test.cpp
      bit/core/utilities/optional.test.cpp
      bit/core/utilities/tribool.test.cpp
      bit/core/utilities/expected.test.cpp
      bit/core/utilities/scope_guard.test.cpp
      bit/core/utilities/variant.test.cpp

      # containers
      bit/core/containers/array_view.test.cpp
      bit/core/containers/set_view.test.cpp
      bit/core/containers/span.test.cpp
      bit/core/containers/string_view.test.cpp
      bit/core/containers/ring_deque.test.cpp
      bit/core/containers/ring_buffer.test.cpp

      # memory
      bit/core/memory/exclusive_ptr.test.cpp

      main.test.cpp
)

add_executable(core_test ${sources})

target_link_libraries(core_test PRIVATE
  CppBits::core
  Catch2::Catch2
)

#-----------------------------------------------------------------------------

add_test( NAME "core_test_default"
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND "$<TARGET_FILE:core_test>" )

add_test( NAME "core_test_all"
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND "$<TARGET_FILE:core_test>" "*" )


