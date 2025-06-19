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

