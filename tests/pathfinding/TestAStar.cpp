#include "pathfinding/AStar.h"
#include <gtest/gtest.h>

using namespace pe::pathfinding;

TEST(AStar, StraightLine)
{
    Grid grid(1, std::vector<int>(5,0));
    auto path = FindPath(grid, {0,0}, {4,0});
    ASSERT_EQ(path.size(), 5u);
    EXPECT_EQ(path.front(), glm::ivec2(0,0));
    EXPECT_EQ(path.back(), glm::ivec2(4,0));
}

TEST(AStar, AvoidObstacle)
{
    Grid grid(3, std::vector<int>(3,0));
    grid[1][1] = 1; // obstacle
    auto path = FindPath(grid, {0,1}, {2,1});
    ASSERT_EQ(path.size(), 5u); // around obstacle
    EXPECT_EQ(path.back(), glm::ivec2(2,1));
}

TEST(AStar, NoPath)
{
    Grid grid(1, std::vector<int>{0,1,0});
    auto path = FindPath(grid, {0,0}, {2,0});
    EXPECT_TRUE(path.empty());
}
