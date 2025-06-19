#pragma once

#include "core/EventBus.h"
#include "core/LogSystem.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <cstdint>

/**
 * @brief Minimal 2D sprite/quad renderer (first iteration).
 *
 * *Not* yet batching multiple quads; each DrawQuad issues its own draw call.
 */
class BatchRenderer
{
public:
    BatchRenderer();
    ~BatchRenderer();

    bool Init();                 ///< Compile shaders, create VAO/VBO (returns success)
    void Shutdown();             ///< Delete GL resources

    void Begin(int screenWidth, int screenHeight);
    void DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                  uint32_t textureId = 0,
                  const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});
    void Flush();                ///< glDrawArrays, publish FrameRenderedEvent
    void End();                  ///< Just calls Flush() for now

private:
    uint32_t m_vao{0}, m_vbo{0}, m_shader{0};
    bool m_initialized{false};
#ifdef TESTING
public:
    uint32_t GetVao() const { return m_vao; }
    uint32_t GetVbo() const { return m_vbo; }
    uint32_t GetShader() const { return m_shader; }
#endif
};

