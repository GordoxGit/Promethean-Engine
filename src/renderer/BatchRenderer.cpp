#include "renderer/BatchRenderer.h"
#include "renderer/RenderEvents.h"

#include <glm/gtc/matrix_transform.hpp>

/* -------------------------------------------------------------------------
   Sélection de l’en-tête OpenGL en fonction de la plate-forme / du mode
   - GRAPHICS_API_GLES : build Android → <GLES3/*>
   - HEADLESS_GL       : build “stub” (tests unitaires) → aucun header GL réel
   - sinon             : build desktop normal        → <GL/glew.h>
   ------------------------------------------------------------------------- */
#if defined(GRAPHICS_API_GLES)
    #include <GLES3/gl3.h>
    #include <GLES3/gl3ext.h>
#elif defined(HEADLESS_GL)
    /* Pas d’include : on va fournir nos propres stubs plus bas */
#else
    #include <GL/glew.h>
#endif

/* ------------------------------------------------------------------------- */
#ifndef GLSL_VERSION
#   ifdef GRAPHICS_API_GLES
#       define GLSL_VERSION "#version 300 es\n"
#   else
#       define GLSL_VERSION "#version 330 core\n"
#   endif
#endif

#ifdef PROMETHEAN_DEBUG
#   define GL_CHECK(x)                                                        \
        do {                                                                  \
            x;                                                                \
            GLenum e = glGetError();                                          \
            if (e != GL_NO_ERROR)                                             \
                LogSystem::Instance().Warn("GL error {} at " #x, (int)e);     \
        } while (0)
#else
#   define GL_CHECK(x) x
#endif

/* ------------------------------------------------------------------------- */
/* ---- Implémentations GL factices utilisées en mode HEADLESS_GL ------------*/
#if defined(HEADLESS_GL) || defined(TESTING)

using GLenum      = unsigned int;
using GLuint      = unsigned int;
using GLint       = int;
using GLsizei     = int;
using GLfloat     = float;
using GLsizeiptr  = long long;
using GLintptr    = long long;
using GLboolean   = unsigned char;

#ifndef GL_NO_ERROR
#   define GL_NO_ERROR 0
#endif
#define GL_ARRAY_BUFFER    0
#define GL_DYNAMIC_DRAW    0
#define GL_FLOAT           0
#define GL_FALSE           0
#define GL_VERTEX_SHADER   0
#define GL_FRAGMENT_SHADER 0
#define GL_COMPILE_STATUS  0
#define GL_LINK_STATUS     0
#define GL_TRIANGLES       0

/* Petit helper pour générer les stubs en une ligne. Les fonctions réelles
   ne sont jamais appelées dans les tests, donc on retourne des valeurs neutres. */
#define GL_STUB(ret, name, ...)                               \
    extern "C" ret name(__VA_ARGS__) { return {}; }

/* Génération des stubs minimalistes */
GL_STUB(void,   glGenVertexArrays, GLsizei, GLuint*)
GL_STUB(void,   glGenBuffers,      GLsizei, GLuint*)
GL_STUB(GLuint, glCreateShader,    GLenum)
GL_STUB(void,   glShaderSource,    GLuint, GLsizei, const char* const*, const GLint*)
GL_STUB(void,   glCompileShader,   GLuint)
GL_STUB(void,   glGetShaderiv,     GLuint, GLenum, GLint*)
GL_STUB(GLuint, glCreateProgram,   void)
GL_STUB(void,   glAttachShader,    GLuint, GLuint)
GL_STUB(void,   glLinkProgram,     GLuint)
GL_STUB(void,   glGetProgramiv,    GLuint, GLenum, GLint*)
GL_STUB(void,   glDeleteShader,    GLuint)
GL_STUB(void,   glDeleteProgram,   GLuint)
GL_STUB(void,   glBindVertexArray, GLuint)
GL_STUB(void,   glBindBuffer,      GLenum, GLuint)
GL_STUB(void,   glBufferData,      GLenum, GLsizeiptr, const void*, GLenum)
GL_STUB(void,   glBufferSubData,   GLenum, GLintptr, GLsizeiptr, const void*)
GL_STUB(void,   glEnableVertexAttribArray, GLuint)
GL_STUB(void,   glVertexAttribPointer, GLuint, GLint, GLenum, GLboolean, GLsizei, const void*)
GL_STUB(void,   glUseProgram, GLuint)
GL_STUB(GLint,  glGetUniformLocation, GLuint, const char*)
GL_STUB(void,   glUniformMatrix4fv, GLint, GLsizei, GLboolean, const GLfloat*)
GL_STUB(void,   glUniform4fv, GLint, GLsizei, const GLfloat*)
GL_STUB(void,   glViewport, GLint, GLint, GLsizei, GLsizei)
GL_STUB(void,   glDrawArrays, GLenum, GLint, GLsizei)
GL_STUB(void,   glDeleteVertexArrays, GLsizei, const GLuint*)
GL_STUB(void,   glDeleteBuffers,     GLsizei, const GLuint*)
GL_STUB(GLenum, glGetError,          void)

#endif /* HEADLESS_GL || TESTING */
/* ------------------------------------------------------------------------- */

namespace
{
    const char* vertexSrc =
        GLSL_VERSION
        "layout(location=0) in vec2 aPos;\n"
        "uniform mat4 u_proj;\n"
        "void main(){\n"
        "    gl_Position = u_proj * vec4(aPos, 0.0, 1.0);\n"
        "}\n";

    const char* fragmentSrc =
        GLSL_VERSION
    #ifdef GRAPHICS_API_GLES
        "precision mediump float;\n"
    #endif
        "uniform vec4 u_color;\n"
        "void main(){\n"
    #ifdef GRAPHICS_API_GLES
        "    gl_FragColor = u_color;\n"
    #else
        "    out vec4 FragColor;\n"
        "    FragColor = u_color;\n"
    #endif
        "}\n";
}

/* ===================================================================== */
/*                          Implémentation                               */
/* ===================================================================== */
BatchRenderer::BatchRenderer() = default;
BatchRenderer::~BatchRenderer() { Shutdown(); }

bool BatchRenderer::Init()
{
    if (m_initialized)
        return true;

    GL_CHECK(glGenVertexArrays(1, &m_vao));
    GL_CHECK(glGenBuffers(1, &m_vbo));
    GL_CHECK(glBindVertexArray(m_vao));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6,
                          nullptr, GL_DYNAMIC_DRAW));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
                                   4 * sizeof(float), (void*)0));

    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    GL_CHECK(glShaderSource(vert, 1, &vertexSrc, nullptr));
    GL_CHECK(glCompileShader(vert));
    GLint status;
    GL_CHECK(glGetShaderiv(vert, GL_COMPILE_STATUS, &status));

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    GL_CHECK(glShaderSource(frag, 1, &fragmentSrc, nullptr));
    GL_CHECK(glCompileShader(frag));
    GL_CHECK(glGetShaderiv(frag, GL_COMPILE_STATUS, &status));

    m_shader = glCreateProgram();
    GL_CHECK(glAttachShader(m_shader, vert));
    GL_CHECK(glAttachShader(m_shader, frag));
    GL_CHECK(glLinkProgram(m_shader));
    GL_CHECK(glGetProgramiv(m_shader, GL_LINK_STATUS, &status));

    GL_CHECK(glDeleteShader(vert));
    GL_CHECK(glDeleteShader(frag));

    m_initialized = true;
    return true;
}

void BatchRenderer::Shutdown()
{
    if (!m_initialized)
        return;

    GL_CHECK(glDeleteBuffers(1, &m_vbo));
    GL_CHECK(glDeleteVertexArrays(1, &m_vao));
    GL_CHECK(glDeleteProgram(m_shader));

    m_vao = m_vbo = m_shader = 0;
    m_initialized = false;
}

void BatchRenderer::Begin(int screenWidth, int screenHeight)
{
    if (!m_initialized)
    {
        LogSystem::Instance().Error("BatchRenderer::Begin without Init");
        return;
    }

    GL_CHECK(glViewport(0, 0, screenWidth, screenHeight));
    glm::mat4 proj = glm::ortho(0.f, (float)screenWidth,
                                (float)screenHeight, 0.f);
    GL_CHECK(glUseProgram(m_shader));
    GLint loc = glGetUniformLocation(m_shader, "u_proj");
    GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, &proj[0][0]));
}

void BatchRenderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                             uint32_t /*textureId*/,
                             const glm::vec4& color)
{
    if (!m_initialized)
    {
        LogSystem::Instance().Error("DrawQuad called before Init");
        return;
    }

    float x = pos.x; float y = pos.y; float w = size.x; float h = size.y;
    const float verts[24] = {
        x,     y,     0.f, 0.f,
        x+w,   y,     1.f, 0.f,
        x+w,   y+h,   1.f, 1.f,
        x,     y,     0.f, 0.f,
        x+w,   y+h,   1.f, 1.f,
        x,     y+h,   0.f, 1.f
    };

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts));

    GLint loc = glGetUniformLocation(m_shader, "u_color");
    GL_CHECK(glUniform4fv(loc, 1, &color[0]));

    GL_CHECK(glUseProgram(m_shader));
    GL_CHECK(glBindVertexArray(m_vao));
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));
}

void BatchRenderer::Flush()
{
    if (!m_initialized)
    {
        LogSystem::Instance().Error("Flush called before Init");
        return;
    }

    EventBus::Instance().Publish(FrameRenderedEvent{});
}

void BatchRenderer::End()
{
    Flush();
}
