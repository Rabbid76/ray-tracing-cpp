cmake_minimum_required(VERSION 3.12)
set (CMAKE_CXX_STANDARD 20)
project (ray-tracing-core-unit-test)

include_directories (
    "./")

file(GLOB rtc_core_test_source 
    "*/*.cpp"
    "*/*.h")

add_executable (ray-tracing-core-test 
    ${rtc_core_test_source}
    "test_main.cpp"
    "rtc_test_assert.h")
target_link_libraries(ray-tracing-core-test ray-tracing-core)

add_test (NAME unit-test.ray-tracing-core.math COMMAND ray-tracing-core-test "math")
add_test (NAME unit-test.ray-tracing-core.pdf COMMAND ray-tracing-core-test "pdf")
add_test (NAME unit-test.ray-tracing-core.texture COMMAND ray-tracing-core-test "texture")
add_test (NAME unit-test.ray-tracing-core.geometry COMMAND ray-tracing-core-test "geometry")
add_test (NAME unit-test.ray-tracing-core.material COMMAND ray-tracing-core-test "material")
add_test (NAME unit-test.ray-tracing-core.core COMMAND ray-tracing-core-test "core")
add_test (NAME unit-test.ray-tracing-core.environment COMMAND ray-tracing-core-test "environment")
