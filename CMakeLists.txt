# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.12)

#set_property(TARGET tgt PROPERTY CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD 20)

# CMAKE_<LANG>_COMPILER_ID
# https://cmake.org/cmake/help/v3.0/variable/CMAKE_LANG_COMPILER_ID.html
if (MAKE_CONFIG STREQUAL "MSVC")
    # [...]
elseif (MAKE_CONFIG STREQUAL "MINGW")
    add_compile_options(
        -Wno-volatile
    )
elseif (MAKE_CONFIG STREQUAL "Clang")
    # [...]
endif()

project ("ray-tracing-cpp")

find_package(X11)
if(X11_FOUND)
    add_definitions(-DCMAKE_HAS_X)
    include_directories(${X11_INCLUDE_DIR})
endif()

include_directories (
    "test-suite"
    "lib/glm"
    "lib"
    "lib/rapidjson/include"
)

ENABLE_TESTING ()

add_subdirectory (ray-tracing-core)
add_subdirectory (ray-tracing-utility)
add_subdirectory (ray-tracing-playground)
