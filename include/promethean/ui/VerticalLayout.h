#pragma once

#include "ui/WidgetContainer.h"
#include <cassert>

/**
 * @brief Layout arranging children vertically with spacing.
 */
class VerticalLayout : public WidgetContainer {
public:
    explicit VerticalLayout(float spacing = 5.f) : m_spacing(spacing) {}

    void AddChild(Widget* child);
    void SetPosition(const glm::vec2& pos) override;
    void SetSize(const glm::vec2& size) override;
    void Draw(BatchRenderer& renderer) override;
    bool HandleEvent(const SDL_Event& event) override { return WidgetContainer::HandleEvent(event); }

private:
    void Recompute();
    float m_spacing;
};
