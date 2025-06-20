#pragma once

#include "renderer/BatchRenderer.h"
#include <SDL.h>

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
};

