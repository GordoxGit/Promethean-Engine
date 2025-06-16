cmake_minimum_required(VERSION 3.20)
project(PrometheanEngine VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(promethean src/main.cpp)

# Set compile options
if(MSVC)
    target_compile_options(promethean PRIVATE /W4 /WX)
else()
    target_compile_options(promethean PRIVATE -Wall -Wextra -Werror)
endif()

install(TARGETS promethean)

enable_testing()
add_test(NAME run_promethean COMMAND promethean)
