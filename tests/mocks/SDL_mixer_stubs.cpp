#include <SDL_mixer.h>

// Stubs for SDL_mixer functions used in tests. They avoid touching real audio
// devices or files and provide deterministic behavior across platforms.

extern "C" {

static int dummy_channels = 8;
static int last_channel = 0;
int stub_last_halt_channel = -2;
static int master_volume = MIX_MAX_VOLUME;
static int music_volume = MIX_MAX_VOLUME;

int Mix_OpenAudio(int, Uint16, int, int) { return 0; }
void Mix_CloseAudio() {}

int Mix_AllocateChannels(int n) {
    if (n != -1) dummy_channels = n;
    return dummy_channels;
}

Mix_Chunk* Mix_LoadWAV(const char*) {
    static int fake_chunk;
    return reinterpret_cast<Mix_Chunk*>(&fake_chunk);
}

Mix_Music* Mix_LoadMUS(const char*) {
    static int fake_music;
    return reinterpret_cast<Mix_Music*>(&fake_music);
}

int Mix_PlayChannel(int, Mix_Chunk*, int) { return last_channel++; }
int Mix_PlayMusic(Mix_Music*, int) { return 0; }
int Mix_FadeInMusic(Mix_Music*, int, int) { return 0; }

int Mix_HaltChannel(int c) { stub_last_halt_channel = c; return 0; }
int Mix_HaltMusic() { return 0; }

void Mix_PauseMusic() {}
void Mix_ResumeMusic() {}

int Mix_Volume(int, int v) {
    if (v != -1) master_volume = v;
    return master_volume;
}

int Mix_VolumeMusic(int v) {
    if (v != -1) music_volume = v;
    return music_volume;
}

void Mix_FreeChunk(Mix_Chunk*) {}
void Mix_FreeMusic(Mix_Music*) {}

const char* Mix_GetError() { return ""; }

} // extern "C"
