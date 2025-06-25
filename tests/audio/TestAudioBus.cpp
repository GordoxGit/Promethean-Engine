#include "audio/AudioEngine.h"
#include <gtest/gtest.h>
#ifdef USE_SDL_STUBS
extern "C" int stub_music_volume;
extern "C" int stub_master_volume;
extern "C" int stub_last_volume_channel;
extern std::vector<int> stub_fadeout_channels;
#endif

using namespace Promethean;

static const std::string kAudioPath = "assets/audio/";

TEST(AudioBus, DefaultState) {
    AudioEngine a; ASSERT_TRUE(a.init());
    EXPECT_EQ(a.getBusVolume(AudioBus::SFX), MIX_MAX_VOLUME);
    EXPECT_FALSE(a.isBusMuted(AudioBus::BGM));
    a.shutdown();
}

TEST(AudioBus, SetGetVolume) {
    AudioEngine a; ASSERT_TRUE(a.init());
    a.setBusVolume(AudioBus::SFX, 64);
    EXPECT_EQ(a.getBusVolume(AudioBus::SFX), 64);
    a.shutdown();
}

TEST(AudioBus, MuteState) {
    AudioEngine a; ASSERT_TRUE(a.init());
    a.muteBus(AudioBus::UI, true);
    EXPECT_TRUE(a.isBusMuted(AudioBus::UI));
    a.shutdown();
}

TEST(AudioBus, FadeOutEmptyBus) {
    AudioEngine a; ASSERT_TRUE(a.init());
    EXPECT_TRUE(a.fadeOutBus(AudioBus::SFX, 50));
    a.shutdown();
}

#ifdef USE_SDL_STUBS
TEST(AudioBus, MuteMusicUsesSDL) {
    AudioEngine a; ASSERT_TRUE(a.init());
    a.muteBus(AudioBus::BGM, true);
    EXPECT_EQ(stub_music_volume, 0);
    a.shutdown();
}

TEST(AudioBus, PlaySoundUsesBusVolume) {
    AudioEngine a; ASSERT_TRUE(a.init());
    a.setBusVolume(AudioBus::SFX, 32);
    stub_last_volume_channel = -2;
    a.playSound(kAudioPath + "beep.wav", 1.0f, AudioBus::SFX);
    EXPECT_NE(stub_last_volume_channel, -2);
    EXPECT_EQ(stub_master_volume, 32);
    a.shutdown();
}

TEST(AudioBus, FadeOutMusicBus) {
    AudioEngine a; ASSERT_TRUE(a.init());
    a.playMusic(kAudioPath + "beep.wav");
    extern int stub_fadeout_music_calls;
    stub_fadeout_music_calls = 0;
    EXPECT_TRUE(a.fadeOutBus(AudioBus::BGM, 200));
    EXPECT_EQ(stub_fadeout_music_calls,1);
    a.shutdown();
}

TEST(AudioBus, FadeOutSfxChannels) {
    AudioEngine a; ASSERT_TRUE(a.init());
    int c1 = a.playSound(kAudioPath + "beep.wav");
    int c2 = a.playSound(kAudioPath + "boop.wav");
    stub_fadeout_channels.clear();
    EXPECT_TRUE(a.fadeOutBus(AudioBus::SFX, 100));
    ASSERT_EQ(stub_fadeout_channels.size(), 2u);
    EXPECT_EQ(stub_fadeout_channels[0], c1);
    EXPECT_EQ(stub_fadeout_channels[1], c2);
    a.shutdown();
}
#endif

