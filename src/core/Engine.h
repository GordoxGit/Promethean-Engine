#ifndef PROMETHEAN_ENGINE_H
#define PROMETHEAN_ENGINE_H

#include <memory>

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
};

} // namespace Promethean

#endif // PROMETHEAN_ENGINE_H
