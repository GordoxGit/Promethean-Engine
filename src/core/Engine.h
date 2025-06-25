#ifndef PROMETHEAN_ENGINE_H
#define PROMETHEAN_ENGINE_H

#include <memory>
#include "core/StateStack.h"
#include "renderer/BatchRenderer.h"
#include "debug/ImGuiLayer.h"
#include "audio/AudioEngine.h"

// Forward declarations pour éviter les dépendances cycliques
struct SDL_Window;

namespace Promethean {

/**
 * \brief Core engine class handling SDL initialization and main window.
 */
class Engine {
public:
    Engine();
    ~Engine();

    /**
     * \brief Initialize the engine and create the main window.
     * \return true on success.
     */
    bool Initialize();

    /**
     * \brief Shutdown the engine and free resources.
     */
    void Shutdown();

    /** Run the main loop until no state remains. */
    void Run();

    /** Push an initial state onto the stack. */
    void RegisterState(std::unique_ptr<State> state);

    /**
     * \brief Check if the engine is initialized.
     */
    [[nodiscard]] bool IsInitialized() const { return m_initialized; }

private:
    struct SDLWindowDeleter {
        void operator()(SDL_Window* w) const;
    };

    bool m_initialized{false};
    bool m_headless{false};
    std::unique_ptr<SDL_Window, SDLWindowDeleter> m_window;
    SDL_GLContext m_glContext{nullptr};
    BatchRenderer m_renderer;
    AudioEngine   m_audioEngine;
    ImGuiLayer    m_imgui;
    StateStack    m_states;
};

} // namespace Promethean

#endif // PROMETHEAN_ENGINE_H
