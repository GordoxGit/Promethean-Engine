#pragma once

#include "debug/DebugPrimitives.h"
#include <vector>
#include <glm/vec2.hpp>

class BatchRenderer;

/**
 * @brief Overlay storing debug primitives for rendering.
 */
class DebugOverlay {
public:
    /// Singleton accessor
    static DebugOverlay& Get();

    /** Add a line lasting for \p duration seconds. */
    void AddLine(const glm::vec2& a, const glm::vec2& b, float duration);
    /** Add a circle lasting for \p duration seconds. */
    void AddCircle(const glm::vec2& center, float radius, float duration);
    /** Add an axis-aligned box lasting for \p duration seconds. */
    void AddBox(const glm::vec2& min, const glm::vec2& max, float duration);

    /** Update primitives lifetimes and remove expired ones. */
    void Update(float deltaTime);
    /** Render all primitives using the provided renderer. */
    void Render(BatchRenderer& renderer);

#ifdef TESTING
    size_t GetLineCount() const { return m_lines.size(); }
    size_t GetCircleCount() const { return m_circles.size(); }
    size_t GetBoxCount() const { return m_boxes.size(); }
    void Reset();
#endif

private:
    DebugOverlay() = default;

    std::vector<DebugLine>   m_lines;
    std::vector<DebugCircle> m_circles;
    std::vector<DebugBox>    m_boxes;
};

