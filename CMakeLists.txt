cmake_minimum_required(VERSION 3.25)
project(PrometheanEngine VERSION 0.1 LANGUAGES CXX)

# Enable C++20 and warnings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

add_subdirectory(src)

enable_testing()
add_subdirectory(tests)
