# CMakeList.txt : CMake project for ray-tracing-core, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)
set (CMAKE_CXX_STANDARD 20)

# Add source to this project's executable.
add_executable (ray-tracing-core-plyaground
	"ray_tracing_playground.cpp"
	"ray_tracing_playground.h")

# TODO: Add tests and install targets if needed.
