# CMakeList.txt : CMake project for ray-tracing-core, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add include directories
include_directories (
    "C:/source/_cpp_extern/glm-0.9.9.3/glm"
    "./include")

# Add source to this project's executable.

file(GLOB rtc_core_math_source 
    "source/*/*.cpp")

add_library (ray-tracing-core STATIC 
    ${rtc_core_math_source})

# TODO: Add tests and install targets if needed.