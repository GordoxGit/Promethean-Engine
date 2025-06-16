#include "Engine.h"

#include <SDL.h>
#include <spdlog/spdlog.h>
#include <cstdlib>

namespace Promethean {

Engine::Engine() = default;

Engine::~Engine() {
    Shutdown();
}

bool Engine::Initialize() {
    if (m_initialized)
        return true;

    // Ensure headless environments work
    if(!std::getenv("DISPLAY")) {
        setenv("SDL_VIDEODRIVER", "dummy", 0);
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        spdlog::error("SDL_Init failed: {}", SDL_GetError());
        return false;
    }

    m_window.reset(SDL_CreateWindow("Promethean Engine",
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   1280,
                                   720,
                                   SDL_WINDOW_SHOWN));
    if (!m_window) {
        spdlog::error("Failed to create window: {}", SDL_GetError());
        SDL_Quit();
        return false;
    }

    spdlog::info("Engine initialized successfully");
    m_initialized = true;
    return true;
}

void Engine::Shutdown() {
    if (!m_initialized)
        return;

    m_window.reset();
    SDL_Quit();
    spdlog::info("Engine shutdown");
    m_initialized = false;
}

} // namespace Promethean

