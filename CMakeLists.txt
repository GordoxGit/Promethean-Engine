cmake_minimum_required(VERSION 3.22)

# =====================================================
# Configuration du projet
# =====================================================
project(PrometheanEngine
    VERSION 0.1.0
    DESCRIPTION "Minimal 2D engine using SDL2"
    LANGUAGES CXX
)

# =====================================================
# Sélection de la lib OpenGL / OpenGL ES
# =====================================================
if(ANDROID)
    # → Build mobile : OpenGL ES 3.x fourni par le NDK
    set(PROM_GFX_LIB GLESv2)                      # <- plus de find_package(OpenGLES)
else()
    # → Build desktop : OpenGL + GLEW (installé via vcpkg)
    find_package(OpenGL 3.3 REQUIRED)
    find_package(GLEW   REQUIRED)
    set(PROM_GFX_LIB OpenGL::GL GLEW::GLEW)
endif()

# =====================================================
# Standards C++
# =====================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuration du build
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options du projet
option(PROMETHEAN_BUILD_TESTS "Build unit tests" ON)
option(PROMETHEAN_BUILD_EXAMPLES "Build examples" ON)

# Désactiver les tests sur Android par défaut
if(ANDROID)
    set(PROMETHEAN_BUILD_TESTS OFF CACHE BOOL "Tests disabled on Android" FORCE)
endif()

# Configuration des paths de sortie
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Configuration multiplateforme
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# =====================================================
# Dépendances (via vcpkg)
# =====================================================
find_package(SDL2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# SDL2 extensions (optionnelles pour Android)
if(NOT ANDROID)
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    find_package(SDL2_ttf   CONFIG REQUIRED)
endif()

find_package(nlohmann_json CONFIG REQUIRED)
# (find_package(OpenGL REQUIRED) supprimé – déjà géré plus haut)

# =====================================================
# Bibliothèque moteur
# =====================================================
add_library(engine
    # Core
    src/core/Engine.cpp
    src/core/Engine.h
    src/core/LogSystem.cpp
    src/core/LogSystem.h
    src/core/EventBus.cpp
    src/core/EventBus.h
    # Renderer
    src/renderer/BatchRenderer.cpp
    src/renderer/BatchRenderer.h
    src/renderer/RenderEvents.h
)
add_library(Promethean::Engine ALIAS engine)

target_include_directories(engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

# Liens des dépendances de base
target_link_libraries(engine
    PUBLIC
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        ${PROM_GFX_LIB}                # <- remplace l’ancienne référence fixe
        spdlog::spdlog
        glm::glm
)

target_compile_features(engine PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(engine PRIVATE /W4)
    target_compile_definitions(engine PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_definitions(engine
    PRIVATE
        $<$<CONFIG:Debug>:PROMETHEAN_DEBUG>
        $<$<CONFIG:Release>:PROMETHEAN_RELEASE>
)

if(PROMETHEAN_BUILD_TESTS AND NOT ANDROID)
    target_compile_definitions(engine PUBLIC TESTING)
endif()

# === CONFIGURATION ANDROID SPÉCIFIQUE ===
if(ANDROID)
    target_compile_definitions(engine PRIVATE PROMETHEAN_ANDROID GRAPHICS_API_GLES)

    target_link_libraries(engine PUBLIC
        android
        log
        GLESv1_CM
        GLESv2
        OpenSLES
    )

    set_target_properties(engine PROPERTIES
        ANDROID_GUI TRUE
        ANDROID_SKIP_STRIP FALSE
    )

    target_link_options(engine PRIVATE
        -Wl,--no-undefined
        -Wl,--gc-sections
    )
endif()

# =====================================================
# Exécutable principal (sauf si Android sans exemples)
# =====================================================
if(NOT ANDROID OR PROMETHEAN_BUILD_EXAMPLES)
    add_executable(promethean src/main.cpp)
    target_link_libraries(promethean PRIVATE Promethean::Engine)

    set_target_properties(promethean PROPERTIES
        OUTPUT_NAME "promethean"
        DEBUG_POSTFIX "_d"
    )

    if(ANDROID)
        set_target_properties(promethean PROPERTIES ANDROID_GUI TRUE)
        target_link_libraries(promethean PRIVATE android log atomic m)
    endif()
endif()

# =====================================================
# Tests unitaires
# =====================================================
if(PROMETHEAN_BUILD_TESTS AND NOT ANDROID)
    find_package(GTest CONFIG REQUIRED)
    enable_testing()

    add_executable(engine_tests
        tests/EngineTests.cpp
        tests/core/TestLogSystem.cpp
        tests/core/TestEventBus.cpp
        tests/renderer/TestBatchRenderer.cpp
    )

    target_link_libraries(engine_tests
        PRIVATE
            Promethean::Engine
            GTest::gtest_main
    )
    target_compile_definitions(engine_tests PRIVATE TESTING)

    include(GoogleTest)
    gtest_discover_tests(engine_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        PROPERTIES ENVIRONMENT "SDL_VIDEODRIVER=dummy"
    )

    add_test(
        NAME EngineTests
        COMMAND engine_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    set_tests_properties(EngineTests PROPERTIES
        ENVIRONMENT "SDL_VIDEODRIVER=dummy;DISPLAY="
        TIMEOUT 60
    )
endif()

# =====================================================
# Installation
# =====================================================
include(GNUInstallDirs)

install(TARGETS engine
    EXPORT PrometheanEngineTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY src/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/promethean
    FILES_MATCHING PATTERN "*.h"
)

if(TARGET promethean)
    install(TARGETS promethean DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

install(EXPORT PrometheanEngineTargets
    FILE PrometheanEngineTargets.cmake
    NAMESPACE Promethean::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PrometheanEngine
)

# =====================================================
# Résumé de configuration
# =====================================================
message(STATUS "=== Promethean Engine Configuration ===")
message(STATUS "Version:        ${PROJECT_VERSION}")
message(STATUS "Build type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "Platform:       ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler:       ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build tests:    ${PROMETHEAN_BUILD_TESTS}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

if(ANDROID)
    message(STATUS "Android ABI:      ${ANDROID_ABI}")
    message(STATUS "Android Platform: ${ANDROID_PLATFORM}")
    message(STATUS "Android STL:      ${ANDROID_STL}")
    message(STATUS "Android NDK:      ${CMAKE_ANDROID_NDK}")
endif()
message(STATUS "=====================================")

# =====================================================
# CPack (pour desktop)
# =====================================================
if(NOT ANDROID)
    set(CPACK_PACKAGE_NAME "PrometheanEngine")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_VENDOR "Promethean Engine Team")
    include(CPack)
endif()
