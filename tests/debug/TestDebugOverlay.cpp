#include "debug/DebugOverlay.h"
#include "renderer/BatchRenderer.h"
#include <gtest/gtest.h>

TEST(DebugOverlay, AddAndExpireLine)
{
    auto& ov = DebugOverlay::Get();
    ov.Reset();
    ov.AddLine({0.f,0.f},{1.f,1.f},0.1f);
    EXPECT_EQ(ov.GetLineCount(),1u);
    ov.Update(0.05f);
    EXPECT_EQ(ov.GetLineCount(),1u);
    ov.Update(0.06f);
    EXPECT_EQ(ov.GetLineCount(),0u);
}

TEST(DebugOverlay, UpdateRemovesExpired)
{
    auto& ov = DebugOverlay::Get();
    ov.Reset();
    ov.AddLine({0,0},{1,1},0.1f);
    ov.AddLine({1,1},{2,2},0.2f);
    ov.Update(0.15f);
    EXPECT_EQ(ov.GetLineCount(),1u);
}

TEST(DebugOverlay, RenderInvokesDraw)
{
    auto& ov = DebugOverlay::Get();
    ov.Reset();
    ov.AddLine({0,0},{1,1},0.2f);
    BatchRenderer br;
    ASSERT_TRUE(br.Init());
    int before = TestGL::GetDrawArraysCount();
    ov.Render(br);
    int after = TestGL::GetDrawArraysCount();
    EXPECT_EQ(after, before + 1);
    br.Shutdown();
}
