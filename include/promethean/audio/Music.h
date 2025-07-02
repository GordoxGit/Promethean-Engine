#pragma once
#include <SDL_mixer.h>

namespace Promethean {

class Music {
public:
    explicit Music(Mix_Music* music = nullptr) : m_music(music) {}
    ~Music() { if(m_music) Mix_FreeMusic(m_music); }
    Music(const Music&) = delete;
    Music& operator=(const Music&) = delete;
    Music(Music&& other) noexcept : m_music(other.m_music) { other.m_music=nullptr; }
    Music& operator=(Music&& other) noexcept { if(this!=&other){ if(m_music) Mix_FreeMusic(m_music); m_music=other.m_music; other.m_music=nullptr;} return *this; }
    [[nodiscard]] Mix_Music* Get() const { return m_music; }
private:
    Mix_Music* m_music;
};

}
