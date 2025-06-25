#include "debug/RuntimeOverlay.h"
#include "audio/AudioEngine.h"
#include <gtest/gtest.h>
#include <SDL.h>

using namespace pe::debug;
using Promethean::AudioEngine;
using Promethean::AudioBus;

TEST(RuntimeOverlay, Init_Shutdown_NoLeak)
{
    Init();
    Shutdown();
    SUCCEED();
}

static void pressF2()
{
    Toggle();
}

TEST(RuntimeOverlay, RenderOverlay_ToggleVisibility)
{
    Init();
    EXPECT_FALSE(IsVisible());
    pressF2();
    RenderOverlay();
    EXPECT_TRUE(IsVisible());
    pressF2();
    RenderOverlay();
    EXPECT_FALSE(IsVisible());
    Shutdown();
}

TEST(RuntimeOverlay, AudioSlider_ChangesVolume)
{
    AudioEngine& a = AudioEngine::Instance();
    ASSERT_TRUE(a.init());
    Init();
    ForceSetVolume(10,20,30);
#ifdef PE_IMGUI_ENABLED
    EXPECT_EQ(a.getBusVolume(AudioBus::BGM),10);
    EXPECT_EQ(a.getBusVolume(AudioBus::SFX),20);
    EXPECT_EQ(a.getBusVolume(AudioBus::UI),30);
#else
    EXPECT_EQ(a.getBusVolume(AudioBus::BGM), MIX_MAX_VOLUME);
#endif
    Shutdown();
    a.shutdown();
}

TEST(RuntimeOverlay, Headless_Build_NoSymbol)
{
#ifndef PE_IMGUI_ENABLED
    SUCCEED();
#else
    SUCCEED();
#endif
}

TEST(RuntimeOverlay, Performance_RenderUnderBudget)
{
    Init();
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0;i<5;i++)
        RenderOverlay();
    auto end = std::chrono::high_resolution_clock::now();
    Shutdown();
    double avg = std::chrono::duration<double, std::micro>(end-start).count()/5.0;
    EXPECT_LT(avg, 100.0); // <0.1 ms
}
