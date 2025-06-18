#include "core/LogSystem.h"
#include <gtest/gtest.h>
#include <spdlog/sinks/base_sink.h>
#include <mutex>

class CollectSink : public spdlog::sinks::base_sink<std::mutex>
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

TEST(LogSystem, SetLevel) {
    auto& log = LogSystem::Instance();
    log.SetLevel(LogSystem::Level::Debug);
    EXPECT_EQ(log.GetLevel(), LogSystem::Level::Debug);
}

TEST(LogSystem, DebugBelowThreshold) {
    auto& log = LogSystem::Instance();
    auto mem_sink = std::make_shared<CollectSink>();
    log.SetCustomSinkForTesting(mem_sink);
    log.SetLevel(LogSystem::Level::Info);
    log.Debug("hidden message");
    EXPECT_TRUE(mem_sink->messages.empty());
}

TEST(LogSystem, ErrorAlwaysPrinted) {
    auto& log = LogSystem::Instance();
    auto mem_sink = std::make_shared<CollectSink>();
    log.SetCustomSinkForTesting(mem_sink);
    log.SetLevel(LogSystem::Level::Warn);
    log.Error("critical failure");
    ASSERT_FALSE(mem_sink->messages.empty());
    EXPECT_NE(mem_sink->messages.front().find("[error]"), std::string::npos);
}

TEST(LogSystem, Flush) {
    auto& log = LogSystem::Instance();
    auto mem_sink = std::make_shared<CollectSink>();
    log.SetCustomSinkForTesting(mem_sink);
    log.SetLevel(LogSystem::Level::Info);
    log.Info("hello");
    log.Flush();
    EXPECT_TRUE(mem_sink->messages.empty());
}

TEST(LogSystem, Singleton) {
    auto& log1 = LogSystem::Instance();
    auto& log2 = LogSystem::Instance();
    EXPECT_EQ(&log1, &log2);
}
