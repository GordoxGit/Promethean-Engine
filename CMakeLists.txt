cmake_minimum_required(VERSION 3.22)

# ====================================================================
#                   Configuration du projet
# ====================================================================
project(PrometheanEngine
    VERSION     0.1.0
    DESCRIPTION "Minimal 2D engine using SDL2"
    LANGUAGES   CXX
)
include(cmake/Options.cmake)

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
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(PROMETHEAN_BUILD_TESTS    "Build unit tests"    ON)
option(PROMETHEAN_BUILD_EXAMPLES "Build examples"      ON)
option(PROMETHEAN_CI_ANDROID     "Android build running in CI" OFF)
if(ANDROID)
    option(PROMETHEAN_WITH_IMGUI "Build with ImGui" OFF)
else()
    option(PROMETHEAN_WITH_IMGUI "Build with ImGui" ON)
endif()

if(ANDROID AND PROMETHEAN_CI_ANDROID)
    set(PROMETHEAN_BUILD_TESTS OFF CACHE BOOL "Tests disabled on Android CI" FORCE)
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
find_package(tinyxml2      CONFIG REQUIRED)

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
    # ── ECS ---------------------------------------------------------------
    src/ecs/Entity.cpp            src/ecs/Entity.h
    src/ecs/Registry.cpp          src/ecs/Registry.h
    src/ecs/System.h              src/ecs/Component.h
    src/ecs/ecs.h
    # ── Renderer ────────────────────────────────────────────────────
    src/renderer/BatchRenderer.cpp     src/renderer/BatchRenderer.h
    src/renderer/RenderEvents.h
    src/renderer/TileMapRenderer.cpp   src/renderer/TileMapRenderer.h
    # ── Input ---------------------------------------------------------------
    src/input/ActionMapper.cpp         src/input/ActionMapper.h
    src/input/InputManager.cpp         src/input/InputManager.h
    src/input/PlayerAction.h
    src/input/InputEvents.h
    # ── Debug ---------------------------------------------------------------
    src/debug/DebugOverlay.cpp         src/debug/DebugOverlay.h
    src/debug/DebugPrimitives.h
    src/debug/RuntimeOverlay.cpp       src/debug/RuntimeOverlay.h
    # ── Audio ---------------------------------------------------------------
    src/audio/AudioEngine.cpp          src/audio/AudioEngine.h
    src/audio/AudioSettings.cpp        src/audio/AudioSettings.h
    src/audio/Sound.cpp                src/audio/Sound.h
    src/audio/Music.cpp                src/audio/Music.h
    # ── UI -------------------------------------------------------------------
    src/ui/Button.cpp                  src/ui/Button.h
    src/ui/HorizontalLayout.cpp        src/ui/HorizontalLayout.h
    src/ui/VerticalLayout.cpp          src/ui/VerticalLayout.h
    src/ui/Slider.cpp                  src/ui/Slider.h
    src/ui/UIManager.cpp               src/ui/UIManager.h
    src/ui/UIWindow.cpp                src/ui/UIWindow.h
    src/ui/UIButton.cpp                src/ui/UIButton.h
    src/ui/UIOverlay.cpp               src/ui/UIOverlay.h
    src/ui/WidgetContainer.h
    src/ui/Widget.h
    # ── Assets ---------------------------------------------------------------
    src/assets/AssetManager.cpp        src/assets/AssetManager.h
    src/assets/TileMap.cpp             src/assets/TileMap.h
    # ── Physics --------------------------------------------------------------
    src/physics/CollisionLayer.cpp     src/physics/CollisionLayer.h
    # ── AI -------------------------------------------------------------------
    src/ai/Pathfinder.cpp              src/ai/Pathfinder.h
    # ── Pathfinding ---------------------------------------------------------
    src/pathfinding/AStar.cpp          src/pathfinding/AStar.h
    src/ecs/NavComponent.cpp           src/ecs/NavComponent.h
    src/ecs/PathfindingSystem.cpp      src/ecs/PathfindingSystem.h
    src/ecs/BehaviorComponent.cpp      src/ecs/BehaviorComponent.h
    src/ecs/BehaviorSystem.cpp         src/ecs/BehaviorSystem.h
    # ── Settings -------------------------------------------------------------
    src/core/SettingsManager.cpp       src/core/SettingsManager.h
    # ── Save ---------------------------------------------------------------
    src/save/SaveManager.cpp           src/save/SaveManager.h
    # ── Editor -------------------------------------------------------------
    src/editor/WorldEditor.cpp         src/editor/WorldEditor.h
    src/editor/EditorUI.cpp            src/editor/EditorUI.h
    src/editor/EditorState.cpp         src/editor/EditorState.h
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

if(PROMETHEAN_WITH_IMGUI)
    add_library(imgui
        third_party/imgui/imgui.cpp
        third_party/imgui/imgui_draw.cpp
        third_party/imgui/imgui_tables.cpp
        third_party/imgui/imgui_widgets.cpp
        third_party/imgui/imgui_demo.cpp
        third_party/imgui/backends/imgui_impl_sdl.cpp
        third_party/imgui/backends/imgui_impl_opengl3.cpp
    )
    target_include_directories(imgui PUBLIC
        third_party/imgui
        third_party/imgui/backends
    )
    target_link_libraries(imgui PUBLIC ${PROM_GFX_LIB} ${PROM_SDL2_TARGET})
endif()

# --------------------------------------------------------------------
# Liaison des bibliothèques du moteur
# --------------------------------------------------------------------
target_link_libraries(engine
    PUBLIC
        ${PROM_SDL2MAIN_TARGET}
        ${PROM_SDL2_TARGET}
        ${PROM_GFX_LIB}
        $<$<BOOL:${PROMETHEAN_WITH_IMGUI}>:imgui>
        spdlog::spdlog
        glm::glm
        tinyxml2::tinyxml2
        $<$<NOT:$<BOOL:${ANDROID}>>:                 # ⇐ extensions uniquement desktop
            $<$<TARGET_EXISTS:SDL2_image::SDL2_image>:SDL2_image::SDL2_image>
            $<$<NOT:$<TARGET_EXISTS:SDL2_image::SDL2_image>>:SDL2_image::SDL2_image-static>
            $<$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>:SDL2_mixer::SDL2_mixer>
            $<$<NOT:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>>:SDL2_mixer::SDL2_mixer-static>
            $<$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>:SDL2_ttf::SDL2_ttf>
            $<$<NOT:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>>:SDL2_ttf::SDL2_ttf-static>
        >
)

target_compile_features(engine PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(engine PRIVATE /W4)
    target_compile_definitions(engine PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

target_compile_definitions(engine PRIVATE
    $<$<CONFIG:Debug>:PROMETHEAN_DEBUG>
    $<$<CONFIG:Release>:PROMETHEAN_RELEASE>
    $<$<BOOL:${PROMETHEAN_WITH_IMGUI}>:PROMETHEAN_WITH_IMGUI>
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

    find_package(SDL2_mixer CONFIG REQUIRED)

    # Gérer explicitement les cibles SDL2_mixer statiques ou partagées
    if(TARGET SDL2_mixer::SDL2_mixer)
        set(PROM_SDL2_MIXER_TARGET SDL2_mixer::SDL2_mixer)
    elseif(TARGET SDL2_mixer::SDL2_mixer-static)
        set(PROM_SDL2_MIXER_TARGET SDL2_mixer::SDL2_mixer-static)
    else()
        message(FATAL_ERROR "Cible SDL2_mixer introuvable (ni SDL2_mixer::SDL2_mixer ni SDL2_mixer::SDL2_mixer-static)")
    endif()

    target_link_libraries(engine PUBLIC ${PROM_SDL2_MIXER_TARGET})

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

    set(USE_SDL_STUBS OFF)
    if(DEFINED ENV{CI_HEADLESS} AND NOT MSVC)
        set(USE_SDL_STUBS ON)
    endif()

    add_executable(engine_tests
        tests/TestMain.cpp
        tests/EngineTests.cpp
        tests/core/TestLogSystem.cpp
        tests/core/TestEventBus.cpp
        tests/core/TestStateStack.cpp
        tests/renderer/TestBatchRenderer.cpp
        tests/debug/TestDebugOverlay.cpp
        tests/debug/TestRuntimeOverlay.cpp
        tests/assets/TestAssetManager.cpp
        tests/assets/TestTileMap.cpp
        tests/physics/TestCollisionLayer.cpp
        tests/ai/TestPathfinder.cpp
        tests/input/TestActionMapper.cpp
        tests/ui/TestButton.cpp
        tests/ui/TestLayout.cpp
        tests/ui/TestSlider.cpp
        tests/ui/TestUIManager.cpp
        tests/audio/TestAudioEngine.cpp
        tests/audio/TestAudioBus.cpp
        tests/ecs/TestECS.cpp
        tests/pathfinding/TestAStar.cpp
        tests/ecs/TestPathfindingSystem.cpp
        tests/ecs/TestBehaviorSystem.cpp
        tests/save/TestSaveManager.cpp
        tests/editor/TestWorldEditor.cpp
    )

    file(GLOB TEST_AUDIO_FILES
        ${CMAKE_SOURCE_DIR}/tests/assets/audio/*)
    file(COPY ${TEST_AUDIO_FILES}
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/assets/audio)

    if(USE_SDL_STUBS)
        target_sources(engine_tests PRIVATE tests/mocks/SDL_mixer_stubs.cpp)
        target_compile_definitions(engine_tests PRIVATE USE_SDL_STUBS)
    endif()

    target_link_libraries(engine_tests
        PRIVATE Promethean::Engine GTest::gtest
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

install(DIRECTORY assets/sample_project
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/promethean)

install(FILES README.md docs/GettingStarted.md
        DESTINATION ${CMAKE_INSTALL_DOCDIR})

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
