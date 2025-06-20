#include "ui/Button.h"
#include "assets/AssetManager.h"
#include "core/EventBus.h"
#include <gtest/gtest.h>

static SDL_Event MouseMotion(int x,int y)
{
    SDL_Event e{};
    e.type = SDL_MOUSEMOTION;
    e.motion.x = x;
    e.motion.y = y;
    e.button.x = x;
    e.button.y = y;
    return e;
}

static SDL_Event MouseDown(int x,int y)
{
    SDL_Event e{};
    e.type = SDL_MOUSEBUTTONDOWN;
    e.button.button = SDL_BUTTON_LEFT;
    e.button.x = x;
    e.button.y = y;
    return e;
}

static SDL_Event MouseUp(int x,int y)
{
    SDL_Event e{};
    e.type = SDL_MOUSEBUTTONUP;
    e.button.button = SDL_BUTTON_LEFT;
    e.button.x = x;
    e.button.y = y;
    return e;
}

TEST(Button, NormalToHover)
{
    AssetManager mgr{4};
    Button btn(mgr,"b","n","h","p");
    btn.HandleEvent(MouseMotion(10,10));
    EXPECT_EQ(btn.GetState(), ButtonState::HOVER);
}

TEST(Button, HoverToNormal)
{
    AssetManager mgr{4};
    Button btn(mgr,"b","n","h","p");
    btn.HandleEvent(MouseMotion(5,5));
    btn.HandleEvent(MouseMotion(150,150));
    EXPECT_EQ(btn.GetState(), ButtonState::NORMAL);
}

TEST(Button, PressedState)
{
    AssetManager mgr{4};
    Button btn(mgr,"b","n","h","p");
    btn.HandleEvent(MouseMotion(5,5));
    btn.HandleEvent(MouseDown(5,5));
    EXPECT_EQ(btn.GetState(), ButtonState::PRESSED);
}

TEST(Button, ClickFiresEvent)
{
    AssetManager mgr{4};
    Button btn(mgr,"b","n","h","p");
    int count=0;
    auto id = EventBus::Instance().Subscribe<ButtonClickedEvent>([&](const std::any&){ ++count; });
    btn.HandleEvent(MouseMotion(5,5));
    btn.HandleEvent(MouseDown(5,5));
    btn.HandleEvent(MouseUp(5,5));
    EventBus::Instance().Unsubscribe(id);
    EXPECT_EQ(count,1);
    EXPECT_EQ(btn.GetState(), ButtonState::HOVER);
}

TEST(Button, NoEventOnDragOut)
{
    AssetManager mgr{4};
    Button btn(mgr,"b","n","h","p");
    int count=0;
    auto id = EventBus::Instance().Subscribe<ButtonClickedEvent>([&](const std::any&){ ++count; });
    btn.HandleEvent(MouseMotion(5,5));
    btn.HandleEvent(MouseDown(5,5));
    btn.HandleEvent(MouseMotion(200,200));
    btn.HandleEvent(MouseUp(200,200));
    EventBus::Instance().Unsubscribe(id);
    EXPECT_EQ(count,0);
    EXPECT_EQ(btn.GetState(), ButtonState::NORMAL);
}

