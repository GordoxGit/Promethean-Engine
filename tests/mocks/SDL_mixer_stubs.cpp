#include <SDL_mixer.h>
#include <vector>

// Stubs for SDL_mixer functions used in tests. They avoid touching real audio
// devices or files and provide deterministic behavior across platforms.

extern "C" int stub_last_halt_channel = -2;
extern "C" int stub_fadeout_music_calls = 0;
extern "C" int stub_fadein_music_ms = 0;
extern "C" int stub_last_volume_channel = -2;
extern "C" int stub_master_volume = MIX_MAX_VOLUME;
extern "C" int stub_music_volume = MIX_MAX_VOLUME;
std::vector<int> stub_fadeout_channels;
extern "C" void (*stub_channel_finished_cb)(int) = nullptr;

namespace {

struct DummyChunk { int id; };
struct DummyMusic { int id; };

static int dummy_channels = 8;
static int last_channel   = 0;

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
extern "C" int Mix_FadeOutChannel(int ch, int) { stub_fadeout_channels.push_back(ch); return 0; }
extern "C" int Mix_FadingMusic() { return MIX_NO_FADING; }
extern "C" void Mix_ChannelFinished(void (*cb)(int)) { stub_channel_finished_cb = cb; }

extern "C" int Mix_HaltChannel(int c) {
    stub_last_halt_channel = c;
    if(stub_channel_finished_cb) stub_channel_finished_cb(c);
    return 0;
}

extern "C" int Mix_HaltMusic() { return 0; }

extern "C" void Mix_PauseMusic() {}
extern "C" void Mix_ResumeMusic() {}

extern "C" int Mix_Volume(int channel, int v) {
    stub_last_volume_channel = channel;
    if (v != -1) stub_master_volume = v;
    return stub_master_volume;
}

extern "C" int Mix_VolumeMusic(int v) {
    if (v != -1) stub_music_volume = v;
    return stub_music_volume;
}

extern "C" void Mix_FreeChunk(Mix_Chunk* c) {
    delete reinterpret_cast<DummyChunk*>(c);
}

extern "C" void Mix_FreeMusic(Mix_Music* m) {
    delete reinterpret_cast<DummyMusic*>(m);
}

extern "C" const char* Mix_GetError() { return "SDL_mixer stub"; }

} // namespace
