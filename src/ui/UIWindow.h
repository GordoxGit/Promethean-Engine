#pragma once

#include "ui/WidgetContainer.h"
#include <functional>

/**
 * @brief Basic UI window holding widgets. Can be shown or hidden.
 */
class UIWindow : public WidgetContainer {
public:
    using Callback = std::function<void()>;

    /// Set callbacks invoked when the window is shown/hidden.
    void SetOnShow(Callback cb) { m_onShow = std::move(cb); }
    void SetOnHide(Callback cb) { m_onHide = std::move(cb); }

    /// Show the window (calls OnShow).
    void Show();
    /// Hide the window (calls OnHide).
    void Hide();
    bool IsVisible() const { return m_visible; }

    void Draw(BatchRenderer& renderer) override;
    bool HandleEvent(const SDL_Event& event) override;

private:
    bool     m_visible{false};
    Callback m_onShow;
    Callback m_onHide;
};

