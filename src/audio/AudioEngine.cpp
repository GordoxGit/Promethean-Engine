#include "audio/AudioEngine.h"
#include "core/LogSystem.h"
#include <algorithm>

namespace Promethean {

bool AudioEngine::init()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
    {
        LogSystem::Instance().Error("Mix_OpenAudio failed: {}", Mix_GetError());
        m_initialized = false;
        return false;
    }
    Mix_AllocateChannels(32);
    setMasterVolume(1.0f);
    m_initialized = true;
    return true;
}

void AudioEngine::shutdown()
{
    for (auto& [_, chunk] : m_sounds) {
        Mix_Chunk* ptr = chunk.release();
        if (ptr) Mix_FreeChunk(ptr);
    }
    m_sounds.clear();
    for (auto& [_, mus] : m_music) {
        Mix_Music* ptr = mus.release();
        if (ptr) Mix_FreeMusic(ptr);
    }
    m_music.clear();
    Mix_CloseAudio();
    m_initialized = false;
}

int AudioEngine::playSound(const std::string& name, float volume)
{
    Mix_Chunk* chunk = nullptr;
    auto it = m_sounds.find(name);
    if(it == m_sounds.end())
    {
        chunk = Mix_LoadWAV(name.c_str());
        if(!chunk)
        {
#ifdef TESTING
            static Uint8 dummy[4] = {0};
            chunk = Mix_QuickLoad_RAW(dummy, sizeof(dummy));
#else
            LogSystem::Instance().Warn("Failed to load sound {}: {}", name, Mix_GetError());
            return -1;
#endif
        }
        if(m_sounds.size() >= 32)
        {
            m_sounds.erase(m_sounds.begin());
        }
        m_sounds.emplace(name, ChunkPtr(chunk, Mix_FreeChunk));
    }
    else
    {
        chunk = it->second.get();
    }

    int channel = Mix_PlayChannel(-1, chunk, 0);
    if(channel >= 0)
        Mix_Volume(channel, static_cast<int>(std::clamp(volume*m_masterVolume,0.f,1.f) * MIX_MAX_VOLUME));
    EventBus::Instance().Publish(AudioEvent{AudioEvent::Type::PlaySFX, name, volume});
    return channel;
}

int AudioEngine::playMusic(const std::string& name, bool loop, float fadeInMs)
{
    Mix_Music* music = nullptr;
    auto it = m_music.find(name);
    if(it == m_music.end())
    {
        music = Mix_LoadMUS(name.c_str());
        if(!music)
        {
#ifdef TESTING
            music = reinterpret_cast<Mix_Music*>(1);
#else
            LogSystem::Instance().Warn("Failed to load music {}: {}", name, Mix_GetError());
            return -1;
#endif
        }
        for (auto& [_, mus] : m_music) {
            Mix_Music* ptr = mus.release();
            if (ptr) Mix_FreeMusic(ptr);
        }
        m_music.clear();
        m_music.emplace(name, MusicPtr(music, Mix_FreeMusic));
    }
    else
        music = it->second.get();

    int loops = loop ? -1 : 1;
    int result = (fadeInMs > 0.f) ? Mix_FadeInMusic(music, loops, static_cast<int>(fadeInMs))
                                  : Mix_PlayMusic(music, loops);
    Mix_VolumeMusic(static_cast<int>(m_masterVolume * MIX_MAX_VOLUME));
    if(result == 0)
        EventBus::Instance().Publish(AudioEvent{AudioEvent::Type::PlayMusic, name, 1.0f});
    return result;
}

void AudioEngine::pauseMusic()
{
    Mix_PauseMusic();
    EventBus::Instance().Publish(AudioEvent{AudioEvent::Type::PauseMusic, "", 0.f});
}

void AudioEngine::resumeMusic()
{
    Mix_ResumeMusic();
}

void AudioEngine::stopAll()
{
    if (!m_initialized) return;

    Mix_HaltMusic();
    Mix_HaltChannel(-1);

    auto safeErase = [](auto& map, auto freeFn){
        for (auto it = map.begin(); it != map.end(); ) {
            if (it->second) freeFn(it->second.release());
            it = map.erase(it);
        }
    };
    safeErase(m_music,  Mix_FreeMusic);
    safeErase(m_sounds, Mix_FreeChunk);

    EventBus::Instance().Publish(AudioEvent{AudioEvent::Type::StopAll, "", 0.f});
}

void AudioEngine::setMasterVolume(float volume)
{
    m_masterVolume = std::clamp(volume, 0.f, 1.f);
    int vol = static_cast<int>(m_masterVolume * MIX_MAX_VOLUME);
    Mix_Volume(-1, vol);
    Mix_VolumeMusic(vol);
}

float AudioEngine::getMasterVolume() const
{
    return m_masterVolume;
}

} // namespace Promethean
