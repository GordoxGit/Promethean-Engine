#pragma once

#include "ui/Widget.h"
#include <functional>
#include <string>
#include <SDL.h>

/**
 * @brief Simple clickable button widget.
 */
class UIButton : public Widget {
public:
    using Callback = std::function<void()>;

    explicit UIButton(std::string id);

    void SetOnClick(Callback cb) { m_onClick = std::move(cb); }

    void Draw(BatchRenderer& renderer) override;
    bool HandleEvent(const SDL_Event& event) override;
    void SetPosition(const glm::vec2& pos) override;
    void SetSize(const glm::vec2& size) override;

#ifdef TESTING
    const SDL_Rect& GetRect() const { return m_rect; }
#endif

private:
    std::string m_id;
    SDL_Rect    m_rect{0,0,100,40};
    Callback    m_onClick;
    bool        m_pressed{false};
};
