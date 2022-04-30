# CMakeList.txt : CMake project for ray-tracing-core, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)
set (CMAKE_CXX_STANDARD 20)
project (ray-tracing-utility)

# Add include directories
include_directories (
    "./include"
    "../ray-tracing-core/include"
)

# Add source to this project's executable.

file(GLOB rtc_utility_source
    "include/*/*.h"
    "source/*/*.cpp"
)

add_library (ray-tracing-utility STATIC 
    ${rtc_utility_source}
)

add_subdirectory (test)