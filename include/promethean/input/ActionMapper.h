#pragma once
#include <unordered_map>
#include <array>
#include <vector>
#include "SDL_scancode.h"
#include "SDL_events.h"
#include "PlayerAction.h"
#include "core/EventBus.h"
#include "InputEvents.h"

namespace Promethean {

/**
 * @brief Maps device inputs to logical player actions.
 */
class ActionMapper {
public:
    /** Associate an SDL scancode with a player action. */
    void mapKey(SDL_Scancode key, PlayerAction action);
#if defined(__ANDROID__)
    /** Associate a rectangular touch area with a player action. */
    void mapTouchArea(const SDL_Rect& area, PlayerAction action);
#endif
    /** Returns true if the action is currently pressed. */
    bool isActionPressed(PlayerAction action) const;

    /** Update action states from an SDL event. */
    void handleEvent(const SDL_Event& e);

private:
    std::unordered_map<int, PlayerAction> m_keyToAction;
#if defined(__ANDROID__)
    std::vector<std::pair<SDL_Rect, PlayerAction>> m_touchAreas;
#endif
    std::array<bool, PlayerActionCount> m_actionState{};
};

} // namespace Promethean
