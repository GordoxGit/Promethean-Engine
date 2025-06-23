#include "ui/Slider.h"
#include "renderer/BatchRenderer.h"
#include <algorithm>
#include <cmath>

Slider::Slider(const std::string& id,
               const std::string& trackTex,
               const std::string& knobTex,
               float initialValue)
    : m_id(id)
{
    (void)trackTex;
    (void)knobTex;
    m_value = std::clamp(initialValue, 0.f, 1.f);
}

void Slider::SetPosition(const glm::vec2& pos)
{
    m_trackRect.x = static_cast<int>(pos.x);
    m_trackRect.y = static_cast<int>(pos.y);
    m_knobRect.y  = m_trackRect.y;
    m_knobRect.x  = m_trackRect.x + static_cast<int>((m_trackRect.w - m_knobRect.w) * m_value);
}

void Slider::SetSize(const glm::vec2& size)
{
    m_trackRect.w = static_cast<int>(size.x);
    m_trackRect.h = static_cast<int>(size.y);
    m_knobRect.w  = m_trackRect.h;
    m_knobRect.h  = m_trackRect.h;
    m_knobRect.x  = m_trackRect.x + static_cast<int>((m_trackRect.w - m_knobRect.w) * m_value);
    m_knobRect.y  = m_trackRect.y;
}

void Slider::Draw(BatchRenderer& renderer)
{
    renderer.DrawQuad({static_cast<float>(m_trackRect.x), static_cast<float>(m_trackRect.y)},
                      {static_cast<float>(m_trackRect.w), static_cast<float>(m_trackRect.h)});
    renderer.DrawQuad({static_cast<float>(m_knobRect.x), static_cast<float>(m_knobRect.y)},
                      {static_cast<float>(m_knobRect.w), static_cast<float>(m_knobRect.h)});
}

static bool PointInRect(int x, int y, const SDL_Rect& r)
{
    return x >= r.x && x < r.x + r.w && y >= r.y && y < r.y + r.h;
}

void Slider::UpdateFromPosition(int x)
{
    int left  = m_trackRect.x;
    int right = m_trackRect.x + m_trackRect.w - m_knobRect.w;
    int clamped = std::clamp(x - m_knobRect.w / 2, left, right);
    float value = 0.f;
    if (right != left)
        value = static_cast<float>(clamped - left) / static_cast<float>(right - left);
    SetValue(value);
}

bool Slider::HandleEvent(const SDL_Event& event)
{
    switch(event.type)
    {
        case SDL_MOUSEBUTTONDOWN:
            if(event.button.button == SDL_BUTTON_LEFT &&
               PointInRect(event.button.x, event.button.y, m_knobRect))
            {
                m_dragging = true;
                return true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if(event.button.button == SDL_BUTTON_LEFT && m_dragging)
            {
                m_dragging = false;
                return true;
            }
            break;
        case SDL_MOUSEMOTION:
            if(m_dragging)
            {
                UpdateFromPosition(event.motion.x);
                return true;
            }
            break;
        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_RIGHT)
            {
                SetValue(m_value + 0.02f);
                return true;
            }
            else if(event.key.keysym.sym == SDLK_LEFT)
            {
                SetValue(m_value - 0.02f);
                return true;
            }
            break;
        default:
            break;
    }
    return false;
}

void Slider::SetValue(float value)
{
    value = std::clamp(value, 0.f, 1.f);
    if(std::fabs(value - m_value) < 0.01f)
        return;
    m_value = value;
    m_knobRect.x = m_trackRect.x + static_cast<int>((m_trackRect.w - m_knobRect.w) * m_value);
    SliderValueChangedEvent ev{m_id, m_value};
    EventBus::Instance().Publish(ev);
}

float Slider::GetValue() const
{
    return m_value;
}

