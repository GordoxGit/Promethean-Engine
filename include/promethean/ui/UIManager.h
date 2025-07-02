#pragma once

#include "ui/UIWindow.h"
#include <vector>

/**
 * @brief Manages a stack of UI windows.
 */
class UIManager {
public:
    /// Push a window on top of the stack and show it.
    void PushWindow(UIWindow* window);
    /// Pop the top window and hide it.
    void PopWindow();
    /// Current top window (or nullptr).
    UIWindow* TopWindow() const;
    /// Number of active windows.
    size_t WindowCount() const { return m_stack.size(); }

    void HandleEvent(const SDL_Event& event);
    void Render(BatchRenderer& renderer);

private:
    std::vector<UIWindow*> m_stack;
};
