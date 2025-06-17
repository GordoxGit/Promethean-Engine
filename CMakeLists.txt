cmake_minimum_required(VERSION 3.22)

# Configuration du projet
project(PrometheanEngine 
    VERSION 0.1.0
    DESCRIPTION "Minimal 2D engine using SDL2"
    LANGUAGES CXX
)

# Standards C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Configuration du build
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options du projet
option(PROMETHEAN_BUILD_TESTS "Build unit tests" ON)
option(PROMETHEAN_BUILD_EXAMPLES "Build examples" ON)

# Configuration des paths de sortie
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Configuration multiplateforme
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# Configuration vcpkg et dépendances
find_package(SDL2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

# SDL2 extensions
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)  
find_package(SDL2_ttf CONFIG REQUIRED)

# Autres dépendances
find_package(nlohmann_json CONFIG REQUIRED)

# Tests seulement si demandés
if(PROMETHEAN_BUILD_TESTS)
    find_package(GTest CONFIG REQUIRED)
    enable_testing()
endif()

# === BIBLIOTHÈQUE MOTEUR ===
add_library(engine
    # Core
    src/core/Engine.cpp
    src/core/Engine.h
)

# Alias pour usage externe
add_library(Promethean::Engine ALIAS engine)

# Configuration de la bibliothèque
target_include_directories(engine 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

# Liens des dépendances
target_link_libraries(engine 
    PUBLIC 
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        spdlog::spdlog
)

# Configuration du compilateur
target_compile_features(engine PUBLIC cxx_std_17)

# Flags de compilation par plateforme
if(MSVC)
    target_compile_options(engine PRIVATE /W4)
    target_compile_definitions(engine PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(engine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Configuration Debug/Release
target_compile_definitions(engine 
    PRIVATE
        $<$<CONFIG:Debug>:PROMETHEAN_DEBUG>
        $<$<CONFIG:Release>:PROMETHEAN_RELEASE>
)

# === EXÉCUTABLE PRINCIPAL ===
add_executable(promethean src/main.cpp)

# Configuration de l'exécutable
target_link_libraries(promethean PRIVATE Promethean::Engine)

# Propriétés de l'exécutable
set_target_properties(promethean PROPERTIES
    OUTPUT_NAME "promethean"
    DEBUG_POSTFIX "_d"
)

# === TESTS UNITAIRES ===
if(PROMETHEAN_BUILD_TESTS)
    add_executable(engine_tests 
        tests/EngineTests.cpp
    )
    
    target_link_libraries(engine_tests 
        PRIVATE 
            Promethean::Engine
            GTest::gtest_main
    )
    
    # Configuration des tests
    set_target_properties(engine_tests PROPERTIES
        OUTPUT_NAME "engine_tests"
        FOLDER "Tests"
    )
    
    # Découverte automatique des tests
    include(GoogleTest)
    gtest_discover_tests(engine_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        PROPERTIES 
            ENVIRONMENT "SDL_VIDEODRIVER=dummy"
    )
    
    # Test basique CTest
    add_test(
        NAME EngineTests 
        COMMAND engine_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    
    # Configuration de l'environnement pour les tests
    set_tests_properties(EngineTests PROPERTIES
        ENVIRONMENT "SDL_VIDEODRIVER=dummy"
        TIMEOUT 60
    )
endif()

# === INSTALLATION ===
include(GNUInstallDirs)

# Installation de la bibliothèque
install(TARGETS engine
    EXPORT PrometheanEngineTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Installation des headers
install(DIRECTORY src/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/promethean
    FILES_MATCHING PATTERN "*.h"
)

# Installation de l'exécutable
install(TARGETS promethean
    DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Configuration du package
install(EXPORT PrometheanEngineTargets
    FILE PrometheanEngineTargets.cmake
    NAMESPACE Promethean::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PrometheanEngine
)

# === CONFIGURATION ANDROID ===
if(ANDROID)
    # Configuration spécifique Android
    target_compile_definitions(engine PRIVATE PROMETHEAN_ANDROID)
    
    # Liens Android spécifiques
    target_link_libraries(engine PUBLIC android log)
    
    # Configuration des assets Android
    set_target_properties(promethean PROPERTIES
        ANDROID_PACKAGE_NAME "com.promethean.engine"
    )
endif()

# === RÉSUMÉ DE CONFIGURATION ===
message(STATUS "=== Promethean Engine Configuration ===")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Build tests: ${PROMETHEAN_BUILD_TESTS}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

if(ANDROID)
    message(STATUS "Android ABI: ${ANDROID_ABI}")
    message(STATUS "Android Platform: ${ANDROID_PLATFORM}")
    message(STATUS "Android STL: ${ANDROID_STL}")
endif()

message(STATUS "=====================================")

# === CPACK CONFIGURATION ===
set(CPACK_PACKAGE_NAME "PrometheanEngine")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR "Promethean Engine Team")

include(CPack)
