cmake_minimum_required(VERSION 3.16)
project(raytracer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Type de build" FORCE)
endif()

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
target_compile_definitions(raytracer_lib PUBLIC RT_SOURCE_DIR="${CMAKE_SOURCE_DIR}")
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(raytracer_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()
target_link_libraries(raytracer_lib PUBLIC Threads::Threads)

add_executable(raytracer src/main.cpp)
target_link_libraries(raytracer PRIVATE raytracer_lib)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(raytracer PRIVATE -Wall -Wextra -Wpedantic)
endif()

option(RT_BUILD_TESTS "Compiler les tests" ON)
option(RT_SANITIZE "ASan+UBSan" OFF)
# Pour rechercher les data races, utiliser -fsanitize=thread séparément.
if(RT_SANITIZE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

if(RT_BUILD_TESTS)
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
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(raytracer_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    include(GoogleTest)
    gtest_discover_tests(raytracer_tests)
endif()

add_executable(raytracer_benchmark bench/benchmark.cpp)
target_link_libraries(raytracer_benchmark PRIVATE raytracer_lib)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(raytracer_benchmark PRIVATE -Wall -Wextra -Wpedantic)
endif()
