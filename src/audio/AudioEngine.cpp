#include "audio/AudioEngine.h"
#include "core/LogSystem.h"
#include <algorithm>
#include <iterator>

namespace Promethean {

AudioEngine* AudioEngine::s_instance = nullptr;

AudioEngine& AudioEngine::Instance()
{
    static AudioEngine inst;
    return inst;
}

void AudioEngine::MusicFinishedHook()
{
    if(!s_instance) return;
    if(!s_instance->m_pendingMusic.empty()) {
        auto path  = s_instance->m_pendingMusic;
        bool loop  = s_instance->m_pendingLoop;
        int  fade  = s_instance->m_pendingFadeMs;
        s_instance->m_pendingMusic.clear();
        s_instance->playMusic(path, loop, static_cast<float>(fade));
        EventBus::Instance().Publish(AudioFadeCompletedEvent{});
    } else {
        s_instance->stopAll();
        EventBus::Instance().Publish(AudioFadeCompletedEvent{});
    }
}

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
    s_instance = this;
    Mix_ChannelFinished(ChannelFinishedHook);
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
    m_playingChannels.clear();
    Mix_CloseAudio();
    Mix_ChannelFinished(nullptr);
    s_instance = nullptr;
    m_initialized = false;
}

int AudioEngine::playSound(const std::string& name, float volume, AudioBus bus)
{
    if (!m_initialized) return -1;
    Mix_Chunk* chunk = nullptr;
    auto it = m_sounds.find(name);
    if(it == m_sounds.end())
    {
        chunk = Mix_LoadWAV(name.c_str());
        if(!chunk)
        {
            LogSystem::Instance().Warn("Failed to load sound {}: {}", name, Mix_GetError());
            return -1;
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
    {
        auto& b = m_buses[static_cast<size_t>(bus)];
        b.channels.push_back(channel);
        int mixVol = static_cast<int>(std::clamp(volume*m_masterVolume,0.f,1.f) * b.volume);
        if(b.muted) mixVol = 0;
        Mix_Volume(channel, std::clamp(mixVol,0,MIX_MAX_VOLUME));
        m_playingChannels[channel] = name;
    }
    EventBus::Instance().Publish(AudioEvent{AudioEvent::Type::PlaySFX, name, volume});
    return channel;
}

int AudioEngine::playMusic(const std::string& name, bool loop, float fadeInMs, AudioBus bus)
{
    if (!m_initialized) return -1;
    Mix_Music* music = nullptr;
    auto it = m_music.find(name);
    if(it == m_music.end())
    {
        music = Mix_LoadMUS(name.c_str());
        if(!music)
        {
            LogSystem::Instance().Warn("Failed to load music {}: {}", name, Mix_GetError());
            return -1;
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

    auto& b = m_buses[static_cast<size_t>(bus)];
    int loops = loop ? -1 : 1;
    int result = (fadeInMs > 0.f) ? Mix_FadeInMusic(music, loops, static_cast<int>(fadeInMs))
                                  : Mix_PlayMusic(music, loops);
    int mixVol = b.muted ? 0 : static_cast<int>(b.volume * m_masterVolume);
    Mix_VolumeMusic(std::clamp(mixVol,0,MIX_MAX_VOLUME));
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

bool AudioEngine::fadeOutAll(int ms)
{
    if (!m_initialized) return false;

    EventBus::Instance().Publish(AudioFadeStartedEvent{ms});

    s_instance = this;
    m_pendingMusic.clear();
    Mix_HookMusicFinished(MusicFinishedHook);
    Mix_FadeOutChannel(-1, ms);
    Mix_FadeOutMusic(ms);
    return true;
}

bool AudioEngine::fadeOutBus(AudioBus bus, int ms)
{
    if(!m_initialized) return false;
    if(bus == AudioBus::BGM) return Mix_FadeOutMusic(ms) == 1;
    for(int ch : m_buses[static_cast<size_t>(bus)].channels)
        Mix_FadeOutChannel(ch, ms);
    return true;
}

bool AudioEngine::playMusicCrossFade(const std::string& path, int ms, bool loop)
{
    if (!m_initialized) return false;

    if (m_music.empty())
    {
        return playMusic(path, loop, static_cast<float>(ms)) == 0;
    }

    EventBus::Instance().Publish(AudioFadeStartedEvent{ms});
    s_instance    = this;
    m_pendingMusic = path;
    m_pendingLoop  = loop;
    m_pendingFadeMs = ms;
    Mix_HookMusicFinished(MusicFinishedHook);
    Mix_FadeOutMusic(ms);
    return true;
}

void AudioEngine::stopSound(const std::string& name)
{
    if (!m_initialized) return;
    for (auto it = m_playingChannels.begin(); it != m_playingChannels.end(); )
    {
        if (it->second == name)
        {
            Mix_HaltChannel(it->first);
            it = m_playingChannels.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void AudioEngine::stopAll()
{
    if (!m_initialized) return;

    Mix_HaltMusic();
    Mix_HaltChannel(-1);

    m_playingChannels.clear();
    m_sounds.clear();
    m_music.clear();

    EventBus::Instance().Publish(AudioEvent{AudioEvent::Type::StopAll, "", 0.f});
}

void AudioEngine::setMasterVolume(float volume)
{
    m_masterVolume = std::clamp(volume, 0.f, 1.f);
    int vol = static_cast<int>(m_masterVolume * MIX_MAX_VOLUME);
    Mix_Volume(-1, vol);
    Mix_VolumeMusic(vol);
    for(size_t i=0;i<m_buses.size();++i)
        setBusVolume(static_cast<AudioBus>(i), m_buses[i].volume);
}

float AudioEngine::getMasterVolume() const
{
    return m_masterVolume;
}

void AudioEngine::setBusVolume(AudioBus bus, int vol)
{
    auto& b = m_buses[static_cast<size_t>(bus)];
    b.volume = std::clamp(vol, 0, MIX_MAX_VOLUME);
    if(bus == AudioBus::BGM)
        Mix_VolumeMusic(b.muted ? 0 : static_cast<int>(b.volume * m_masterVolume));
    else
        for(int ch : b.channels)
            Mix_Volume(ch, b.muted ? 0 : static_cast<int>(b.volume * m_masterVolume));
}

int AudioEngine::getBusVolume(AudioBus bus) const
{
    return m_buses[static_cast<size_t>(bus)].volume;
}

void AudioEngine::muteBus(AudioBus bus, bool state)
{
    auto& b = m_buses[static_cast<size_t>(bus)];
    b.muted = state;
    setBusVolume(bus, b.volume);
}

bool AudioEngine::isBusMuted(AudioBus bus) const
{
    return m_buses[static_cast<size_t>(bus)].muted;
}

void AudioEngine::ChannelFinishedHook(int channel)
{
    if(!s_instance) return;
    for(auto& b : s_instance->m_buses)
    {
        auto it = std::find(b.channels.begin(), b.channels.end(), channel);
        if(it != b.channels.end()) { b.channels.erase(it); break; }
    }
}

} // namespace Promethean
