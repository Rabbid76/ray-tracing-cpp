cmake_minimum_required(VERSION 3.12)
set (CMAKE_CXX_STANDARD 20)

include_directories (
    "./")

file(GLOB rtc_core_test_source 
    "*/*.cpp"
    "*/*.h")

add_executable (ray-tracing-core-test 
    ${rtc_core_test_source}
    "test_main.cpp"
    "rtc_core_test_assert.h")
target_link_libraries(ray-tracing-core-test ray-tracing-core)

add_test (NAME ray-tracing-core-unit-test COMMAND ray-tracing-core-test)

