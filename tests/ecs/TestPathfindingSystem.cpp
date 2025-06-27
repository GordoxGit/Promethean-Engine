#include "ecs/PathfindingSystem.h"
#include "ecs/Registry.h"
#include <gtest/gtest.h>

using namespace pe::ecs;
using namespace pe::pathfinding;

TEST(PathfindingSystem, MoveAlongPath)
{
    Grid grid(1, std::vector<int>{0,0,0});
    Registry reg;
    auto e = reg.create();
    auto& nav = reg.add<NavComponent>(e.id());
    nav.position = {0,0};
    nav.destination = {2,0};
    PathfindingSystem sys(reg, &grid);
    sys.Update(0.f); // compute path and move to first step
    EXPECT_EQ(nav.position, glm::ivec2(1,0));
    sys.Update(0.f);
    EXPECT_EQ(nav.position, glm::ivec2(2,0));
    sys.Update(0.f);
    EXPECT_EQ(nav.position, glm::ivec2(2,0));
}
