cmake_minimum_required(VERSION 3.1)

##############################################################################
# Test Options
##############################################################################

option(BIT_CORE_ENABLE_MSAN "Compile and run with memory sanitizers" OFF)
option(BIT_CORE_ENABLE_ASAN "Compile and run with address sanitizers" OFF)
option(BIT_CORE_ENABLE_UBSAN "Compile and run with undefined behavior sanitizers" OFF)
option(BIT_CORE_ENABLE_TSAN "Compile and run with thread sanitizers" OFF)
option(BIT_CORE_ENABLE_COVERAGE "Compile with coverage information" OFF)
option(BIT_CORE_ENABLE_VALGRIND "Compile and run with valgrind" OFF)

##############################################################################
# Utilities
##############################################################################

function(assert_clang_or_gcc)
  if( NOT ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" OR
           "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU") )
    message(FATAL_ERROR "Sanitizers require either GCC or clang.")
  endif()
endfunction()

##############################################################################
# Memory Sanitizer
##############################################################################

if( BIT_CORE_ENABLE_MSAN )
  assert_clang_or_gcc()

  add_compile_options(
    -g
    -O2
    -fno-omit-frame-pointer
    -fsanitize=memory
  )
  link_libraries(
    -fsanitize=memory
  )
endif()

##############################################################################
# Address Sanitizer
##############################################################################

if( BIT_CORE_ENABLE_ASAN )
  assert_clang_or_gcc()

  add_compile_options(
    -g
    -O1
    -fno-omit-frame-pointer
    -fsanitize=address
    -fsanitize=leak
  )
  link_libraries(
    -fsanitize=address
    -fsanitize=leak
  )
endif()

##############################################################################
# UB Sanitizer
##############################################################################

if( BIT_CORE_ENABLE_UBSAN )
  assert_clang_or_gcc()

  add_compile_options(
    -fsanitize=undefined
  )
  link_libraries(
    -fsanitize=undefined
  )
endif()

##############################################################################
# Thread Sanitizer
##############################################################################

if( BIT_CORE_ENABLE_UBSAN )
  assert_clang_or_gcc()

  add_compile_options(
    -fsanitize=thread
  )
  link_libraries(
    -fsanitize=thread
  )
endif()

##############################################################################
# Code Coverage
##############################################################################

if( BIT_CORE_ENABLE_COVERAGE )
  assert_clang_or_gcc()

  add_compile_options(
    -g
    -O0
    -fprofile-arcs
    -ftest-coverage
    --coverage
  )
  link_libraries(--coverage)
endif()

##############################################################################
# Unit Tests
##############################################################################

find_package(Catch2 REQUIRED)

set(sources
      # utilities
      src/bit/core/utilities/any.test.cpp
      src/bit/core/utilities/compressed_pair.test.cpp
      src/bit/core/utilities/delegate.test.cpp
      src/bit/core/utilities/lazy.test.cpp
      src/bit/core/utilities/optional.test.cpp
      src/bit/core/utilities/tribool.test.cpp
      src/bit/core/utilities/expected.test.cpp
      src/bit/core/utilities/variant.test.cpp

      # containers
      src/bit/core/containers/array_view.test.cpp
      src/bit/core/containers/set_view.test.cpp
      src/bit/core/containers/span.test.cpp
      src/bit/core/containers/string_view.test.cpp
      src/bit/core/containers/ring_deque.test.cpp
      src/bit/core/containers/ring_buffer.test.cpp

      # memory
      src/bit/core/memory/exclusive_ptr.test.cpp

      src/main.test.cpp
)

add_executable(core_test ${sources})

target_link_libraries(core_test PRIVATE
  CppBits::Core
  Catch2::Catch2
)

#-----------------------------------------------------------------------------

add_test( NAME "core_test_default"
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND "$<TARGET_FILE:core_test>" )

add_test( NAME "core_test_all"
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND "$<TARGET_FILE:core_test>" "*" )


