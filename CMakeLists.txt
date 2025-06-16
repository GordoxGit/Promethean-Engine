cmake_minimum_required(VERSION 3.22)
project(PrometheanEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)
add_library(engine
    src/core/Engine.cpp
    src/core/Engine.h
)

target_include_directories(engine PUBLIC src)
target_link_libraries(engine PUBLIC SDL2::SDL2 SDL2::SDL2main spdlog::spdlog)

add_executable(promethean src/main.cpp)
target_link_libraries(promethean PRIVATE engine)

# Tests
enable_testing()
add_executable(engine_tests tests/EngineTests.cpp)
target_link_libraries(engine_tests PRIVATE engine GTest::gtest_main)
add_test(NAME EngineTests COMMAND engine_tests)
