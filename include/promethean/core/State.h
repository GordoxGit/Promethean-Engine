#pragma once
#include <SDL.h>
#include "renderer/BatchRenderer.h"

/**
 * \brief Base interface for a game state.
 */
class State {
public:
    virtual ~State() = default;

    /** Called when the state becomes active. */
    virtual void OnEnter() {}

    /** Called when the state is leaving. */
    virtual void OnExit() {}

    /** Called when another state is pushed on top. */
    virtual void Pause() {}

    /** Called when the state becomes active again. */
    virtual void Resume() {}

    /** Forward an SDL event to the state. */
    virtual void HandleEvent(const SDL_Event& ev) {}

    /** Update logic with delta time in seconds. */
    virtual void Update(float dt) = 0;

    /** Render the state using the provided renderer. */
    virtual void Render(BatchRenderer& renderer) = 0;
};
