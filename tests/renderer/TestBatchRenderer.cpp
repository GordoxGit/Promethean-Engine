#include "renderer/BatchRenderer.h"
#include "renderer/RenderEvents.h"
#include "core/EventBus.h"
#include "core/LogSystem.h"
#include <gtest/gtest.h>
#include <spdlog/sinks/base_sink.h>
#include <mutex>

class CollectSinkBR : public spdlog::sinks::base_sink<std::mutex>
{
public:
    std::vector<std::string> messages;
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override
    {
        spdlog::memory_buf_t buf;
        base_sink<std::mutex>::formatter_->format(msg, buf);
        messages.emplace_back(buf.begin(), buf.end());
    }
    void flush_() override { messages.clear(); }
};

TEST(BatchRenderer, InitSuccess)
{
    BatchRenderer br;
    EXPECT_TRUE(br.Init());
    br.Shutdown();
}

TEST(BatchRenderer, DoubleInit)
{
    BatchRenderer br;
    ASSERT_TRUE(br.Init());
    uint32_t vao = br.GetVao();
    EXPECT_TRUE(br.Init());
    EXPECT_EQ(vao, br.GetVao());
    br.Shutdown();
}

TEST(BatchRenderer, BeginWithoutInit)
{
    auto sink = std::make_shared<CollectSinkBR>();
    LogSystem::Instance().SetCustomSinkForTesting(sink);
    BatchRenderer br;
    br.Begin(100,100);
    bool found = false;
    for(const auto& msg : sink->messages)
        if(msg.find("error") != std::string::npos)
            found = true;
    EXPECT_TRUE(found);
}

TEST(BatchRenderer, DrawAfterShutdown)
{
    auto sink = std::make_shared<CollectSinkBR>();
    LogSystem::Instance().SetCustomSinkForTesting(sink);
    BatchRenderer br;
    ASSERT_TRUE(br.Init());
    br.Shutdown();
    br.DrawQuad({0,0}, {10,10});
    bool found = false;
    for(const auto& msg : sink->messages)
        if(msg.find("error") != std::string::npos)
            found = true;
    EXPECT_TRUE(found);
}

TEST(BatchRenderer, PublishFrameEvent)
{
    BatchRenderer br;
    ASSERT_TRUE(br.Init());
    int count = 0;
    auto id = EventBus::Instance().Subscribe<FrameRenderedEvent>([&](const std::any&){ ++count; });
    br.Flush();
    EventBus::Instance().Unsubscribe(id);
    EXPECT_EQ(count,1);
    br.Shutdown();
}

TEST(BatchRenderer, InitTexture_BindsOnce)
{
    BatchRenderer br;
    ASSERT_TRUE(br.Init());
    br.BindTexture(5);
    br.BindTexture(5);
    EXPECT_EQ(TestGL::GetBindTextureCount(), 1);
    EXPECT_EQ(TestGL::GetLastBoundTexture(), 5u);
    br.Shutdown();
}

TEST(BatchRenderer, DrawQuad_UsesUVs)
{
    BatchRenderer br;
    ASSERT_TRUE(br.Init());
    QuadUV uv; uv.topRight = {0.5f,0.f}; uv.bottomRight = {0.5f,0.5f}; uv.bottomLeft={0.f,0.5f};
    br.DrawQuad({1.f,2.f},{3.f,4.f}, uv);
    const float* data = TestGL::GetLastBufferData();
    const float expected[24] = {
        1.f,2.f, uv.topLeft.x,uv.topLeft.y,
        4.f,2.f, uv.topRight.x,uv.topRight.y,
        4.f,6.f, uv.bottomRight.x,uv.bottomRight.y,
        1.f,2.f, uv.topLeft.x,uv.topLeft.y,
        4.f,6.f, uv.bottomRight.x,uv.bottomRight.y,
        1.f,6.f, uv.bottomLeft.x,uv.bottomLeft.y
    };
    for(int i=0;i<24;i++)
        EXPECT_FLOAT_EQ(data[i], expected[i]);
    br.Shutdown();
}

TEST(BatchRenderer, DrawWithoutBind_UsesTex0)
{
    BatchRenderer br;
    ASSERT_TRUE(br.Init());
    br.DrawQuad({0,0},{1,1});
    EXPECT_EQ(TestGL::GetLastUniformTexture(),0);
    br.Shutdown();
}

TEST(BatchRenderer, ShaderUniforms_SetCorrectly)
{
    BatchRenderer br;
    ASSERT_TRUE(br.Init());
    br.BindTexture(7);
    glm::vec4 tint(0.2f,0.3f,0.4f,1.f);
    br.DrawQuad({0,0},{2,2}, {}, tint);
    glm::vec4 got = TestGL::GetLastTint();
    EXPECT_FLOAT_EQ(got.r,tint.r);
    EXPECT_FLOAT_EQ(got.g,tint.g);
    EXPECT_FLOAT_EQ(got.b,tint.b);
    EXPECT_FLOAT_EQ(got.a,tint.a);
    EXPECT_EQ(TestGL::GetLastUniformTexture(),0);
    br.Shutdown();
}

