#include "ui/Button.h"

#include "renderer/BatchRenderer.h"
#include <cassert>

Button::Button(AssetManager& assets,
               const std::string& id,
               const std::string& textureNormal,
               const std::string& textureHover,
               const std::string& texturePressed)
    : m_assets(assets)
    , m_id(id)
{
    m_texNormal  = m_assets.LoadTexture(textureNormal);
    m_texHover   = m_assets.LoadTexture(textureHover);
    m_texPressed = m_assets.LoadTexture(texturePressed);

    m_state = State::NORMAL;
}

void Button::Draw(BatchRenderer& renderer)
{
    (void)renderer;
    // Rendering not implemented yet with new texture system.
    renderer.DrawQuad({static_cast<float>(m_bounds.x), static_cast<float>(m_bounds.y)},
                      {static_cast<float>(m_bounds.w), static_cast<float>(m_bounds.h)});
}

bool Button::HandleEvent(const SDL_Event& event)
{
    const bool inside =
        event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP ?
        (event.button.x >= m_bounds.x && event.button.x < m_bounds.x + m_bounds.w &&
         event.button.y >= m_bounds.y && event.button.y < m_bounds.y + m_bounds.h) : false;

    switch(event.type)
    {
        case SDL_MOUSEMOTION:
            if(inside && m_state == State::NORMAL)
            {
                m_state = State::HOVER;
                LogSystem::Instance().Debug("Button {} state -> HOVER", m_id);
            }
            else if(!inside && m_state != State::PRESSED)
            {
                if(m_state != State::NORMAL)
                {
                    m_state = State::NORMAL;
                    LogSystem::Instance().Debug("Button {} state -> NORMAL", m_id);
                }
            }
            break;
        case SDL_MOUSEBUTTONDOWN:
            if(inside && event.button.button == SDL_BUTTON_LEFT)
            {
                m_state = State::PRESSED;
                LogSystem::Instance().Debug("Button {} state -> PRESSED", m_id);
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(m_state == State::PRESSED)
            {
                if(inside && event.button.button == SDL_BUTTON_LEFT)
                {
                    m_state = State::HOVER;
                    LogSystem::Instance().Debug("Button {} clicked", m_id);
                    EventBus::Instance().Publish(ButtonClickedEvent{m_id});
                }
                else
                {
                    m_state = State::NORMAL;
                    LogSystem::Instance().Debug("Button {} state -> NORMAL", m_id);
                }
            }
            break;
        default:
            break;
    }
    return true;
}

void Button::SetPosition(const glm::vec2& pos)
{
    m_bounds.x = static_cast<int>(pos.x);
    m_bounds.y = static_cast<int>(pos.y);
}

void Button::SetSize(const glm::vec2& size)
{
    m_bounds.w = static_cast<int>(size.x);
    m_bounds.h = static_cast<int>(size.y);
}

