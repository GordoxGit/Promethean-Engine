#include <gtest/gtest.h>
#include "assets/TileMap.h"
#include <fstream>
#include <filesystem>

using namespace Promethean;

TEST(TileMapLoader, MinimalCsv)
{
    const char* xml = R"(<map width='2' height='1' tilewidth='32' tileheight='32' orientation='orthogonal'>
      <tileset firstgid='1' name='dummy' tilewidth='32' tileheight='32' tilecount='4' columns='2'>
        <image source='dummy.png' width='64' height='64'/>
      </tileset>
      <layer name='Layer1' width='2' height='1'><data encoding='csv'>1,2</data></layer>
    </map>)";
    auto tmp = std::filesystem::temp_directory_path() / "m.tmx";
    std::ofstream(tmp) << xml;
    auto map = LoadTileMap(tmp.string());
    ASSERT_TRUE(map);
    ASSERT_EQ(map->layers.size(),1u);
    EXPECT_EQ(map->layers[0].gids.size(),2u);
    EXPECT_EQ(map->layers[0].gids[1],2);
}
