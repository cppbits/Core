cmake_minimum_required(VERSION 3.4)

enable_testing()

set(BIT_CORE_CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake/Modules")
set(BIT_CORE_CMAKE_TEMPLATE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake/Templates")

set(CMAKE_MODULE_PATH "${BIT_CORE_CMAKE_MODULE_PATH}" "${CMAKE_MODULE_PATH}")

##############################################################################
# Project Options
##############################################################################

option(BIT_CORE_COMPILE_SELF_CONTAINMENT_TESTS "Include each header independently in a .cpp file to determine header self-containment" OFF)
option(BIT_CORE_COMPILE_UNIT_TESTS "Compile and run the unit tests for this library" OFF)
option(BIT_CORE_GENERATE_DOCS "Generates doxygen documentation" OFF)
option(BIT_CORE_INSTALL_DOCS "Install documentation for this library" OFF)
option(BIT_CORE_VERBOSE_CONFIGURE "Verbosely configures this library project" OFF)

set(BIT_CORE_DOXYGEN_OUTPUT_PATH "${CMAKE_CURRENT_BINARY_DIR}/doxygen" CACHE STRING "Output location for doxygen")

##############################################################################
# Project
##############################################################################

project(CppBitsCore LANGUAGES CXX)

set(BIT_CORE_VERSION_MAJOR 0 CACHE STRING "major version of Bit::core" FORCE)
set(BIT_CORE_VERSION_MINOR 0 CACHE STRING "minor version of Bit::core" FORCE)
set(BIT_CORE_VERSION_PATCH 1 CACHE STRING "patch version of Bit::core" FORCE)

set(BIT_CORE_VERSION "${BIT_CORE_VERSION_MAJOR}.${BIT_CORE_VERSION_MINOR}.${BIT_CORE_VERSION_PATCH}" CACHE STRING "version of CppBits::core" FORCE)

if( EXISTS "${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake" )
  include("${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake")
  conan_basic_setup()
endif()

if( BIT_CORE_VERBOSE_CONFIGURE )
  message(STATUS "CppBits::core ${BIT_CORE_VERSION}")
endif()

#-----------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# default to debug when building CMake
if( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES )
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

#-----------------------------------------------------------------------------

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" )
  add_compile_options(-Wall -Werror -pedantic )
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
  # TODO: Determine MSVC necessary compiler flags
endif()

# Create the version header
configure_file( "${BIT_CORE_CMAKE_TEMPLATE_PATH}/version.hpp.in"
                "${CMAKE_CURRENT_BINARY_DIR}/include/bit/core/version.hpp"
                @ONLY )

set(header_files
  # Containers
  include/bit/core/containers/array.hpp
  include/bit/core/containers/array_view.hpp
  include/bit/core/containers/ring_array.hpp
  include/bit/core/containers/ring_buffer.hpp
  include/bit/core/containers/ring_deque.hpp
  include/bit/core/containers/map_view.hpp
  include/bit/core/containers/set_view.hpp
  include/bit/core/containers/span.hpp
  include/bit/core/containers/string.hpp
  include/bit/core/containers/string_span.hpp
  include/bit/core/containers/string_view.hpp

  # Iterators
  include/bit/core/iterators/tagged_iterator.hpp
  include/bit/core/iterators/tuple_element_iterator.hpp
  include/bit/core/iterators/zip_iterator.hpp

  # Memory
  include/bit/core/memory/allocator_deleter.hpp
  include/bit/core/memory/exclusive_ptr.hpp
  include/bit/core/memory/memory.hpp
  include/bit/core/memory/observer_ptr.hpp
  include/bit/core/memory/offset_ptr.hpp
  include/bit/core/memory/owner.hpp

  # Ranges
  include/bit/core/ranges/move_range.hpp
  include/bit/core/ranges/range.hpp
  include/bit/core/ranges/reverse_range.hpp
  include/bit/core/ranges/tuple_element_range.hpp
  include/bit/core/ranges/zip_range.hpp

  # Traits
  include/bit/core/traits/composition/bool_constant.hpp
  include/bit/core/traits/composition/conjunction.hpp
  include/bit/core/traits/composition/detected.hpp
  include/bit/core/traits/composition/disjunction.hpp
  include/bit/core/traits/composition/empty.hpp
  include/bit/core/traits/composition/type_identity.hpp
  include/bit/core/traits/composition/integral_constant.hpp
  include/bit/core/traits/composition/negation.hpp
  include/bit/core/traits/composition/nonesuch.hpp
  include/bit/core/traits/composition/sfinae.hpp
  include/bit/core/traits/composition/size_constant.hpp
  include/bit/core/traits/composition/type_list.hpp
  include/bit/core/traits/composition/void_t.hpp
  include/bit/core/traits/concepts/is_associative_container.hpp
  include/bit/core/traits/concepts/is_bidirectional_iterator.hpp
  include/bit/core/traits/concepts/is_container.hpp
  include/bit/core/traits/concepts/is_contiguous_container.hpp
  include/bit/core/traits/concepts/is_equality_comparable.hpp
  include/bit/core/traits/concepts/is_forward_iterator.hpp
  include/bit/core/traits/concepts/is_input_iterator.hpp
  include/bit/core/traits/concepts/is_iterator.hpp
  include/bit/core/traits/concepts/is_nullable_pointer.hpp
  include/bit/core/traits/concepts/is_random_access_iterator.hpp
  include/bit/core/traits/concepts/is_range.hpp
  include/bit/core/traits/concepts/is_reverse_range.hpp
  include/bit/core/traits/concepts/is_reversible_container.hpp
  include/bit/core/traits/concepts/is_sequence_container.hpp
  include/bit/core/traits/properties/is_assignable.hpp
  include/bit/core/traits/properties/is_const_member_function_pointer.hpp
  include/bit/core/traits/properties/is_copy_assignable.hpp
  include/bit/core/traits/properties/is_copy_constructible.hpp
  include/bit/core/traits/properties/is_cv_member_function_pointer.hpp
  include/bit/core/traits/properties/is_default_constructible.hpp
  include/bit/core/traits/properties/is_destructible.hpp
  include/bit/core/traits/properties/is_explicitly_constructible.hpp
  include/bit/core/traits/properties/is_explicitly_convertible.hpp
  include/bit/core/traits/properties/is_implicitly_constructible.hpp
  include/bit/core/traits/properties/is_implicitly_convertible.hpp
  include/bit/core/traits/properties/is_member_function_pointer.hpp
  include/bit/core/traits/properties/is_member_object_pointer.hpp
  include/bit/core/traits/properties/is_member_pointer.hpp
  include/bit/core/traits/properties/is_move_assignable.hpp
  include/bit/core/traits/properties/is_move_constructible.hpp
  include/bit/core/traits/properties/is_nothrow_assignable.hpp
  include/bit/core/traits/properties/is_nothrow_copy_assignable.hpp
  include/bit/core/traits/properties/is_nothrow_copy_constructible.hpp
  include/bit/core/traits/properties/is_nothrow_default_constructible.hpp
  include/bit/core/traits/properties/is_nothrow_destructible.hpp
  include/bit/core/traits/properties/is_nothrow_move_assignable.hpp
  include/bit/core/traits/properties/is_nothrow_move_constructible.hpp
  include/bit/core/traits/properties/is_trivially_assignable.hpp
  include/bit/core/traits/properties/is_trivially_copy_assignable.hpp
  include/bit/core/traits/properties/is_trivially_copy_constructible.hpp
  include/bit/core/traits/properties/is_trivially_default_constructible.hpp
  include/bit/core/traits/properties/is_trivially_destructible.hpp
  include/bit/core/traits/properties/is_trivially_move_assignable.hpp
  include/bit/core/traits/properties/is_trivially_move_constructible.hpp
  include/bit/core/traits/properties/is_volatile_member_function_pointer.hpp
  include/bit/core/traits/relationships/arity.hpp
  include/bit/core/traits/relationships/function_argument.hpp
  include/bit/core/traits/relationships/index_of_type.hpp
  include/bit/core/traits/relationships/is_base_of_all.hpp
  include/bit/core/traits/relationships/is_base_of_any.hpp
  include/bit/core/traits/relationships/is_derived_from_all.hpp
  include/bit/core/traits/relationships/is_derived_from_any.hpp
  include/bit/core/traits/relationships/is_different.hpp
  include/bit/core/traits/relationships/is_one_of.hpp
  include/bit/core/traits/relationships/is_same.hpp
  include/bit/core/traits/relationships/is_same_constness.hpp
  include/bit/core/traits/relationships/is_same_cv.hpp
  include/bit/core/traits/relationships/is_same_decay.hpp
  include/bit/core/traits/relationships/is_same_sign.hpp
  include/bit/core/traits/relationships/is_same_volatility.hpp
  include/bit/core/traits/relationships/nth_type.hpp
  include/bit/core/traits/relationships/pointer_rank.hpp
  include/bit/core/traits/relationships/resolves_overload.hpp
  include/bit/core/traits/transformations/base_type.hpp
  include/bit/core/traits/transformations/function_signature.hpp
  include/bit/core/traits/transformations/match_cv_qualifiers.hpp
  include/bit/core/traits/transformations/match_ref_qualifiers.hpp
  include/bit/core/traits/transformations/match_sign_qualifiers.hpp
  include/bit/core/traits/transformations/normalize_char.hpp
  include/bit/core/traits/transformations/remove_cvref.hpp
  include/bit/core/traits/transformations/remove_pointers.hpp

  # Utilities
  include/bit/core/utilities/aligned_storage.hpp
  include/bit/core/utilities/assert.hpp
  include/bit/core/utilities/byte.hpp
  include/bit/core/utilities/casts.hpp
  include/bit/core/utilities/compiler_traits.hpp
  include/bit/core/utilities/compressed_pair.hpp
  include/bit/core/utilities/compressed_tuple.hpp
  include/bit/core/utilities/container_utilities.hpp
  include/bit/core/utilities/delegate.hpp
  include/bit/core/utilities/expected.hpp
  include/bit/core/utilities/hash.hpp
  include/bit/core/utilities/in_place.hpp
  include/bit/core/utilities/invoke.hpp
  include/bit/core/utilities/lazy.hpp
  include/bit/core/utilities/macros.hpp
  include/bit/core/utilities/monostate.hpp
  include/bit/core/utilities/optional.hpp
  include/bit/core/utilities/propagate_const.hpp
  include/bit/core/utilities/scope_guard.hpp
  include/bit/core/utilities/source_location.hpp
  include/bit/core/utilities/tribool.hpp
  include/bit/core/utilities/tuple.hpp
  include/bit/core/utilities/tuple_utilities.hpp
  include/bit/core/utilities/types.hpp
  include/bit/core/utilities/uninitialized_storage.hpp
  include/bit/core/utilities/variant.hpp
)

set(inline_header_files
  # Containers
  include/bit/core/containers/detail/array.inl
  include/bit/core/containers/detail/array_view.inl
  include/bit/core/containers/detail/ring_array.inl
  include/bit/core/containers/detail/ring_buffer.inl
  include/bit/core/containers/detail/ring_deque.inl
  include/bit/core/containers/detail/map_view.inl
  include/bit/core/containers/detail/set_view.inl
  include/bit/core/containers/detail/span.inl
  include/bit/core/containers/detail/string.inl
  include/bit/core/containers/detail/string_span.inl
  include/bit/core/containers/detail/string_view.inl

  # Iterators
  include/bit/core/iterators/detail/tagged_iterator.inl
  include/bit/core/iterators/detail/tuple_element_iterator.inl
  include/bit/core/iterators/detail/zip_iterator.inl

  # Memory
  include/bit/core/memory/detail/allocator_deleter.inl
  include/bit/core/memory/detail/exclusive_ptr.inl
  include/bit/core/memory/detail/memory.inl
  include/bit/core/memory/detail/observer_ptr.inl
  include/bit/core/memory/detail/offset_ptr.inl

  # Ranges
  include/bit/core/ranges/detail/move_range.inl
  include/bit/core/ranges/detail/range.inl
  include/bit/core/ranges/detail/reverse_range.inl
  include/bit/core/ranges/detail/tuple_element_range.inl
  include/bit/core/ranges/detail/zip_range.inl

  # Utilities
  include/bit/core/utilities/detail/assert.inl
  include/bit/core/utilities/detail/byte.inl
  include/bit/core/utilities/detail/casts.inl
  include/bit/core/utilities/detail/compressed_pair.inl
  include/bit/core/utilities/detail/compressed_tuple.inl
  include/bit/core/utilities/detail/container_utilities.inl
  include/bit/core/utilities/detail/delegate.inl
  include/bit/core/utilities/detail/expected.inl
  include/bit/core/utilities/detail/hash.inl
  include/bit/core/utilities/detail/invoke.inl
  include/bit/core/utilities/detail/lazy.inl
  include/bit/core/utilities/detail/monostate.inl
  include/bit/core/utilities/detail/optional.inl
  include/bit/core/utilities/detail/propagate_const.inl
  include/bit/core/utilities/detail/scope_guard.inl
  include/bit/core/utilities/detail/source_location.inl
  include/bit/core/utilities/detail/tribool.inl
  include/bit/core/utilities/detail/tuple_utilities.inl
  include/bit/core/utilities/detail/uninitialized_storage.inl
  include/bit/core/utilities/detail/variant.inl
)

add_library(core INTERFACE)
add_library(CppBits::core ALIAS core)

target_compile_features(core INTERFACE cxx_std_14)

target_include_directories(core INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
  $<INSTALL_INTERFACE:include>
)

##############################################################################
# Header Self-Containment Tests
##############################################################################

include(AddSelfContainmentTest)

if( BIT_CORE_COMPILE_SELF_CONTAINMENT_TESTS )
  add_self_containment_test(core_self_containment_test
    TARGET core
    HEADERS ${header_files}
  )
  target_sources(core_self_containment_test PRIVATE ${inline_header_files})
endif()

##############################################################################
# Unit Tests
##############################################################################

if( BIT_CORE_COMPILE_UNIT_TESTS )
  add_subdirectory(test)
endif()

##############################################################################
# Documentation
##############################################################################

if( BIT_CORE_GENERATE_DOCS )
  add_subdirectory(docs)
endif()

##############################################################################
# Installation
##############################################################################

set(PACKAGE_NAME "CppBitsCore")

configure_file(
  "${BIT_CORE_CMAKE_TEMPLATE_PATH}/PackageConfigVersion.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
  @ONLY
)

configure_file(
  "${BIT_CORE_CMAKE_TEMPLATE_PATH}/PackageConfig.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  @ONLY
)

#-----------------------------------------------------------------------------

include(GNUInstallDirs)

set(BIT_CORE_CMAKE_CONFIG_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PACKAGE_NAME}")

# Includes
install(
  DIRECTORY "include/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)
install(
  DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include/"
  DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Targets
install(
  TARGETS "core"
  EXPORT "CppBitsCoreTargets"
  DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)
install(
  EXPORT "CppBitsCoreTargets"
  NAMESPACE "CppBits::"
  DESTINATION "${BIT_CORE_CMAKE_CONFIG_DESTINATION}"
)
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
  DESTINATION "${BIT_CORE_CMAKE_CONFIG_DESTINATION}"
)

# Documentation
if( BIT_CORE_INSTALL_DOCS AND BIT_CORE_GENERATE_DOCS )
  install(
    DIRECTORY "${BIT_CORE_DOXYGEN_OUTPUT_PATH}/"
    DESTINATION "${CMAKE_INSTALL_DOCDIR}"
  )
endif()
