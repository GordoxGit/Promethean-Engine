#include "ecs/BehaviorSystem.h"
#include "ecs/Registry.h"
#include <gtest/gtest.h>

using namespace pe::ecs;

TEST(BehaviorSystem, IdleSwitchesToSeek)
{
    Registry reg;
    auto e = reg.create();
    auto& pos = reg.add<Position>(e.id());
    auto& beh = reg.add<BehaviorComponent>(e.id());
    beh.idleDuration = 0.f;
    beh.target = {1.f, 0.f};
    BehaviorSystem sys(reg);
    sys.Update(0.1f);
    EXPECT_EQ(beh.state, BehaviorState::Seek);
}

TEST(BehaviorSystem, SeekMovesAndReturnsIdle)
{
    Registry reg;
    auto e = reg.create();
    auto& pos = reg.add<Position>(e.id());
    auto& beh = reg.add<BehaviorComponent>(e.id());
    beh.idleDuration = 0.f;
    beh.target = {1.f, 0.f};
    BehaviorSystem sys(reg);
    sys.Update(0.f); // switch to Seek immediately
    sys.Update(1.f); // move toward target
    sys.Update(0.f); // evaluate arrival
    EXPECT_NEAR(pos.x, 1.f, 0.05f);
    EXPECT_EQ(beh.state, BehaviorState::Idle);
}
