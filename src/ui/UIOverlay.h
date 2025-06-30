#pragma once

#include "core/State.h"
#include "ui/UIManager.h"

/**
 * @brief State overlay hosting UI windows managed by UIManager.
 */
class UIOverlay : public State {
public:
    UIManager& GetManager() { return m_manager; }

    void HandleEvent(const SDL_Event& ev) override { m_manager.HandleEvent(ev); }
    void Update(float) override {}
    void Render(BatchRenderer& r) override { m_manager.Render(r); }

private:
    UIManager m_manager;
};
