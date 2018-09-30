cmake_minimum_required(VERSION 3.1)

##############################################################################
# Test Options
##############################################################################

option(BIT_CORE_ENABLE_ASAN "Compile and run the address sanetizer" OFF)
option(BIT_CORE_ENABLE_USAN "Compile and run the undefined behavior sanitizer" OFF)
option(BIT_CORE_ENABLE_TSAN "Compile and run the thread sanitizer" OFF)

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

##############################################################################
# Address Sanitizer
##############################################################################

if( BIT_CORE_ENABLE_ASAN )

  add_executable(core_test ${sources})
  target_link_libraries(core_test PRIVATE
    -fsanitize=address
    -fsanitize=leak
    -fuse-ld=gold
  )

  target_compile_options(core_test PRIVATE
    -g
    -O1
    -fno-omit-frame-pointer
    -fsanitize=address
    -fsanitize=leak
    -fuse-ld=gold
  )

endif()

##############################################################################
# Undefined Behavior Sanitizer
##############################################################################

if( BIT_CORE_ENABLE_UBSAN )

  target_compile_options(core_test PRIVATE
    -fsanitize=undefined
    -fuse-ld=gold
  )
  target_link_libraries(core_test PRIVATE
    -fsanitize=undefined
    -fuse-ld=gold
  )

endif()

##############################################################################
# Thread Sanitizer
##############################################################################

if( BIT_CORE_ENABLE_TSAN )

  target_compile_options(core_test PRIVATE
    -fsanitize=thread
    -fuse-ld=gold
  )
  target_link_libraries(core_test PRIVATE
    -fsanitize=thread
    -fuse-ld=gold
  )

endif()
