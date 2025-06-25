#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "core/EventBus.h"
#include "SDL_mixer.h"

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

class AudioEngine {
public:
    bool  init();
    void  shutdown();

    int   playSound(const std::string& name, float volume = 1.0f);
    int   playMusic(const std::string& name, bool loop = true, float fadeInMs = 0.0f);
    bool  playMusicCrossFade(const std::string& path, int ms = 1000, bool loop = true);
    void  pauseMusic();
    void  resumeMusic();
    void  stopSound(const std::string& name);
    bool  fadeOutAll(int ms = 500);
    void  stopAll();

    void  setMasterVolume(float volume);
    float getMasterVolume() const;

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

    static AudioEngine* s_instance;
    static void MusicFinishedHook();
};

}
