#pragma once

#include "ui/Widget.h"
#include <vector>
#include <glm/vec2.hpp>

/**
 * @brief Base widget container storing children.
 */
class WidgetContainer : public Widget {
public:
    void AddChild(Widget* child) { m_children.push_back(child); }

    void Draw(BatchRenderer& renderer) override {
        for(auto* c : m_children) c->Draw(renderer);
    }

    bool HandleEvent(const SDL_Event& event) override {
        bool handled = false;
        for(auto* c : m_children)
            handled = c->HandleEvent(event) || handled;
        return handled;
    }

    void SetPosition(const glm::vec2& pos) override { m_position = pos; }
    void SetSize(const glm::vec2& size) override { m_size = size; }

protected:
    glm::vec2 m_position{0.f,0.f};
    glm::vec2 m_size{0.f,0.f};
    std::vector<Widget*> m_children;
};
