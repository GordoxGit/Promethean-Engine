option(PE_CI "Build running in CI" OFF)

if(PE_CI)
    add_compile_definitions(PE_CI=1)
endif()
