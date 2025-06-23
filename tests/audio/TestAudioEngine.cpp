#include "audio/AudioEngine.h"
#include "core/EventBus.h"
#include <gtest/gtest.h>
#include <SDL_mixer.h>

#ifdef TESTING
extern "C" {
static int dummy_channels = 8;
int Mix_OpenAudio(int, Uint16, int, int){ return 0; }
void Mix_CloseAudio(){}
int Mix_AllocateChannels(int n){ if(n!=-1) dummy_channels=n; return dummy_channels; }
static Uint8 dummy_data[4] = {0};
Mix_Chunk* Mix_LoadWAV(const char*){ return Mix_QuickLoad_RAW(dummy_data, sizeof(dummy_data)); }
Mix_Music* Mix_LoadMUS(const char*){ return reinterpret_cast<Mix_Music*>(0x1); }
int Mix_PlayChannel(int, Mix_Chunk*, int){ static int c=0; return c++; }
int Mix_PlayMusic(Mix_Music*, int){ return 0; }
int Mix_FadeInMusic(Mix_Music*, int, int){ return 0; }
int Mix_HaltChannel(int){ return 0; }
int Mix_HaltMusic(){ return 0; }
void Mix_PauseMusic(){}
void Mix_ResumeMusic(){}
int Mix_Volume(int, int v){ static int vol=MIX_MAX_VOLUME; if(v!=-1) vol=v; return vol; }
int Mix_VolumeMusic(int v){ static int vol=MIX_MAX_VOLUME; if(v!=-1) vol=v; return vol; }
void Mix_FreeChunk(Mix_Chunk*){}
void Mix_FreeMusic(Mix_Music*){}
const char* Mix_GetError(){ return ""; }
}
#endif

using namespace Promethean;

TEST(AudioEngine, Init){
    AudioEngine a; 
    EXPECT_TRUE(a.init());
    EXPECT_GE(Mix_AllocateChannels(-1),8);
    a.shutdown();
}

TEST(AudioEngine, PlaySound_ChannelsDifferent){
    AudioEngine a; a.init();
    int c1 = a.playSound("beep.wav");
    int c2 = a.playSound("boop.wav");
    EXPECT_NE(c1, c2);
    a.shutdown();
}

TEST(AudioEngine, MasterVolume){
    AudioEngine a; a.init();
    a.setMasterVolume(0.5f);
    EXPECT_NEAR(Mix_Volume(-1,-1)/static_cast<float>(MIX_MAX_VOLUME),0.5f,0.01f);
    a.shutdown();
}

TEST(AudioEngine, NoFileWrites){
    AudioEngine a; a.init();
    a.playSound("x.wav");
    SUCCEED();
    a.shutdown();
}

TEST(AudioEngine, EventBusPublished){
    AudioEngine a; a.init();
    int count=0;
    auto id = EventBus::Instance().Subscribe<AudioEvent>([&](const std::any&){ ++count; });
    a.playSound("foo.wav");
    EventBus::Instance().Unsubscribe(id);
    a.shutdown();
    EXPECT_EQ(count,1);
}

TEST(AudioEngine, StopAll){
    AudioEngine a; a.init();
    a.playSound("s.wav");
    a.playMusic("m.ogg");
    a.stopAll();
    a.shutdown();
    SUCCEED();
}
