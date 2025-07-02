#pragma once

#include "renderer/BatchRenderer.h"
#include <SDL.h>
#include <glm/vec2.hpp>

/**
 * @brief Base class for UI widgets.
 */
class Widget {
public:
    virtual ~Widget() = default;

    /** Draw the widget using the provided renderer. */
    virtual void Draw(BatchRenderer& renderer) = 0;

    /** Handle an SDL event. */
    virtual bool HandleEvent(const SDL_Event& event) = 0;

    /** Set widget top-left position in pixels. */
    virtual void SetPosition(const glm::vec2& pos) = 0;

    /** Set widget size in pixels. */
    virtual void SetSize(const glm::vec2& size) = 0;
};

