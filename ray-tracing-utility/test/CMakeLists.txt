cmake_minimum_required(VERSION 3.12)
set (CMAKE_CXX_STANDARD 20)
project (ray-tracing-utility-unit-test)

include_directories (
    "./")

file(GLOB rtc_utility_test_source 
    "*/*.cpp"
    "*/*.h")

add_executable (ray-tracing-utility-test 
    ${rtc_utility_test_source}
    "test_main.cpp"
    "rtu_test_assert.h")
#target_link_libraries(ray-tracing-utility-test ray-tracing-utility)

add_test (NAME unit-test.ray-tracing-utility.iterator COMMAND ray-tracing-utility-test "iterator")