#include "renderer/BatchRenderer.h"
#include "renderer/RenderEvents.h"

#include <glm/gtc/matrix_transform.hpp>

#if defined(HEADLESS_GL)
// → exécution de tests sans contexte : stubs inoffensifs
#  include "renderer/headless_gl_stub.h"
#elif defined(GRAPHICS_API_GLES)
#  include <GLES3/gl3.h>
#  include <GLES3/gl3ext.h>
#else
#  include <GL/glew.h>
#endif

#ifndef GLSL_VERSION
#  if defined(GRAPHICS_API_GLES)
#    define GLSL_VERSION "#version 300 es\n"
#  else
#    define GLSL_VERSION "#version 330 core\n"
#  endif
#endif

#ifdef PROMETHEAN_DEBUG
#  define GL_CHECK(call)                                                     \
    do {                                                                     \
        call;                                                                \
        GLenum _err = glGetError();                                          \
        if (_err != GL_NO_ERROR)                                             \
            LogSystem::Instance().Warn("OpenGL error {} at " #call, (int)_err); \
    } while (0)
#else
#  define GL_CHECK(call) call
#endif

namespace {

const char* vertexSrc =
    GLSL_VERSION
    "layout(location=0) in vec2 aPos;\n"
    "uniform mat4 u_proj;\n"
    "void main(){\n"
    "    gl_Position = u_proj * vec4(aPos, 0.0, 1.0);\n"
    "}\n";

const char* fragmentSrc =
    GLSL_VERSION
#if defined(GRAPHICS_API_GLES)
    "precision mediump float;\n"
#endif
    "uniform vec4 u_color;\n"
    "out vec4 FragColor;\n"
    "void main(){\n"
#if defined(GRAPHICS_API_GLES)
    "    FragColor = u_color;\n"
#else
    "    FragColor = u_color;\n"
#endif
    "}\n";
} // namespace

// ------------------------------------------------------------------
// ctor / dtor
// ------------------------------------------------------------------
BatchRenderer::BatchRenderer() = default;
BatchRenderer::~BatchRenderer() { Shutdown(); }

// ------------------------------------------------------------------
// Init / Shutdown (sécurisés)
// ------------------------------------------------------------------
bool BatchRenderer::Init()
{
    if (m_initialised)
        return false;

    // ----------------- VAO / VBO -----------------
    GL_CHECK(glGenVertexArrays(1, &m_vao));
    GL_CHECK(glGenBuffers(1, &m_vbo));

    GL_CHECK(glBindVertexArray(m_vao));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6, nullptr, GL_DYNAMIC_DRAW));

    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(
        0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0)));

    // ----------------- Shaders -----------------
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    GL_CHECK(glShaderSource(vert, 1, &vertexSrc, nullptr));
    GL_CHECK(glCompileShader(vert));

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    GL_CHECK(glShaderSource(frag, 1, &fragmentSrc, nullptr));
    GL_CHECK(glCompileShader(frag));

    m_shader = glCreateProgram();
    GL_CHECK(glAttachShader(m_shader, vert));
    GL_CHECK(glAttachShader(m_shader, frag));
    GL_CHECK(glLinkProgram(m_shader));

    GL_CHECK(glDeleteShader(vert));
    GL_CHECK(glDeleteShader(frag));

    m_initialised = true;
    return true;
}

void BatchRenderer::Shutdown()
{
    if (!m_initialised)
        return;

    GL_CHECK(glDeleteBuffers(1, &m_vbo));
    GL_CHECK(glDeleteVertexArrays(1, &m_vao));
    GL_CHECK(glDeleteProgram(m_shader));

    m_vao = m_vbo = m_shader = 0;
    m_initialised = false;
}

// ------------------------------------------------------------------
// Frame management
// ------------------------------------------------------------------
void BatchRenderer::Begin(int screenWidth, int screenHeight)
{
    if (!m_initialised)
    {
        LogSystem::Instance().Error("BatchRenderer::Begin called before Init()");
        return;
    }

    GL_CHECK(glViewport(0, 0, screenWidth, screenHeight));

    glm::mat4 proj = glm::ortho(0.f, static_cast<float>(screenWidth),
                                static_cast<float>(screenHeight), 0.f);

    GL_CHECK(glUseProgram(m_shader));
    GLint loc = glGetUniformLocation(m_shader, "u_proj");
    GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, &proj[0][0]));
}

void BatchRenderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                             uint32_t /*textureId*/, const glm::vec4& color)
{
    if (!m_initialised)
    {
        LogSystem::Instance().Error("DrawQuad() called before Init()");
        return;
    }

    float x = pos.x, y = pos.y, w = size.x, h = size.y;
    const float verts[24] = {
        x,     y,     0.f, 0.f,
        x + w, y,     1.f, 0.f,
        x + w, y + h, 1.f, 1.f,
        x,     y,     0.f, 0.f,
        x + w, y + h, 1.f, 1.f,
        x,     y + h, 0.f, 1.f};

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
    if (!m_initialised)
    {
        LogSystem::Instance().Error("Flush() called before Init()");
        return;
    }

    EventBus::Instance().Publish(FrameRenderedEvent{});
}

void BatchRenderer::End() { Flush(); }
