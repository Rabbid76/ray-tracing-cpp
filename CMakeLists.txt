# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)

#set_property(TARGET tgt PROPERTY CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD 20)

project ("ray-tracing-cpp")

# Include sub-projects.
add_subdirectory ("ray-tracing-core")
add_subdirectory ("ray-tracing-core-playground")
