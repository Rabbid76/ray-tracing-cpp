# CMakeList.txt : CMake project for ray-tracing-core, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)
set (CMAKE_CXX_STANDARD 20)
project (ray-tracing-core)

# Add include directories
include_directories (
    "./include")

# Add source to this project's executable.

file(GLOB rtc_core_source 
    "source/*/*.cpp")

add_library (ray-tracing-core STATIC 
    ${rtc_core_source})

add_subdirectory (test)

#add_executable(ray-tracing-core-tests test/test.cpp)
#target_link_libraries(ray-tracing-core-tests PRIVATE Catch2::Catch2WithMain)

# TODO: Add tests and install targets if needed.

