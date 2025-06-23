#include "ui/Slider.h"
#include "core/EventBus.h"
#include <gtest/gtest.h>

static SDL_Event MouseMotion(int x, int y)
{
    SDL_Event e{};
    e.type = SDL_MOUSEMOTION;
    e.motion.x = x;
    e.motion.y = y;
    e.button.x = x;
    e.button.y = y;
    return e;
}

static SDL_Event MouseDown(int x, int y)
{
    SDL_Event e{};
    e.type = SDL_MOUSEBUTTONDOWN;
    e.button.button = SDL_BUTTON_LEFT;
    e.button.x = x;
    e.button.y = y;
    return e;
}

static SDL_Event MouseUp(int x, int y)
{
    SDL_Event e{};
    e.type = SDL_MOUSEBUTTONUP;
    e.button.button = SDL_BUTTON_LEFT;
    e.button.x = x;
    e.button.y = y;
    return e;
}

static SDL_Event KeyDown(SDL_Keycode key)
{
    SDL_Event e{};
    e.type = SDL_KEYDOWN;
    e.key.keysym.sym = key;
    return e;
}

TEST(Slider, InitialValue)
{
    Slider s("s","t","k",0.7f);
    EXPECT_NEAR(s.GetValue(), 0.7f, 0.001f);
}

TEST(Slider, DragUpdatesValue)
{
    Slider s("s","t","k",0.f);
    s.SetPosition({0.f,0.f});
    s.SetSize({100.f,10.f});
    auto knob = s.GetKnobRect();
    s.HandleEvent(MouseDown(knob.x+5, knob.y+5));
    s.HandleEvent(MouseMotion(100, knob.y+5));
    s.HandleEvent(MouseUp(100, knob.y+5));
    EXPECT_NEAR(s.GetValue(), 1.f, 0.05f);
}

TEST(Slider, EventFired)
{
    Slider s("s","t","k",0.f);
    s.SetPosition({0.f,0.f});
    s.SetSize({100.f,10.f});
    int count = 0;
    auto id = EventBus::Instance().Subscribe<SliderValueChangedEvent>([&](const std::any&){ ++count; });
    s.SetValue(0.5f);
    s.SetValue(0.505f);
    s.SetValue(0.6f);
    EventBus::Instance().Unsubscribe(id);
    EXPECT_EQ(count,2);
}

TEST(Slider, KeyboardIncrement)
{
    Slider s("s","t","k",0.f);
    s.SetPosition({0.f,0.f});
    s.SetSize({100.f,10.f});
    for(int i=0;i<10;i++)
        s.HandleEvent(KeyDown(SDLK_RIGHT));
    EXPECT_NEAR(s.GetValue(), 0.2f, 0.02f);
}

TEST(Slider, BoundsClamping)
{
    Slider s("s","t","k",0.5f);
    s.SetValue(-1.f);
    EXPECT_FLOAT_EQ(s.GetValue(), 0.f);
    s.SetValue(2.f);
    EXPECT_FLOAT_EQ(s.GetValue(), 1.f);
}

