#include <SDL_mixer.h>

// Stubs for SDL_mixer functions used in tests. They avoid touching real audio
// devices or files and provide deterministic behavior across platforms.

extern "C" int stub_last_halt_channel = -2;
extern "C" int stub_fadeout_music_calls = 0;
extern "C" int stub_fadein_music_ms = 0;

namespace {

struct DummyChunk { int id; };
struct DummyMusic { int id; };

static int dummy_channels = 8;
static int last_channel   = 0;
static int master_volume  = MIX_MAX_VOLUME;
static int music_volume   = MIX_MAX_VOLUME;
static int next_chunk_id  = 0;
static int next_music_id  = 0;

extern "C" int Mix_OpenAudio(int, Uint16, int, int) { return 0; }
extern "C" void Mix_CloseAudio() {}

extern "C" int Mix_AllocateChannels(int n) {
    if (n != -1) dummy_channels = n;
    return dummy_channels;
}

extern "C" Mix_Chunk* Mix_LoadWAV_RW(SDL_RWops*, int) {
    return reinterpret_cast<Mix_Chunk*>(new DummyChunk{next_chunk_id++});
}

extern "C" Mix_Chunk* Mix_LoadWAV(const char*) { return Mix_LoadWAV_RW(nullptr, 0); }

extern "C" Mix_Music* Mix_LoadMUS(const char*) {
    return reinterpret_cast<Mix_Music*>(new DummyMusic{next_music_id++});
}

extern "C" int Mix_PlayChannel(int, Mix_Chunk*, int) { return last_channel++; }
extern "C" int Mix_PlayMusic(Mix_Music*, int) { return 0; }
extern "C" int Mix_FadeInMusic(Mix_Music*, int, int ms) { stub_fadein_music_ms = ms; return 0; }
static void (*hook_finished)(void) = nullptr;
extern "C" void Mix_HookMusicFinished(void (*cb)(void)) { hook_finished = cb; }
extern "C" int Mix_FadeOutMusic(int) { ++stub_fadeout_music_calls; if(hook_finished) hook_finished(); return 1; }
extern "C" int Mix_FadeOutChannel(int, int) { return 0; }
extern "C" int Mix_FadingMusic() { return MIX_NO_FADING; }

extern "C" int Mix_HaltChannel(int c) {
    stub_last_halt_channel = c;
    return 0;
}

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

extern "C" void Mix_FreeChunk(Mix_Chunk* c) {
    delete reinterpret_cast<DummyChunk*>(c);
}

extern "C" void Mix_FreeMusic(Mix_Music* m) {
    delete reinterpret_cast<DummyMusic*>(m);
}

extern "C" const char* Mix_GetError() { return "SDL_mixer stub"; }

} // namespace
