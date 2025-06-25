#include "renderer/BatchRenderer.h"
#include "renderer/RenderEvents.h"

#include <glm/gtc/matrix_transform.hpp>
#include <cassert>

/* -------------------------------------------------------------------------
   Sélection de l’en‑tête OpenGL en fonction de la plate‑forme / du mode
   ------------------------------------------------------------------------- */
#if defined(GRAPHICS_API_GLES)
    #include <GLES3/gl3.h>
    #include <GLES3/gl3ext.h>
#elif defined(HEADLESS_GL)
    /* Pas d'en‑tête GL réel en mode headless : des stubs sont définis plus bas. */
#else
    #include <GL/glew.h>
#endif

static int s_drawCalls = 0;

#ifndef GLSL_VERSION
#   ifdef GRAPHICS_API_GLES
#       define GLSL_VERSION "#version 300 es\n"
#   else
#       define GLSL_VERSION "#version 330 core\n"
#   endif
#endif

#ifdef PROMETHEAN_DEBUG
#   define GL_CHECK(x)                                                         \
        do { x; GLenum e = glGetError();                                       \
        if (e != GL_NO_ERROR)                                                  \
            LogSystem::Instance().Warn("GL error {} at " #x, (int)e);        \
        } while (0)
#else
#   define GL_CHECK(x) x
#endif

/* -------------------------------------------------------------------------
   Implémentations GL factices utilisées en mode HEADLESS_GL / TESTING
   ------------------------------------------------------------------------- */
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
#define GL_LINES           0
#define GL_TEXTURE_2D      0

// ----------------------------------------------------------------------
// Helpers pour déclarer rapidement des stubs C extern "C"
//  - GL_STUB_VOID : pour les fonctions retournant void
//  - GL_STUB_VAL  : pour les fonctions retournant une valeur
// ----------------------------------------------------------------------
#define GL_STUB_VOID(name, ...)   extern "C" void   name(__VA_ARGS__) { /* no‑op */ }
#define GL_STUB_VAL(ret, name, ...) extern "C" ret  name(__VA_ARGS__) { return (ret)0; }

// ➜ Stubs "void"
GL_STUB_VOID(glGenVertexArrays,        GLsizei, GLuint*)
GL_STUB_VOID(glGenBuffers,             GLsizei, GLuint*)
GL_STUB_VOID(glShaderSource,           GLuint, GLsizei, const char* const*, const GLint*)
GL_STUB_VOID(glCompileShader,          GLuint)
GL_STUB_VOID(glGetShaderiv,            GLuint, GLenum, GLint*)
GL_STUB_VOID(glAttachShader,           GLuint, GLuint)
GL_STUB_VOID(glLinkProgram,            GLuint)
GL_STUB_VOID(glGetProgramiv,           GLuint, GLenum, GLint*)
GL_STUB_VOID(glDeleteShader,           GLuint)
GL_STUB_VOID(glDeleteProgram,          GLuint)
GL_STUB_VOID(glBindVertexArray,        GLuint)
GL_STUB_VOID(glBindBuffer,             GLenum, GLuint)
GL_STUB_VOID(glBufferData,             GLenum, GLsizeiptr, const void*, GLenum)
GL_STUB_VOID(glEnableVertexAttribArray,GLuint)
GL_STUB_VOID(glVertexAttribPointer,    GLuint, GLint, GLenum, GLboolean, GLsizei, const void*)
GL_STUB_VOID(glUseProgram,             GLuint)
GL_STUB_VOID(glUniformMatrix4fv,       GLint, GLsizei, GLboolean, const GLfloat*)
GL_STUB_VOID(glViewport,               GLint, GLint, GLsizei, GLsizei)
GL_STUB_VOID(glDeleteVertexArrays,     GLsizei, const GLuint*)
GL_STUB_VOID(glDeleteBuffers,          GLsizei, const GLuint*)

// ➜ Stubs "valeur"
GL_STUB_VAL(GLuint,   glCreateShader,      GLenum)
GL_STUB_VAL(GLuint,   glCreateProgram,     void)
GL_STUB_VAL(GLint,    glGetUniformLocation,GLuint, const char*)
GL_STUB_VAL(GLenum,   glGetError,          void)

#ifdef TESTING
namespace TestGL {
    int bindTextureCount = 0;
    GLuint lastBoundTexture = 0;
    int drawArraysCount = 0;
    float lastBufferData[24] = {};
    glm::vec4 lastTint{0.f,0.f,0.f,0.f};
    GLint lastUniformTexture = 0;
}

extern "C" void glBindTexture(GLenum, GLuint texture)
{ TestGL::bindTextureCount++; TestGL::lastBoundTexture = texture; }

extern "C" void glUniform1i(GLint, GLint value)
{ TestGL::lastUniformTexture = value; }

extern "C" void glUniform4fv(GLint, GLsizei, const GLfloat* value)
{ if(value) TestGL::lastTint = {value[0],value[1],value[2],value[3]}; }

extern "C" void glBufferSubData(GLenum, GLintptr, GLsizeiptr size, const void* data)
{
    const float* f = static_cast<const float*>(data);
    for(size_t i=0;i<24 && i<size/sizeof(float);++i)
        TestGL::lastBufferData[i]=f[i];
}

extern "C" void glDrawArrays(GLenum, GLint, GLsizei)
{ TestGL::drawArraysCount++; }
#else
GL_STUB_VOID(glBindTexture, GLenum, GLuint)
GL_STUB_VOID(glUniform1i, GLint, GLint)
GL_STUB_VOID(glUniform4fv, GLint, GLsizei, const GLfloat*)
GL_STUB_VOID(glBufferSubData, GLenum, GLintptr, GLsizeiptr, const void*)
GL_STUB_VOID(glDrawArrays, GLenum, GLint, GLsizei)
#endif

#endif /* HEADLESS_GL || TESTING */

/* ------------------------------------------------------------------------- */
namespace {
    const char* vertexSrc =
        GLSL_VERSION
        "layout(location=0) in vec2 aPos;\n"
        "layout(location=1) in vec2 aUV;\n"
        "out vec2 vUV;\n"
        "uniform mat4 u_proj;\n"
        "void main(){\n"
        "    vUV = aUV;\n"
        "    gl_Position = u_proj * vec4(aPos, 0.0, 1.0);\n"
        "}\n";

    const char* fragmentSrc =
        GLSL_VERSION
    #ifdef GRAPHICS_API_GLES
        "precision mediump float;\n"
    #endif
        "in vec2 vUV;\n"
        "uniform sampler2D u_tex;\n"
        "uniform vec4 u_tint;\n"
    #ifdef GRAPHICS_API_GLES
        "void main(){\n"
        "    gl_FragColor = texture(u_tex, vUV) * u_tint;\n"
        "}\n"
    #else
        "out vec4 FragColor;\n"
        "void main(){\n"
        "    FragColor = texture(u_tex, vUV) * u_tint;\n"
        "}\n"
    #endif
        ;
}

/* =========================== Implémentation ============================ */
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
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 6, nullptr, GL_DYNAMIC_DRAW));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
    GL_CHECK(glEnableVertexAttribArray(1));
    GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));

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

    s_drawCalls = 0;
    GL_CHECK(glViewport(0, 0, screenWidth, screenHeight));
    glm::mat4 proj = glm::ortho(0.f, static_cast<float>(screenWidth),
                                static_cast<float>(screenHeight), 0.f);
    GL_CHECK(glUseProgram(m_shader));
    GLint loc = glGetUniformLocation(m_shader, "u_proj");
    GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, &proj[0][0]));
}

void BatchRenderer::BindTexture(uint32_t textureId)
{
    if (textureId == 0)
    {
        assert(false && "BindTexture called with 0");
        return;
    }

    if (textureId == m_boundTexture)
        return;

    m_boundTexture = textureId;
    LogSystem::Instance().Debug("BatchRenderer::BindTexture {}", textureId);
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, textureId));
}

void BatchRenderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                             const QuadUV& uv, const glm::vec4& tint)
{
    if (!m_initialized)
    {
        LogSystem::Instance().Error("DrawQuad called before Init");
        return;
    }

    float x = pos.x, y = pos.y, w = size.x, h = size.y;
    const float verts[24] = {
        x,   y,   uv.topLeft.x,     uv.topLeft.y,
        x+w, y,   uv.topRight.x,    uv.topRight.y,
        x+w, y+h, uv.bottomRight.x, uv.bottomRight.y,
        x,   y,   uv.topLeft.x,     uv.topLeft.y,
        x+w, y+h, uv.bottomRight.x, uv.bottomRight.y,
        x,   y+h, uv.bottomLeft.x,  uv.bottomLeft.y
    };

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts));

    GLint locTint = glGetUniformLocation(m_shader, "u_tint");
    GL_CHECK(glUniform4fv(locTint, 1, &tint[0]));
    GLint locTex = glGetUniformLocation(m_shader, "u_tex");
    GL_CHECK(glUniform1i(locTex, 0));

    GL_CHECK(glUseProgram(m_shader));
    GL_CHECK(glBindVertexArray(m_vao));
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 6));
    ++s_drawCalls;
}

void BatchRenderer::DrawLine(const glm::vec2& a, const glm::vec2& b,
                             const glm::vec4& color)
{
    if (!m_initialized)
    {
        LogSystem::Instance().Error("DrawLine called before Init");
        return;
    }

    const float verts[8] = {
        a.x, a.y, 0.f, 0.f,
        b.x, b.y, 1.f, 1.f
    };

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts));

    GLint locTint = glGetUniformLocation(m_shader, "u_tint");
    GL_CHECK(glUniform4fv(locTint, 1, &color[0]));
    GLint locTex = glGetUniformLocation(m_shader, "u_tex");
    GL_CHECK(glUniform1i(locTex, 0));

    GL_CHECK(glUseProgram(m_shader));
    GL_CHECK(glBindVertexArray(m_vao));
    GL_CHECK(glDrawArrays(GL_LINES, 0, 2));
    ++s_drawCalls;
}

void BatchRenderer::Flush()
{
    if (!m_initialized)
    {
        LogSystem::Instance().Error("Flush called before Init");
        return;
    }

    LogSystem::Instance().Debug("BatchRenderer::Flush");
    EventBus::Instance().Publish(FrameRenderedEvent{});
}

void BatchRenderer::End()
{
    Flush();
}

int BatchRenderer::GetDrawCallCount()
{
    return s_drawCalls;
}

void BatchRenderer::ResetStats()
{
    s_drawCalls = 0;
}

#ifdef TESTING
namespace TestGL {
int      GetBindTextureCount()    { return bindTextureCount; }
uint32_t GetLastBoundTexture()    { return lastBoundTexture; }
const float* GetLastBufferData()  { return lastBufferData; }
glm::vec4 GetLastTint()           { return lastTint; }
int      GetLastUniformTexture()  { return lastUniformTexture; }
int      GetDrawArraysCount()     { return drawArraysCount; }
}
#endif
