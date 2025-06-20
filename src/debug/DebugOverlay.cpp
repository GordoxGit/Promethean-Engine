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
    m_lines.push_back({a, b, duration});
}

void DebugOverlay::AddCircle(const glm::vec2& center, float radius, float duration)
{
    m_circles.push_back({center, radius, duration});
}

void DebugOverlay::AddBox(const glm::vec2& min, const glm::vec2& max, float duration)
{
    m_boxes.push_back({min, max, duration});
}

void DebugOverlay::Update(float dt)
{
    auto prune = [dt](auto& vec) {
        for (auto& p : vec) p.duration -= dt;
        vec.erase(std::remove_if(vec.begin(), vec.end(),
                 [](auto const& p){ return p.duration <= 0.f; }),
                 vec.end());
    };
    prune(m_lines);
    prune(m_circles);
    prune(m_boxes);
}

void DebugOverlay::Render(BatchRenderer& renderer)
{
    // Exécute quand même le rendu même si aucun shader n'est actif (tests headless)
    renderer.Begin(1, 1);

    const auto white = glm::vec4(1.f);

    // Lignes simples
    for (const auto& l : m_lines)
        renderer.DrawLine(l.start, l.end, white);

    // Contours des boîtes
    for (const auto& b : m_boxes)
    {
        renderer.DrawLine({b.min.x, b.min.y}, {b.max.x, b.min.y}, white);
        renderer.DrawLine({b.max.x, b.min.y}, {b.max.x, b.max.y}, white);
        renderer.DrawLine({b.max.x, b.max.y}, {b.min.x, b.max.y}, white);
        renderer.DrawLine({b.min.x, b.max.y}, {b.min.x, b.min.y}, white);
    }

    // Contours des cercles
    constexpr int SEG = 16;
    const float step = 2.f * 3.14159265f / SEG;
    for (const auto& c : m_circles)
        for (int i = 0; i < SEG; ++i)
        {
            float a0 = step * i;
            float a1 = step * (i + 1);
            glm::vec2 p0 = c.center + c.radius * glm::vec2(std::cos(a0), std::sin(a0));
            glm::vec2 p1 = c.center + c.radius * glm::vec2(std::cos(a1), std::sin(a1));
            renderer.DrawLine(p0, p1, white);
        }

    renderer.Flush();
}

#ifdef TESTING
void DebugOverlay::Reset()
{
    m_lines.clear();
    m_circles.clear();
    m_boxes.clear();
}
#endif
