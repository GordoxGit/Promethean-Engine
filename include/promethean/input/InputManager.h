#pragma once
#include <array>
#include <memory>
#include "SDL_events.h"
#include "ActionMapper.h"
#include "PlayerAction.h"

namespace Promethean {

/**
 * @brief Centralized input manager keeping raw device state.
 */
class InputManager {
public:
    InputManager();

    /** Process an SDL event to update input state. */
    void HandleEvent(const SDL_Event& e);

    /** Update state each frame (currently no-op). */
    void Update();

    /** Check if a key is pressed. */
    bool IsKeyPressed(int keycode) const;
    /** Check if a mouse button is pressed. */
    bool IsMouseButtonPressed(int button) const;
    /** True if pointer or first finger is down. */
    bool IsPointerDown() const { return m_pointerDown; }
    /** Get current pointer position. */
    void GetPointerPosition(float& x, float& y) const;

    /** Query a mapped action state. */
    bool IsActionPressed(PlayerAction action) const;

    /** Access the internal mapper. */
    ActionMapper& GetMapper();

private:
    std::array<bool, SDL_NUM_SCANCODES> m_keyState{};
    std::array<bool, 8>                 m_mouseButtons{};
    bool   m_pointerDown{false};
    float  m_pointerX{0.f};
    float  m_pointerY{0.f};

    std::unique_ptr<ActionMapper> m_mapper;
};

} // namespace Promethean
