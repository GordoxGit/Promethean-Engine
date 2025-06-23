#include "input/ActionMapper.h"
#include "core/EventBus.h"
#include <gtest/gtest.h>
#include <malloc.h>

using Promethean::ActionMapper;

static SDL_Event KeyDown(SDL_Scancode sc)
{
    SDL_Event e{}; e.type = SDL_KEYDOWN; e.key.keysym.scancode = sc; return e;
}
static SDL_Event KeyUp(SDL_Scancode sc)
{
    SDL_Event e{}; e.type = SDL_KEYUP; e.key.keysym.scancode = sc; return e;
}

#if defined(__ANDROID__)
static SDL_Event FingerDown(int x,int y)
{
    SDL_Event e{}; e.type=SDL_FINGERDOWN; e.tfinger.x=x; e.tfinger.y=y; return e;
}
#endif

TEST(ActionMapper, MapKeyTriggersAction)
{
    ActionMapper m; m.mapKey(SDL_SCANCODE_SPACE, PlayerAction::Jump);
    m.handleEvent(KeyDown(SDL_SCANCODE_SPACE));
    EXPECT_TRUE(m.isActionPressed(PlayerAction::Jump));
}

TEST(ActionMapper, ActionRelease)
{
    ActionMapper m; m.mapKey(SDL_SCANCODE_SPACE, PlayerAction::Jump);
    m.handleEvent(KeyDown(SDL_SCANCODE_SPACE));
    m.handleEvent(KeyUp(SDL_SCANCODE_SPACE));
    EXPECT_FALSE(m.isActionPressed(PlayerAction::Jump));
}

#if defined(__ANDROID__)
TEST(ActionMapper, TouchArea)
{
    ActionMapper m; SDL_Rect r{0,0,100,100}; m.mapTouchArea(r, PlayerAction::Confirm);
    m.handleEvent(FingerDown(50,50));
    EXPECT_TRUE(m.isActionPressed(PlayerAction::Confirm));
}
#endif

TEST(ActionMapper, NoAllocations)
{
    ActionMapper m; m.mapKey(SDL_SCANCODE_A, PlayerAction::MoveLeft);
    m.handleEvent(KeyDown(SDL_SCANCODE_A)); // warm-up
    auto before = mallinfo();
    m.handleEvent(KeyDown(SDL_SCANCODE_A));
    auto after = mallinfo();
    EXPECT_EQ(after.uordblks, before.uordblks);
}

TEST(ActionMapper, EventBusPublication)
{
    ActionMapper m; m.mapKey(SDL_SCANCODE_B, PlayerAction::MoveRight);
    int count=0; auto id = EventBus::Instance().Subscribe<ActionStateChangedEvent>([&](const std::any&){ ++count; });
    m.handleEvent(KeyDown(SDL_SCANCODE_B));
    m.handleEvent(KeyUp(SDL_SCANCODE_B));
    EventBus::Instance().Unsubscribe(id);
    EXPECT_EQ(count,2);
}
