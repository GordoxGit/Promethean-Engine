#include "debug/DebugOverlay.h"
#include "renderer/BatchRenderer.h"
#include <algorithm>
#include <cmath>
#include <cassert>

DebugOverlay& DebugOverlay::Get()
{
    static DebugOverlay instance;
    return instance;
}

void DebugOverlay::AddLine(const glm::vec2& a, const glm::vec2& b, float duration)
{
    m_lines.push_back({a,b,duration});
}

void DebugOverlay::AddCircle(const glm::vec2& center, float radius, float duration)
{
    m_circles.push_back({center,radius,duration});
}

void DebugOverlay::AddBox(const glm::vec2& min, const glm::vec2& max, float duration)
{
    m_boxes.push_back({min,max,duration});
}

void DebugOverlay::Update(float dt)
{
    auto update = [dt](auto& vec)
    {
        for(auto& p : vec) p.duration -= dt;
        vec.erase(std::remove_if(vec.begin(), vec.end(),
            [](const auto& p){ return p.duration <= 0.f; }), vec.end());
    };
    update(m_lines);
    update(m_circles);
    update(m_boxes);
}

void DebugOverlay::Render(BatchRenderer& renderer)
{
#ifdef TESTING
    assert(renderer.GetShader() != 0 && "Renderer not initialized");
#endif
    const glm::vec4 white(1.f,1.f,1.f,1.f);
    for(const auto& l : m_lines)
        renderer.DrawLine(l.start, l.end, white);

    for(const auto& b : m_boxes)
    {
        renderer.DrawLine({b.min.x,b.min.y}, {b.max.x,b.min.y}, white);
        renderer.DrawLine({b.max.x,b.min.y}, {b.max.x,b.max.y}, white);
        renderer.DrawLine({b.max.x,b.max.y}, {b.min.x,b.max.y}, white);
        renderer.DrawLine({b.min.x,b.max.y}, {b.min.x,b.min.y}, white);
    }

    const int SEG = 16;
    const float step = 2.f * 3.14159265f / SEG;
    for(const auto& c : m_circles)
    {
        for(int i=0;i<SEG;i++)
        {
            float a0 = step * i;
            float a1 = step * (i+1);
            glm::vec2 p0 = c.center + c.radius * glm::vec2(std::cos(a0), std::sin(a0));
            glm::vec2 p1 = c.center + c.radius * glm::vec2(std::cos(a1), std::sin(a1));
            renderer.DrawLine(p0, p1, white);
        }
    }
}

#ifdef TESTING
void DebugOverlay::Reset()
{
    m_lines.clear();
    m_circles.clear();
    m_boxes.clear();
}
#endif

