#include <gtest/gtest.h>
#include "core/Engine.h"

// Tests de base pour le moteur
TEST(EngineTest, Initialize) {
    Promethean::Engine engine;
    EXPECT_TRUE(engine.Initialize());
    EXPECT_TRUE(engine.IsInitialized());
}

TEST(EngineTest, InitializeTwice) {
    Promethean::Engine engine;
    EXPECT_TRUE(engine.Initialize());
    EXPECT_TRUE(engine.Initialize()); // Should succeed and return true
    EXPECT_TRUE(engine.IsInitialized());
}

TEST(EngineTest, Shutdown) {
    Promethean::Engine engine;
    ASSERT_TRUE(engine.Initialize());
    engine.Shutdown();
    EXPECT_FALSE(engine.IsInitialized());
}

TEST(EngineTest, ShutdownWithoutInit) {
    Promethean::Engine engine;
    EXPECT_FALSE(engine.IsInitialized());
    engine.Shutdown(); // Should not crash
    EXPECT_FALSE(engine.IsInitialized());
}

TEST(EngineTest, ShutdownTwice) {
    Promethean::Engine engine;
    ASSERT_TRUE(engine.Initialize());
    engine.Shutdown();
    engine.Shutdown(); // Should not crash
    EXPECT_FALSE(engine.IsInitialized());
}

TEST(EngineTest, DestructorCleansUp) {
    {
        Promethean::Engine engine;
        ASSERT_TRUE(engine.Initialize());
        // Destructor should be called here
    }
    // Test passes if no crash occurs
}
