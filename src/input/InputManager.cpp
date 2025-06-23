#include "input/InputManager.h"

namespace Promethean {

InputManager::InputManager() : m_mapper(std::make_unique<ActionMapper>()) {}

void InputManager::HandleEvent(const SDL_Event& e)
{
    switch(e.type)
    {
    case SDL_KEYDOWN:
    case SDL_KEYUP:
        if(e.key.keysym.scancode < SDL_NUM_SCANCODES)
            m_keyState[e.key.keysym.scancode] = (e.type == SDL_KEYDOWN);
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        if(e.button.button < m_mouseButtons.size())
            m_mouseButtons[e.button.button] = (e.type == SDL_MOUSEBUTTONDOWN);
        m_pointerDown = (e.type == SDL_MOUSEBUTTONDOWN);
        m_pointerX = static_cast<float>(e.button.x);
        m_pointerY = static_cast<float>(e.button.y);
        break;
    case SDL_MOUSEMOTION:
        m_pointerX = static_cast<float>(e.motion.x);
        m_pointerY = static_cast<float>(e.motion.y);
        break;
    case SDL_FINGERDOWN:
    case SDL_FINGERUP:
        m_pointerDown = (e.type == SDL_FINGERDOWN);
        m_pointerX = e.tfinger.x;
        m_pointerY = e.tfinger.y;
        break;
    default:
        break;
    }

    if(m_mapper)
        m_mapper->handleEvent(e);
}

void InputManager::Update() {}

bool InputManager::IsKeyPressed(int keycode) const
{
    if(keycode < SDL_NUM_SCANCODES)
        return m_keyState[keycode];
    return false;
}

bool InputManager::IsMouseButtonPressed(int button) const
{
    if(button < static_cast<int>(m_mouseButtons.size()))
        return m_mouseButtons[button];
    return false;
}

void InputManager::GetPointerPosition(float& x, float& y) const
{
    x = m_pointerX;
    y = m_pointerY;
}

bool InputManager::IsActionPressed(PlayerAction action) const
{
    return m_mapper ? m_mapper->isActionPressed(action) : false;
}

ActionMapper& InputManager::GetMapper()
{
    if(!m_mapper)
        m_mapper = std::make_unique<ActionMapper>();
    return *m_mapper;
}

} // namespace Promethean
