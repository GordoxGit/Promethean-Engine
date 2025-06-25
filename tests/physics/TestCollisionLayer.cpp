#include "physics/CollisionLayer.h"
#include "assets/TileMap.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

using namespace Promethean;

static std::shared_ptr<TileMap> SimpleMap()
{
    const char* xml = R"(<map width='1' height='1' tilewidth='32' tileheight='32'>
      <tileset firstgid='1' name='dummy' tilewidth='32' tileheight='32' tilecount='1' columns='1'>
        <image source='dummy.png' width='32' height='32'/>
      </tileset>
      <layer name='Layer' width='1' height='1'><data encoding='csv'>0</data></layer>
      <objectgroup name='collision'>
        <object id='1' x='0' y='0' width='32' height='32'/>
      </objectgroup>
    </map>)";
    auto tmp = std::filesystem::temp_directory_path()/"c.tmx";
    std::ofstream(tmp) << xml;
    return LoadTileMap(tmp.string());
}

TEST(CollisionLayer, Build_SimpleMap_ReturnsCorrectCount)
{
    auto map = SimpleMap();
    ASSERT_TRUE(map);
    CollisionLayer cl;
    auto err = cl.Build(*map);
    EXPECT_FALSE(err.has_value());
    auto res = cl.Query({0.f,0.f});
    EXPECT_EQ(res.size(),1u);
}

TEST(CollisionLayer, Query_PointOutside_ReturnsEmpty)
{
    auto map = SimpleMap();
    ASSERT_TRUE(map);
    CollisionLayer cl; cl.Build(*map);
    auto res = cl.Query({40.f,40.f});
    EXPECT_TRUE(res.empty());
}

TEST(CollisionLayer, Query_PointInside_ReturnsCollider)
{
    auto map = SimpleMap();
    ASSERT_TRUE(map);
    CollisionLayer cl; cl.Build(*map);
    auto res = cl.Query({16.f,16.f});
    ASSERT_EQ(res.size(),1u);
    EXPECT_FLOAT_EQ(res[0].max.x,32.f);
}
