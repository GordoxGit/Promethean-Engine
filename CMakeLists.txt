cmake_minimum_required(VERSION 3.22)

# ====================================================================
#                   Configuration du projet
# ====================================================================
project(PrometheanEngine
    VERSION     0.1.0
    DESCRIPTION "Minimal 2D engine using SDL2"
    LANGUAGES   CXX
)

# ====================================================================
#                  Choix de la pile graphique
# ====================================================================
if(ANDROID)
    set(PROM_GFX_LIB GLESv2)                         # OpenGL ES
else()
    find_package(OpenGL 3.3 REQUIRED)                # Desktop GL
    find_package(GLEW   REQUIRED)
    set(PROM_GFX_LIB OpenGL::GL GLEW::GLEW)
endif()

# ====================================================================
#                       Standard / flags C++
# ====================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PROMETHEAN_BUILD_TESTS    "Build unit tests"    ON)
option(PROMETHEAN_BUILD_EXAMPLES "Build examples"      ON)

if(ANDROID)
    set(PROMETHEAN_BUILD_TESTS OFF CACHE BOOL "Tests disabled on Android" FORCE)
endif()

# Répertoires de sortie
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# ====================================================================
#                         Dépendances vcpkg
# ====================================================================
find_package(SDL2          CONFIG REQUIRED)          # toujours
find_package(spdlog        CONFIG REQUIRED)
find_package(glm           CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

if(NOT ANDROID)                                     # extensions SDL2 uniquement desktop
    find_package(SDL2_image CONFIG REQUIRED)
    find_package(SDL2_mixer CONFIG REQUIRED)
    find_package(SDL2_ttf   CONFIG REQUIRED)
endif()

# ====================================================================
#                         Bibliothèque moteur
# ====================================================================
add_library(engine
    # ── Core ────────────────────────────────────────────────────────
    src/core/Engine.cpp                src/core/Engine.h
    src/core/LogSystem.cpp             src/core/LogSystem.h
    src/core/EventBus.cpp              src/core/EventBus.h
    src/core/StateStack.cpp            src/core/StateStack.h
    src/core/State.h
    # ── Renderer ────────────────────────────────────────────────────
    src/renderer/BatchRenderer.cpp     src/renderer/BatchRenderer.h
    src/renderer/RenderEvents.h
    # ── Input ---------------------------------------------------------------
    src/input/ActionMapper.cpp         src/input/ActionMapper.h
    src/input/InputManager.cpp         src/input/InputManager.h
    src/input/PlayerAction.h
    src/input/InputEvents.h
    # ── Debug ---------------------------------------------------------------
    src/debug/DebugOverlay.cpp         src/debug/DebugOverlay.h
    src/debug/DebugPrimitives.h
    # ── Audio ---------------------------------------------------------------
    src/audio/AudioEngine.cpp          src/audio/AudioEngine.h
    src/audio/Sound.cpp                src/audio/Sound.h
    src/audio/Music.cpp                src/audio/Music.h
    # ── UI -------------------------------------------------------------------
    src/ui/Button.cpp                  src/ui/Button.h
    src/ui/HorizontalLayout.cpp        src/ui/HorizontalLayout.h
    src/ui/VerticalLayout.cpp          src/ui/VerticalLayout.h
    src/ui/Slider.cpp                  src/ui/Slider.h
    src/ui/WidgetContainer.h
    src/ui/Widget.h
    # ── Assets ---------------------------------------------------------------
    src/assets/AssetManager.cpp        src/assets/AssetManager.h
)
add_library(Promethean::Engine ALIAS engine)

target_include_directories(engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

# --------------------------------------------------------------------
# Gestion explicite de SDL2 (core + main) pour éviter les alias manquants
# --------------------------------------------------------------------
if(TARGET SDL2::SDL2)
    set(PROM_SDL2_TARGET SDL2::SDL2)
elseif(TARGET SDL2::SDL2-static)                     # triplet static
    set(PROM_SDL2_TARGET SDL2::SDL2-static)
else()
    message(FATAL_ERROR "Ni SDL2::SDL2 ni SDL2::SDL2-static n'ont été trouvés !")
endif()

if(TARGET SDL2::SDL2main)                            # optionnel hors Linux
    set(PROM_SDL2MAIN_TARGET SDL2::SDL2main)
endif()

# --------------------------------------------------------------------
# Liaison des bibliothèques du moteur
# --------------------------------------------------------------------
target_link_libraries(engine
    PUBLIC
        ${PROM_SDL2MAIN_TARGET}
        ${PROM_SDL2_TARGET}
        ${PROM_GFX_LIB}
        spdlog::spdlog
        glm::glm
        $<$<NOT:$<BOOL:${ANDROID}>>:                 # ⇐ extensions uniquement desktop
            $<$<TARGET_EXISTS:SDL2_image::SDL2_image>:SDL2_image::SDL2_image>
            $<$<NOT:$<TARGET_EXISTS:SDL2_image::SDL2_image>>:SDL2_image::SDL2_image-static>
            $<$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>:SDL2_mixer::SDL2_mixer>
            $<$<NOT:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>>:SDL2_mixer::SDL2_mixer-static>
            $<$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>:SDL2_ttf::SDL2_ttf>
            $<$<NOT:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>>:SDL2_ttf::SDL2_ttf-static>
        >
)

target_compile_features(engine PUBLIC cxx_std_17)

if(MSVC)
    target_compile_options(engine PRIVATE /W4)
    target_compile_definitions(engine PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_definitions(engine PRIVATE
    $<$<CONFIG:Debug>:PROMETHEAN_DEBUG>
    $<$<CONFIG:Release>:PROMETHEAN_RELEASE>
)

# --------------------------------------------------------------------
# Mode « head-less » pour les tests CI desktop
# --------------------------------------------------------------------
if(PROMETHEAN_BUILD_TESTS AND NOT ANDROID)
    target_compile_definitions(engine PRIVATE HEADLESS_GL TESTING)
endif()

# ====================================================================
#                Spécificités de la cible Android
# ====================================================================
if(ANDROID)
    target_compile_definitions(engine PRIVATE PROMETHEAN_ANDROID GRAPHICS_API_GLES)

    target_link_libraries(engine PUBLIC
        android
        log
        GLESv3
        OpenSLES
    )

    set_target_properties(engine PROPERTIES
        ANDROID_GUI        TRUE
        ANDROID_SKIP_STRIP FALSE
    )

    target_link_options(engine PRIVATE
        -Wl,--no-undefined
        -Wl,--gc-sections
    )
endif()

# ====================================================================
#                       Exécutable de démo
# ====================================================================
if(NOT ANDROID OR PROMETHEAN_BUILD_EXAMPLES)
    add_executable(promethean src/main.cpp)
    target_link_libraries(promethean PRIVATE Promethean::Engine)

    set_target_properties(promethean PROPERTIES
        OUTPUT_NAME   "promethean"
        DEBUG_POSTFIX "_d"
    )

    if(ANDROID)
        set_target_properties(promethean PROPERTIES ANDROID_GUI TRUE)
        target_link_libraries(promethean PRIVATE android log atomic m)
    endif()
endif()

# ====================================================================
#                           Tests unitaires
# ====================================================================
if(PROMETHEAN_BUILD_TESTS AND NOT ANDROID)
    find_package(GTest CONFIG REQUIRED)
    enable_testing()

    add_executable(engine_tests
        tests/EngineTests.cpp
        tests/core/TestLogSystem.cpp
        tests/core/TestEventBus.cpp
        tests/core/TestStateStack.cpp
        tests/renderer/TestBatchRenderer.cpp
        tests/debug/TestDebugOverlay.cpp
        tests/assets/TestAssetManager.cpp
        tests/input/TestActionMapper.cpp
        tests/ui/TestButton.cpp
        tests/ui/TestLayout.cpp
        tests/ui/TestSlider.cpp
        tests/audio/TestAudioEngine.cpp
    )

    target_link_libraries(engine_tests
        PRIVATE Promethean::Engine GTest::gtest_main
    )
    target_compile_definitions(engine_tests PRIVATE TESTING)

    include(GoogleTest)
    gtest_discover_tests(engine_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        PROPERTIES ENVIRONMENT "SDL_VIDEODRIVER=dummy;SDL_AUDIODRIVER=dummy"
    )

    add_test(NAME EngineTests COMMAND engine_tests WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    set_tests_properties(EngineTests PROPERTIES ENVIRONMENT "SDL_VIDEODRIVER=dummy;SDL_AUDIODRIVER=dummy;DISPLAY=" TIMEOUT 60)
endif()

# ====================================================================
#                             Installation
# ====================================================================
include(GNUInstallDirs)

install(TARGETS engine
        EXPORT  PrometheanEngineTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/promethean FILES_MATCHING PATTERN "*.h")

if(TARGET promethean)
    install(TARGETS promethean DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()

install(EXPORT PrometheanEngineTargets
        FILE        PrometheanEngineTargets.cmake
        NAMESPACE   Promethean::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PrometheanEngine)

# ====================================================================
#                         Résumé de configuration
# ====================================================================
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
message(STATUS "======================================")

# ====================================================================
#                    Packaging (desktop uniquement)
# ====================================================================
if(NOT ANDROID)
    set(CPACK_PACKAGE_NAME                "PrometheanEngine")
    set(CPACK_PACKAGE_VERSION             "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
    set(CPACK_PACKAGE_VENDOR              "Promethean Engine Team")
    include(CPack)
endif()
