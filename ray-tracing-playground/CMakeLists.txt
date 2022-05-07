# CMakeList.txt : CMake project for ray-tracing-core, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)
set (CMAKE_CXX_STANDARD 20)

# Add include directories
include_directories (
    include
	../ray-tracing-core/include
	../ray-tracing-utility/include
	../lib
)

add_executable(ray-tracing-core-plyaground ray_tracing_playground.cpp)
target_link_libraries(ray-tracing-core-plyaground ray-tracing-core ray-tracing-utility)
if(X11_FOUND)
    target_link_libraries(ray-tracing-core-plyaground ${X11_LIBRARIES})
endif()

add_executable(ray-tracing-core-test-scene ray_tracing_test_scene.cpp)
target_link_libraries(ray-tracing-core-test-scene ray-tracing-core ray-tracing-utility)
if(X11_FOUND)
	target_link_libraries(ray-tracing-core-test-scene ${X11_LIBRARIES})
endif()

# TODO: Add tests and install targets if needed.
