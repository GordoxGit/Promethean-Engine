#include "core/EventBus.h"
#include "core/LogSystem.h"
#include <gtest/gtest.h>
#include <spdlog/sinks/base_sink.h>
#include <atomic>
#include <thread>
#include <mutex>

struct FooEvent { int v; };

class CollectSinkEB : public spdlog::sinks::base_sink<std::mutex>
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

TEST(EventBus, SubscribeAndPublish)
{
    auto& bus = EventBus::Instance();
    int value = 0;
    size_t id = bus.Subscribe<FooEvent>([&](const std::any& e){
        const auto& ev = std::any_cast<const std::reference_wrapper<const FooEvent>&>(e).get();
        value = ev.v;
    });
    FooEvent fe{42};
    bus.Publish(fe);
    EXPECT_EQ(value, 42);
    bus.Unsubscribe(id);
}

TEST(EventBus, Unsubscribe)
{
    auto& bus = EventBus::Instance();
    int count = 0;
    size_t id = bus.Subscribe<FooEvent>([&](const std::any&){ ++count; });
    bus.Unsubscribe(id);
    bus.Publish(FooEvent{1});
    EXPECT_EQ(count, 0);
}

TEST(EventBus, MultipleSubscribers)
{
    auto& bus = EventBus::Instance();
    std::vector<int> order;
    size_t id1 = bus.Subscribe<FooEvent>([&](const std::any&){ order.push_back(1); });
    size_t id2 = bus.Subscribe<FooEvent>([&](const std::any&){ order.push_back(2); });
    bus.Publish(FooEvent{0});
    ASSERT_EQ(order.size(), 2u);
    EXPECT_EQ(order[0], 1);
    EXPECT_EQ(order[1], 2);
    bus.Unsubscribe(id1);
    bus.Unsubscribe(id2);
}

TEST(EventBus, ThreadSafety)
{
    auto& bus = EventBus::Instance();
    std::atomic<int> counter{0};
    size_t sub = bus.Subscribe<FooEvent>([&](const std::any&){ counter.fetch_add(1); });

    auto publishTask = [&](){
        for(int i=0;i<100;i++) bus.Publish(FooEvent{i});
    };
    auto subscribeTask = [&](){
        for(int i=0;i<100;i++) {
            size_t id = bus.Subscribe<FooEvent>([](const std::any&){});
            bus.Unsubscribe(id);
        }
    };
    std::thread t1(publishTask), t2(publishTask), t3(subscribeTask), t4(subscribeTask);
    t1.join(); t2.join(); t3.join(); t4.join();

    EXPECT_EQ(counter.load(), 200);
    bus.Unsubscribe(sub);
}

TEST(EventBus, UnknownUnsubscribe)
{
    auto& log = LogSystem::Instance();
    auto sink = std::make_shared<CollectSinkEB>();
    log.SetCustomSinkForTesting(sink);

    auto& bus = EventBus::Instance();
    bus.Unsubscribe(9999);

    ASSERT_FALSE(sink->messages.empty());
    bool foundWarn = false;
    for(const auto& msg : sink->messages)
        if(msg.find("[warn]") != std::string::npos || msg.find("[warning]") != std::string::npos)
            foundWarn = true;
    EXPECT_TRUE(foundWarn);
}

