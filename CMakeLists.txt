cmake_minimum_required(VERSION 3.16)
project(raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Type de build" FORCE)

find_package(Threads REQUIRED)
include(FetchContent)

add_library(raytracer_lib
    src/geometry/Sphere.cpp
    src/geometry/Plane.cpp
    src/geometry/Triangle.cpp
    src/camera/Camera.cpp
    src/material/Material.cpp
    src/scene/Scene.cpp
    src/scene/SceneParser.cpp
    src/render/Renderer.cpp
    src/render/Image.cpp
    src/render/ThreadPool.cpp
)
target_include_directories(raytracer_lib PUBLIC src)
target_compile_options(raytracer_lib PRIVATE -Wall -Wextra -Wpedantic -O2)
target_link_libraries(raytracer_lib PUBLIC Threads::Threads)

add_executable(raytracer src/main.cpp)
target_link_libraries(raytracer PRIVATE raytracer_lib)
target_compile_options(raytracer PRIVATE -Wall -Wextra -Wpedantic -O2)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
add_executable(raytracer_tests ${TEST_SOURCES})
target_link_libraries(raytracer_tests PRIVATE raytracer_lib GTest::gtest_main)
target_compile_options(raytracer_tests PRIVATE -Wall -Wextra -Wpedantic -O2)
include(GoogleTest)
gtest_discover_tests(raytracer_tests)

add_executable(raytracer_benchmark bench/benchmark.cpp)
target_link_libraries(raytracer_benchmark PRIVATE raytracer_lib)
target_compile_options(raytracer_benchmark PRIVATE -Wall -Wextra -Wpedantic -O2)
