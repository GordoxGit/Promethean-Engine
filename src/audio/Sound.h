#pragma once
#include <SDL_mixer.h>

namespace Promethean {

class Sound {
public:
    explicit Sound(Mix_Chunk* chunk = nullptr) : m_chunk(chunk) {}
    ~Sound() { if(m_chunk) Mix_FreeChunk(m_chunk); }
    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;
    Sound(Sound&& other) noexcept : m_chunk(other.m_chunk) { other.m_chunk=nullptr; }
    Sound& operator=(Sound&& other) noexcept { if(this!=&other){ if(m_chunk) Mix_FreeChunk(m_chunk); m_chunk=other.m_chunk; other.m_chunk=nullptr;} return *this; }
    [[nodiscard]] Mix_Chunk* Get() const { return m_chunk; }
private:
    Mix_Chunk* m_chunk;
};

}
