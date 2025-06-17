#include "Engine.h"

#include <SDL.h>
#include <spdlog/spdlog.h>
#include <cstdlib>

// Cross-platform compatibility
#ifdef _WIN32
    #include <windows.h>
    #define setenv(name, value, overwrite) _putenv_s(name, value)
#endif

namespace Promethean {

Engine::Engine() = default;

Engine::~Engine() {
    Shutdown();
}

bool Engine::Initialize() {
    if (m_initialized)
        return true;

    // Configure SDL for headless environments (Linux CI/CD)
#if defined(__linux__) && !defined(PROMETHEAN_ANDROID)
    if (!std::getenv("DISPLAY")) {
        spdlog::info("No DISPLAY found, setting SDL to dummy video driver for headless mode");
        if (setenv("SDL_VIDEODRIVER", "dummy", 1) != 0) {
            spdlog::warn("Failed to set SDL_VIDEODRIVER to dummy");
        }
    }
#endif

    // Initialize SDL subsystems
    Uint32 sdlFlags = SDL_INIT_VIDEO;
    
    // Only initialize audio on platforms that support it properly
#ifndef PROMETHEAN_ANDROID
    sdlFlags |= SDL_INIT_AUDIO;
#endif

    if (SDL_Init(sdlFlags) != 0) {
        spdlog::error("SDL_Init failed: {}", SDL_GetError());
        return false;
    }

    // Create window with platform-specific settings
    Uint32 windowFlags = SDL_WINDOW_SHOWN;
    
#ifdef PROMETHEAN_ANDROID
    // Android-specific window configuration
    windowFlags |= SDL_WINDOW_FULLSCREEN;
#endif

    m_window.reset(SDL_CreateWindow("Promethean Engine",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   1280,
                                   720,
                                   windowFlags));
    
    if (!m_window) {
        spdlog::error("Failed to create window: {}", SDL_GetError());
        SDL_Quit();
        return false;
    }

#ifdef PROMETHEAN_ANDROID
    spdlog::info("Engine initialized successfully for Android");
#else
    spdlog::info("Engine initialized successfully for desktop platform");
#endif

    m_initialized = true;
    return true;
}

void Engine::Shutdown() {
    if (!m_initialized)
        return;

    if (m_window) {
        m_window.reset();
        spdlog::debug("Window destroyed");
    }

    SDL_Quit();
    spdlog::info("Engine shutdown complete");
    m_initialized = false;
}

} // namespace Promethean
