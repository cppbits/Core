set(temp_state ${CMAKE_FIND_APPBUNDLE})
set(CMAKE_FIND_APPBUNDLE "NEVER")

find_program(CMAKE_DOXYGEN_PROGRAM doxygen)

set(CMAKE_FIND_APPBUNDLE ${temp_state})

if( NOT CMAKE_DOXYGEN_PROGRAM )
  message(FATAL_ERROR "'Doxygen' program not found.")
endif()

# Add target for generating doxygen
file(MAKE_DIRECTORY "${BIT_CORE_DOXYGEN_OUTPUT_PATH}")
add_custom_target(
  core_docs
  COMMAND "${CMAKE_DOXYGEN_PROGRAM}" "${CMAKE_CURRENT_LIST_FILE}/Doxyfile"
  WORKING_DIRECTORY "${BIT_CORE_DOXYGEN_OUTPUT_PATH}"
  COMMENT "Generating Doxygen documentation for Bit::core"
  VERBATIM
)
