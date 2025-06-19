#include "renderer/BatchRenderer.h"
#include "renderer/RenderEvents.h"

#include <glm/gtc/matrix_transform.hpp>

#ifndef TESTING
#  ifdef GRAPHICS_API_GLES
#    include <GLES2/gl2.h>
#  else
#    include <GL/gl.h>
#  endif
#endif

#ifndef GLSL_VERSION
#ifdef GRAPHICS_API_GLES
#define GLSL_VERSION "#version 100\n"
#else
#define GLSL_VERSION "#version 330 core\n"
#endif
#endif

#ifdef PROMETHEAN_DEBUG
#define GL_CHECK(x) do { x; GLenum e = glGetError(); \
    if(e!=GL_NO_ERROR) LogSystem::Instance().Warn("GL error {} at " #x, (int)e); } while(0)
#else
#define GL_CHECK(x) x
#endif

#ifdef TESTING
// Minimal stubs of GL when testing without a real context
using GLenum = unsigned int;
using GLuint = unsigned int;
using GLint = int;
using GLsizei = int;
using GLfloat = float;
using GLsizeiptr = long long;
using GLintptr = long long;
using GLboolean = unsigned char;
#ifndef GL_NO_ERROR
#define GL_NO_ERROR 0
#endif
#define GL_ARRAY_BUFFER 0
#define GL_DYNAMIC_DRAW 0
#define GL_FLOAT 0
#define GL_FALSE 0
#define GL_VERTEX_SHADER 0
#define GL_FRAGMENT_SHADER 0
#define GL_COMPILE_STATUS 0
#define GL_LINK_STATUS 0
#define GL_TRIANGLES 0
using GLsizeiptr = long long;
using GLintptr = long long;
using GLboolean = unsigned char;

static void glGenVertexArrays(GLsizei n, GLuint* arr){ for(int i=0;i<n;++i) arr[i]=1+i; }
static void glGenBuffers(GLsizei n, GLuint* arr){ for(int i=0;i<n;++i) arr[i]=1+i; }
static GLuint glCreateShader(GLenum){ return 1; }
static void glShaderSource(GLuint, GLsizei, const char* const*, const GLint*){}
static void glCompileShader(GLuint){}
static void glGetShaderiv(GLuint, GLenum, GLint* param){ *param=1; }
static GLuint glCreateProgram(){ return 1; }
static void glAttachShader(GLuint, GLuint){}
static void glLinkProgram(GLuint){}
static void glGetProgramiv(GLuint, GLenum, GLint* param){ *param=1; }
static void glDeleteShader(GLuint){}
static void glDeleteProgram(GLuint){}
static void glBindVertexArray(GLuint){}
static void glBindBuffer(GLenum, GLuint){}
static void glBufferData(GLenum, GLsizeiptr, const void*, GLenum){}
static void glBufferSubData(GLenum, GLintptr, GLsizeiptr, const void*){}
static void glEnableVertexAttribArray(GLuint){}
static void glVertexAttribPointer(GLuint, GLint, GLenum, GLboolean, GLsizei, const void*){}
static void glUseProgram(GLuint){}
static GLint glGetUniformLocation(GLuint, const char*){ return 0; }
static void glUniformMatrix4fv(GLint, GLsizei, GLboolean, const GLfloat*){}
static void glUniform4fv(GLint, GLsizei, const GLfloat*){}
static void glViewport(GLint, GLint, GLsizei, GLsizei){}
static void glDrawArrays(GLenum, GLint, GLsizei){}
static void glDeleteVertexArrays(GLsizei, const GLuint*){}
static void glDeleteBuffers(GLsizei, const GLuint*){}
static GLenum glGetError(){ return 0; }
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
#ifdef GRAPHICS_API_GLES
    "precision mediump float;\n"
#endif
    "uniform vec4 u_color;\n"
    "void main(){\n"
    "    gl_FragColor = u_color;\n"
    "}\n";
}

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
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(float)*4*6, nullptr, GL_DYNAMIC_DRAW));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0));

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

    LogSystem::Instance().Debug("BatchRenderer Begin");
    GL_CHECK(glViewport(0, 0, screenWidth, screenHeight));
    glm::mat4 proj = glm::ortho(0.f, (float)screenWidth, (float)screenHeight, 0.f);
    GL_CHECK(glUseProgram(m_shader));
    GLint loc = glGetUniformLocation(m_shader, "u_proj");
    GL_CHECK(glUniformMatrix4fv(loc, 1, GL_FALSE, &proj[0][0]));
}

void BatchRenderer::DrawQuad(const glm::vec2& pos, const glm::vec2& size,
                             uint32_t /*textureId*/, const glm::vec4& color)
{
    if (!m_initialized)
    {
        LogSystem::Instance().Error("DrawQuad called before Init");
        return;
    }

    LogSystem::Instance().Debug("BatchRenderer DrawQuad");
    float x = pos.x; float y = pos.y; float w=size.x; float h=size.y;
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

    LogSystem::Instance().Debug("BatchRenderer Flush");
    EventBus::Instance().Publish(FrameRenderedEvent{});
}

void BatchRenderer::End()
{
    Flush();
}

