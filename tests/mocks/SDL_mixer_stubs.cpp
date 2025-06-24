#include <SDL_mixer.h>

// Stubs for SDL_mixer functions used in tests. They avoid touching real audio
// devices or files and provide deterministic behavior across platforms.

extern "C" int stub_last_halt_channel = -2;

namespace {

static int dummy_channels = 8;
static int last_channel = 0;
static int master_volume = MIX_MAX_VOLUME;
static int music_volume = MIX_MAX_VOLUME;

extern "C" int Mix_OpenAudio(int, Uint16, int, int) { return 0; }
extern "C" void Mix_CloseAudio() {}

extern "C" int Mix_AllocateChannels(int n) {
    if (n != -1) dummy_channels = n;
    return dummy_channels;
}

extern "C" Mix_Chunk* Mix_LoadWAV(const char*) {
    static int fake_chunk;
    return reinterpret_cast<Mix_Chunk*>(&fake_chunk);
}

extern "C" Mix_Music* Mix_LoadMUS(const char*) {
    static int fake_music;
    return reinterpret_cast<Mix_Music*>(&fake_music);
}

extern "C" int Mix_PlayChannel(int, Mix_Chunk*, int) { return last_channel++; }
extern "C" int Mix_PlayMusic(Mix_Music*, int) { return 0; }
extern "C" int Mix_FadeInMusic(Mix_Music*, int, int) { return 0; }

extern "C" int Mix_HaltChannel(int c) { stub_last_halt_channel = c; return 0; }
extern "C" int Mix_HaltMusic() { return 0; }

extern "C" void Mix_PauseMusic() {}
extern "C" void Mix_ResumeMusic() {}

extern "C" int Mix_Volume(int, int v) {
    if (v != -1) master_volume = v;
    return master_volume;
}

extern "C" int Mix_VolumeMusic(int v) {
    if (v != -1) music_volume = v;
    return music_volume;
}

extern "C" void Mix_FreeChunk(Mix_Chunk*) {}
extern "C" void Mix_FreeMusic(Mix_Music*) {}

extern "C" const char* Mix_GetError() { return ""; }

} // namespace
