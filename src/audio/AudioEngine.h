#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "core/EventBus.h"
#include "SDL_mixer.h"
#include <array>
#include <vector>

namespace Promethean {

struct AudioEvent {
    enum class Type { PlaySFX, PlayMusic, PauseMusic, StopAll };
    Type        type;
    std::string asset;
    float       volume;
};

/** Event published when a fade starts. */
struct AudioFadeStartedEvent {
    int durationMs;
};

/** Event published when a fade operation completes. */
struct AudioFadeCompletedEvent {};

enum class AudioBus { BGM, SFX, UI };

struct BusState {
    int volume{MIX_MAX_VOLUME};
    bool muted{false};
    std::vector<int> channels;
};

class AudioEngine {
public:
    static AudioEngine& Instance();
    bool  init();
    void  shutdown();

    int   playSound(const std::string& name, float volume = 1.0f, AudioBus bus = AudioBus::SFX);
    int   playMusic(const std::string& name, bool loop = true, float fadeInMs = 0.0f, AudioBus bus = AudioBus::BGM);
    bool  playMusicCrossFade(const std::string& path, int ms = 1000, bool loop = true);
    void  pauseMusic();
    void  resumeMusic();
    void  stopSound(const std::string& name);
    bool  fadeOutAll(int ms = 500);
    bool  fadeOutBus(AudioBus bus, int ms);
    void  stopAll();

    void  setMasterVolume(float volume);
    float getMasterVolume() const;
    void  setBusVolume(AudioBus bus, int vol);
    int   getBusVolume(AudioBus bus) const;
    void  muteBus(AudioBus bus, bool state);
    bool  isBusMuted(AudioBus bus) const;

private:
    using ChunkPtr = std::unique_ptr<Mix_Chunk, decltype(&Mix_FreeChunk)>;
    using MusicPtr = std::unique_ptr<Mix_Music, decltype(&Mix_FreeMusic)>;
    std::unordered_map<std::string, ChunkPtr> m_sounds;
    std::unordered_map<std::string, MusicPtr> m_music;
    std::unordered_map<int, std::string>       m_playingChannels;
    float m_masterVolume = 1.0f;
    bool  m_initialized  = false;

    std::string m_pendingMusic;
    bool        m_pendingLoop{true};
    int         m_pendingFadeMs{0};

    std::array<BusState,3> m_buses{};

    static void ChannelFinishedHook(int channel);

    static AudioEngine* s_instance;
    static void MusicFinishedHook();
};

}
