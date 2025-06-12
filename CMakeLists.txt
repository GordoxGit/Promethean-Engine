cmake_minimum_required(VERSION 3.20)
project(PixelDepths LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)

add_executable(PixelDepths
    src/main.cpp
    src/core/Engine.cpp
)

target_include_directories(PixelDepths PRIVATE src src/core)

target_link_libraries(PixelDepths SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer)
