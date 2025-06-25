#include "ai/Pathfinder.h"
#include "assets/TileMap.h"
#include "physics/CollisionLayer.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>
#include <chrono>

using namespace pe::ai;
using namespace Promethean;

static std::shared_ptr<TileMap> MakeMap(const char* xml)
{
    auto tmp = std::filesystem::temp_directory_path() / "pf.tmx";
    std::ofstream(tmp) << xml;
    return LoadTileMap(tmp.string());
}

static std::shared_ptr<TileMap> StraightMap()
{
    const char* xml = R"(<map width='5' height='1' tilewidth='32' tileheight='32'>
      <tileset firstgid='1' name='dummy' tilewidth='32' tileheight='32' tilecount='1' columns='1'>
        <image source='dummy.png' width='32' height='32'/>
      </tileset>
      <layer name='Layer' width='5' height='1'><data encoding='csv'>0,0,0,0,0</data></layer>
      <objectgroup name='collision'></objectgroup>
    </map>)";
    return MakeMap(xml);
}

static std::shared_ptr<TileMap> ObstacleMap()
{
    const char* xml = R"(<map width='3' height='3' tilewidth='32' tileheight='32'>
      <tileset firstgid='1' name='dummy' tilewidth='32' tileheight='32' tilecount='1' columns='1'>
        <image source='dummy.png' width='32' height='32'/>
      </tileset>
      <layer name='Layer' width='3' height='3'><data encoding='csv'>0,0,0,0,0,0,0,0,0</data></layer>
      <objectgroup name='collision'>
        <object id='1' x='32' y='32' width='32' height='32'/>
      </objectgroup>
    </map>)";
    return MakeMap(xml);
}

static std::shared_ptr<TileMap> BlockedMap()
{
    const char* xml = R"(<map width='3' height='1' tilewidth='32' tileheight='32'>
      <tileset firstgid='1' name='dummy' tilewidth='32' tileheight='32' tilecount='1' columns='1'>
        <image source='dummy.png' width='32' height='32'/>
      </tileset>
      <layer name='Layer' width='3' height='1'><data encoding='csv'>0,0,0</data></layer>
      <objectgroup name='collision'>
        <object id='1' x='32' y='0' width='32' height='32'/>
      </objectgroup>
    </map>)";
    return MakeMap(xml);
}

TEST(Pathfinder, FindPath_StraightLine_ReturnsCorrectLength)
{
    auto map = StraightMap();
    ASSERT_TRUE(map);
    CollisionLayer cl; cl.Build(*map);
    Pathfinder pf; pf.Init(&cl);
    auto res = pf.FindPath({0,0}, {4,0});
    ASSERT_EQ(res.error, PathError::None);
    EXPECT_EQ(res.cells.size(), 5u);
}

TEST(Pathfinder, FindPath_WithObstacle_AvoidsCollider)
{
    auto map = ObstacleMap();
    ASSERT_TRUE(map);
    CollisionLayer cl; cl.Build(*map);
    Pathfinder pf; pf.Init(&cl);
    auto res = pf.FindPath({0,1}, {2,1});
    ASSERT_EQ(res.error, PathError::None);
    // expected path length 5 (around obstacle)
    EXPECT_EQ(res.cells.size(), 5u);
}

TEST(Pathfinder, FindPath_NoPath_ReturnsError)
{
    auto map = BlockedMap();
    ASSERT_TRUE(map);
    CollisionLayer cl; cl.Build(*map);
    Pathfinder pf; pf.Init(&cl);
    auto res = pf.FindPath({0,0}, {2,0});
    EXPECT_EQ(res.error, PathError::NoPath);
}

TEST(Pathfinder, InvalidEndpoint_ReturnsError)
{
    auto map = BlockedMap();
    ASSERT_TRUE(map);
    CollisionLayer cl; cl.Build(*map);
    Pathfinder pf; pf.Init(&cl);
    auto res = pf.FindPath({1,0}, {0,0});
    EXPECT_EQ(res.error, PathError::InvalidEndpoint);
}

TEST(Pathfinder, FindPath_Performance_WithinBudget)
{
    auto map = StraightMap();
    ASSERT_TRUE(map);
    CollisionLayer cl; cl.Build(*map);
    Pathfinder pf; pf.Init(&cl);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i=0;i<100;i++) {
        pf.FindPath({0,0}, {4,0});
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    EXPECT_LT(ms, 200); // rough budget
}
