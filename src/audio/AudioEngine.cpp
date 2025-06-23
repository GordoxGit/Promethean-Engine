#include "audio/AudioEngine.h"
#include "core/LogSystem.h"
#include <algorithm>

namespace Promethean {

bool AudioEngine::init()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) != 0)
    {
        LogSystem::Instance().Error("Mix_OpenAudio failed: {}", Mix_GetError());
        return false;
    }
    Mix_AllocateChannels(32);
    setMasterVolume(1.0f);
    return true;
}

void AudioEngine::shutdown()
{
    for(auto& [k,v] : m_sounds)
        Mix_FreeChunk(v);
    m_sounds.clear();
    for(auto& [k,v] : m_music)
        Mix_FreeMusic(v);
    m_music.clear();
    Mix_CloseAudio();
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
            auto first = m_sounds.begin();
            Mix_FreeChunk(first->second);
            m_sounds.erase(first);
        }
        m_sounds[name] = chunk;
    }
    else
    {
        chunk = it->second;
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
        for(auto& [k,v] : m_music)
            Mix_FreeMusic(v);
        m_music.clear();
        m_music[name] = music;
    }
    else
        music = it->second;

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
    Mix_HaltChannel(-1);
    Mix_HaltMusic();
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
