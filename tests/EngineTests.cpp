#include <gtest/gtest.h>
#include "core/Engine.h"

TEST(EngineTest, Initialize) {
    Promethean::Engine engine;
    EXPECT_TRUE(engine.Initialize());
    engine.Shutdown();
}

TEST(EngineTest, Shutdown) {
    Promethean::Engine engine;
    ASSERT_TRUE(engine.Initialize());
    engine.Shutdown();
    EXPECT_FALSE(engine.IsInitialized());
}

