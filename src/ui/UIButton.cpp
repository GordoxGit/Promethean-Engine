#include "ui/UIButton.h"
#include "renderer/BatchRenderer.h"

UIButton::UIButton(std::string id)
    : m_id(std::move(id)) {}

void UIButton::Draw(BatchRenderer& renderer)
{
    (void)renderer;
    renderer.DrawQuad({static_cast<float>(m_rect.x), static_cast<float>(m_rect.y)},
                      {static_cast<float>(m_rect.w), static_cast<float>(m_rect.h)});
}

static bool PointInRect(int x, int y, const SDL_Rect& r)
{
    return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h;
}

bool UIButton::HandleEvent(const SDL_Event& event)
{
    switch(event.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT &&
               PointInRect(event.button.x, event.button.y, m_rect))
            {
                m_pressed = true;
                return true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(m_pressed && event.button.button == SDL_BUTTON_LEFT)
            {
                m_pressed = false;
                if(PointInRect(event.button.x, event.button.y, m_rect))
                {
                    if(m_onClick) m_onClick();
                    return true;
                }
            }
            break;
        default:
            break;
    }
    return false;
}

void UIButton::SetPosition(const glm::vec2& pos)
{
    m_rect.x = static_cast<int>(pos.x);
    m_rect.y = static_cast<int>(pos.y);
}

void UIButton::SetSize(const glm::vec2& size)
{
    m_rect.w = static_cast<int>(size.x);
    m_rect.h = static_cast<int>(size.y);
}
