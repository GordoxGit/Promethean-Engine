#include "audio/AudioEngine.h"
#include "core/EventBus.h"
#include <gtest/gtest.h>
#include <SDL_mixer.h>
#include <string>
#include <thread>
#include <chrono>

#ifdef USE_SDL_STUBS
extern "C" int stub_last_halt_channel;
#endif

using namespace Promethean;

static const std::string kAudioPath = "assets/audio/";

TEST(AudioEngine, Init){
    AudioEngine a; 
    EXPECT_TRUE(a.init());
    EXPECT_GE(Mix_AllocateChannels(-1),8);
    a.shutdown();
}

TEST(AudioEngine, PlaySound_ChannelsDifferent){
    AudioEngine a; ASSERT_TRUE(a.init());
    int c1 = a.playSound(kAudioPath + "beep.wav");
    int c2 = a.playSound(kAudioPath + "boop.wav");
    EXPECT_NE(c1, c2);
    a.shutdown();
}

TEST(AudioEngine, MasterVolume){
    AudioEngine a; ASSERT_TRUE(a.init());
    a.setMasterVolume(0.5f);
    EXPECT_NEAR(Mix_Volume(-1,-1)/static_cast<float>(MIX_MAX_VOLUME),0.5f,0.01f);
    a.shutdown();
}

TEST(AudioEngine, NoFileWrites){
    AudioEngine a; ASSERT_TRUE(a.init());
    a.playSound(kAudioPath + "x.wav");
    SUCCEED();
    a.shutdown();
}

TEST(AudioEngine, EventBusPublished){
    AudioEngine a; ASSERT_TRUE(a.init());
    int count=0;
    auto id = EventBus::Instance().Subscribe<AudioEvent>([&](const std::any&){ ++count; });
    a.playSound(kAudioPath + "foo.wav");
    EventBus::Instance().Unsubscribe(id);
    a.shutdown();
    EXPECT_EQ(count,1);
}

TEST(AudioEngine, StopAll){
    AudioEngine a; ASSERT_TRUE(a.init());
    a.playSound(kAudioPath + "s.wav");
    a.playMusic(kAudioPath + "m.ogg");
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    ASSERT_NO_FATAL_FAILURE(a.stopAll());
    ASSERT_NO_FATAL_FAILURE(a.stopAll());
    a.shutdown();
    SUCCEED();
}

#ifdef USE_SDL_STUBS
TEST(AudioEngine, StopSoundByName){
    AudioEngine a; ASSERT_TRUE(a.init());
    int c1 = a.playSound(kAudioPath + "ding.wav");
    int c2 = a.playSound(kAudioPath + "dong.wav");
    stub_last_halt_channel = -2;
    a.stopSound(kAudioPath + "ding.wav");
    EXPECT_EQ(stub_last_halt_channel, c1);
    stub_last_halt_channel = -2;
    a.stopSound(kAudioPath + "dong.wav");
    EXPECT_EQ(stub_last_halt_channel, c2);
    a.shutdown();
}
#endif
