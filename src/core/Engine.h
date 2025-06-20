#ifndef PROMETHEAN_ENGINE_H
#define PROMETHEAN_ENGINE_H

#include <memory>
#include "core/StateStack.h"
#include "renderer/BatchRenderer.h"

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
    std::unique_ptr<SDL_Window, SDLWindowDeleter> m_window;
    BatchRenderer m_renderer;
    StateStack    m_states;
};

} // namespace Promethean

#endif // PROMETHEAN_ENGINE_H
