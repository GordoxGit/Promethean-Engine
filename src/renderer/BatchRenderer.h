#pragma once

#include "core/EventBus.h"
#include "core/LogSystem.h"
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <cstdint>

/**
 * @brief Minimal 2D sprite/quad renderer (first iteration).
 *
 * En mode test (HEADLESS_GL) aucune API OpenGL n’est appelée ; le
 * composant se contente alors de valider la logique haut-niveau
 * (initialisation, cycle Begin/End, publication d’évènements, …).
 */
/**
 * @brief UV coordinates for a quad.
 */
struct QuadUV {
    glm::vec2 bottomLeft  {0.f, 1.f};
    glm::vec2 topLeft     {0.f, 0.f};
    glm::vec2 topRight    {1.f, 0.f};
    glm::vec2 bottomRight {1.f, 1.f};
};

class BatchRenderer
{
public:
    BatchRenderer();
    ~BatchRenderer();

    /** Compile les shaders et crée le VAO/VBO. */
    bool Init();

    /** Libère toutes les ressources GPU. */
    void Shutdown();

    /** Prépare la frame (setup viewport + projection). */
    void Begin(int screenWidth, int screenHeight);

    /**
     * @brief Lie une texture 2D pour les prochains draw.
     * @param textureId Identifiant OpenGL de la texture.
     */
    void BindTexture(uint32_t textureId);

    /** Dessine un quad immédiatement (pas encore de batching). */
    void DrawQuad(const glm::vec2& pos,
                  const glm::vec2& size,
                  const QuadUV&    uv   = {},
                  const glm::vec4& tint = {1.f,1.f,1.f,1.f});

    /** Draw a colored line from A to B. */
    void DrawLine(const glm::vec2& a,
                  const glm::vec2& b,
                  const glm::vec4& color = {1.f,1.f,1.f,1.f});

    /** Flush immédiat puis publication de FrameRenderedEvent. */
    void Flush();

    /** Actuellement équivalent à Flush(). */
    void End();

private:
    uint32_t m_vao{0}, m_vbo{0}, m_shader{0};
    uint32_t m_boundTexture{0};
    bool     m_initialized{false};

#ifdef TESTING
public:
    uint32_t GetVao()    const { return m_vao; }
    uint32_t GetVbo()    const { return m_vbo; }
    uint32_t GetShader() const { return m_shader; }
#endif
};

#ifdef TESTING
namespace TestGL {
    int      GetBindTextureCount();
    uint32_t GetLastBoundTexture();
    const float* GetLastBufferData();
    glm::vec4 GetLastTint();
    int      GetLastUniformTexture();
    int      GetDrawArraysCount();
}
#endif
