#include "editor/WorldEditor.h"
#include <gtest/gtest.h>
#include <filesystem>

using namespace Promethean;

TEST(WorldEditor, CreateSaveLoad)
{
    WorldEditor ed;
    ed.NewMap(2,1,32,32);
    ed.SetTile(0,0,5);
    auto tmp = std::filesystem::temp_directory_path() / "map.json";
    ASSERT_TRUE(ed.SaveJSON(tmp.string()));

    WorldEditor loaded;
    ASSERT_TRUE(loaded.LoadJSON(tmp.string()));
    ASSERT_EQ(loaded.GetMap().mapSize.x, 2);
    ASSERT_EQ(loaded.GetMap().layers[0].gids[0], 5);
}
