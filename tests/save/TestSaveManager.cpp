#include "save/SaveManager.h"
#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <algorithm>

using namespace Promethean;
using namespace pe::ecs;

TEST(SaveManager, SaveAndLoadRegistry) {
    Registry reg;
    // create a couple of entities
    Entity e1 = reg.create();
    reg.add<Position>(e1.id()).x = 1.f;
    reg.add<Velocity>(e1.id()).y = -1.f;

    Entity e2 = reg.create();
    reg.add<Renderable>(e2.id()).texture_id = 42u;

    auto tmp = std::filesystem::temp_directory_path() / "save_test.json";
    ASSERT_TRUE(SaveManager::SaveToFile(tmp.string(), reg));

    Registry loaded;
    ASSERT_TRUE(SaveManager::LoadFromFile(tmp.string(), loaded));

    auto tmp2 = std::filesystem::temp_directory_path() / "save_test2.json";
    ASSERT_TRUE(SaveManager::SaveToFile(tmp2.string(), loaded));

    std::ifstream f1(tmp), f2(tmp2);
    nlohmann::json j1; f1 >> j1; nlohmann::json j2; f2 >> j2;
    auto normalize = [](nlohmann::json j){
        for(auto& e : j["entities"]) e.erase("id");
        std::sort(j["entities"].begin(), j["entities"].end(), [](const auto& a, const auto& b){
            return a.dump() < b.dump();
        });
        return j;
    };
    EXPECT_EQ(normalize(j1).dump(), normalize(j2).dump());
}
