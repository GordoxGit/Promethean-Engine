#include "core/Engine.h"
#include "core/StateStack.h"
#include "renderer/BatchRenderer.h"

#include <cstdlib>

// Inclusions conditionnelles pour éviter les problèmes de compilation
#ifndef PROMETHEAN_ANDROID_CI
    #include <SDL.h>
    #include <spdlog/spdlog.h>
#endif

// Cross-platform compatibility
#ifdef _WIN32
    #include <windows.h>
    #define setenv(name, value, overwrite) _putenv_s(name, value)
#endif

namespace Promethean {

// Implémentation du deleter pour SDL_Window
void Engine::SDLWindowDeleter::operator()(SDL_Window* w) const {
#ifndef PROMETHEAN_ANDROID_CI
    if (w) SDL_DestroyWindow(w);
#endif
}

Engine::Engine() = default;

Engine::~Engine() {
    Shutdown();
}

bool Engine::Initialize() {
#ifdef PROMETHEAN_ANDROID_CI
    // Mode CI Android simplifié - pas d'initialisation réelle
    m_initialized = true;
    return true;
#else
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
#endif // PROMETHEAN_ANDROID_CI
}

void Engine::Shutdown() {
#ifdef PROMETHEAN_ANDROID_CI
    m_initialized = false;
    return;
#else
    if (!m_initialized)
        return;

    if (m_window) {
        m_window.reset();
        spdlog::debug("Window destroyed");
    }

    SDL_Quit();
    spdlog::info("Engine shutdown complete");
    m_initialized = false;
#endif
}

void Engine::RegisterState(std::unique_ptr<State> state)
{
    m_states.Request(StateAction::Push, std::move(state));
}

void Engine::Run()
{
#ifdef PROMETHEAN_ANDROID_CI
    m_states.ApplyRequests();
    return;
#else
    if (!m_initialized)
        return;

    if (!m_renderer.Init())
        return;

    bool running = true;
    Uint32 last = SDL_GetTicks();

    while (running)
    {
        SDL_Event ev;
        while (SDL_PollEvent(&ev))
        {
            if (ev.type == SDL_QUIT)
                running = false;
            else if (ev.type == SDL_KEYDOWN && ev.key.keysym.sym == SDLK_ESCAPE)
                running = false;

            m_states.HandleEvent(ev);
        }

        Uint32 now = SDL_GetTicks();
        float dt = static_cast<float>(now - last) / 1000.f;
        last = now;

        m_states.Update(dt);
        m_states.ApplyRequests();

        m_renderer.Begin(1280, 720);
        m_states.Render(m_renderer);
        m_renderer.End();

        SDL_Delay(1);
        if (m_states.Size() == 0)
            running = false;
    }

    m_renderer.Shutdown();
#endif
}

} // namespace Promethean
