cmake_minimum_required(VERSION 3.12)
set (CMAKE_CXX_STANDARD 20)

add_executable (ray-tracing-core-test "test_main.cpp"  )
ADD_TEST (ray-tracing-core-unit-test ray-tracing-core-test)