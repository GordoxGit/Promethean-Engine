#pragma once

#include "core/EventBus.h"
#include "core/LogSystem.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <cstdint>

/**
 * @brief Minimal 2D sprite/quad renderer (first iteration).
 *
 * Pour l’instant chaque DrawQuad émet son propre draw call.
 */
class BatchRenderer
{
public:
    BatchRenderer();
    ~BatchRenderer();

    /** Initialise les objets GL (VAO/VBO, shader). Renvoie false si déjà initialisé. */
    bool Init();       

    /** Libère toutes les ressources GL. Sûr même si déjà libéré. */
    void Shutdown();   

    /** Prépare la frame (viewport + matrice de projection). */
    void Begin(int screenWidth, int screenHeight);

    /** Enfile un quad (CPU side pour l’instant) et émet le draw call. */
    void DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                  uint32_t textureId = 0,
                  const glm::vec4& color = {1.f, 1.f, 1.f, 1.f});

    /** Flush + publication d’un évènement FrameRendered. */
    void Flush();

    /** Termine la frame (équivaut à Flush() pour le moment). */
    void End();

    [[nodiscard]] bool IsInitialised() const { return m_initialised; }

#ifdef TESTING
    // Exposition de quelques IDs GL pour les tests unitaires
    uint32_t GetVao()    const { return m_vao;    }
    uint32_t GetVbo()    const { return m_vbo;    }
    uint32_t GetShader() const { return m_shader; }
#endif

private:
    uint32_t m_vao     {0};
    uint32_t m_vbo     {0};
    uint32_t m_shader  {0};
    bool     m_initialised {false};
};
