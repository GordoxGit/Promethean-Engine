#include "plugin/PluginManager.h"
#include "scripting/ScriptingManager.h"
#include "ecs/ecs.h"
#include <gtest/gtest.h>

using namespace Promethean;
using namespace pe::ecs;

TEST(PluginManager, LoadLuaPlugin)
{
    Registry reg;
    auto& sm = pe::scripting::ScriptingManager::Instance();
    ASSERT_TRUE(sm.Init(&reg));

    auto& pm = PluginManager::Instance();
    ASSERT_TRUE(pm.LoadFromDirectory("plugins"));
    sm.Shutdown();

    ASSERT_EQ(reg.active(), 1u);
    ASSERT_EQ(pm.GetPlugins().size(), 1u);
}
