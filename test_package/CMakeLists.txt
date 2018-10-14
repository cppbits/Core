cmake_minimum_required(VERSION 3.1)

include("${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
conan_basic_setup(TARGETS)

find_package(CppBitsCore REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

set(source_files
  src/main.cpp
)

add_executable(CppBitsCore.TestPackage ${source_files})
target_link_libraries(CppBitsCore.TestPackage PRIVATE CppBits::Core)
