add_executable(promethean-engine main.cpp)

# Warnings as errors
if(MSVC)
    target_compile_options(promethean-engine PRIVATE /W4 /WX)
else()
    target_compile_options(promethean-engine PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

set_target_properties(promethean-engine PROPERTIES
    OUTPUT_NAME "promethean-engine"
)
