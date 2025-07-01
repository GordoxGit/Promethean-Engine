#include "scripting/ScriptingManager.h"
#include "ecs/ecs.h"
#include <gtest/gtest.h>

using namespace pe::scripting;
using namespace pe::ecs;

TEST(Scripting, RunScript_SpawnsEntity)
{
    Registry reg;
    auto& sm = ScriptingManager::Instance();
    ASSERT_TRUE(sm.Init(&reg));
    ASSERT_TRUE(sm.RunScript("assets/scripts/test_agent.lua"));
    sm.Shutdown();
    EXPECT_EQ(reg.active(), 1u);
}
