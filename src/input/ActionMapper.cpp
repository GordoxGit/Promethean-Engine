#include "input/ActionMapper.h"

namespace Promethean {

void ActionMapper::mapKey(SDL_Scancode key, PlayerAction action)
{
    m_keyToAction[static_cast<int>(key)] = action;
}

#if defined(__ANDROID__)
void ActionMapper::mapTouchArea(const SDL_Rect& area, PlayerAction action)
{
    m_touchAreas.push_back({area, action});
}
#endif

bool ActionMapper::isActionPressed(PlayerAction action) const
{
    return m_actionState[static_cast<size_t>(action)];
}

void ActionMapper::handleEvent(const SDL_Event& e)
{
    auto publish = [&](PlayerAction act, bool pressed){
        size_t idx = static_cast<size_t>(act);
        if(m_actionState[idx] != pressed) {
            m_actionState[idx] = pressed;
            EventBus::Instance().Publish(ActionStateChangedEvent{act, pressed});
        }
    };

    switch(e.type) {
    case SDL_KEYDOWN:
    case SDL_KEYUP: {
        auto it = m_keyToAction.find(static_cast<int>(e.key.keysym.scancode));
        if(it != m_keyToAction.end())
            publish(it->second, e.type == SDL_KEYDOWN);
        break;
    }
#if defined(__ANDROID__)
    case SDL_FINGERDOWN:
    case SDL_FINGERUP: {
        const int x = static_cast<int>(e.tfinger.x);
        const int y = static_cast<int>(e.tfinger.y);
        for(const auto& [rect, act] : m_touchAreas) {
            if(x >= rect.x && x < rect.x + rect.w &&
               y >= rect.y && y < rect.y + rect.h) {
                publish(act, e.type == SDL_FINGERDOWN);
            }
        }
        break;
    }
#endif
    default:
        break;
    }
}

} // namespace Promethean
