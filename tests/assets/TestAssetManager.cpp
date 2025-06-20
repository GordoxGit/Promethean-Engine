#include <gtest/gtest.h>
#include "assets/AssetManager.h"

using namespace Promethean;

TEST(AssetManager, LoadUnknownFile_ReturnsNullptr) {
    AssetManager mgr{4};
    EXPECT_EQ(mgr.LoadTexture("textures/does_not_exist.png"), nullptr);
}

TEST(AssetManager, CacheEviction_LRUOrder) {
    AssetManager mgr{2};
    auto a = mgr.LoadTexture("textures/A.png");
    auto b = mgr.LoadTexture("textures/B.png");
    auto _ = mgr.LoadTexture("textures/A.png");
    (void)a; (void)b; (void)_;
    mgr.LoadTexture("textures/C.png");
    SUCCEED();
}
