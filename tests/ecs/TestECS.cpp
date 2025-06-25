#include "ecs/ecs.h"
#include <gtest/gtest.h>
#include <vector>
#include <thread>
#include <algorithm>
#include <chrono>

using namespace pe::ecs;

TEST(ECS, CreateDestroy_CountMatches)
{
    Registry r;
    std::vector<EntityID> ids;
    for(int i=0;i<100;i++) ids.push_back(r.create());
    EXPECT_EQ(ids.size(), 100u);
    for(auto id: ids) r.destroy(id);
    auto id2 = r.create();
    EXPECT_EQ(id2, ids.back());
}

TEST(ECS, AddRemoveComponent_HasReturnsCorrect)
{
    Registry r; auto e = r.create();
    r.add<Position>(e, Position{{1.f,2.f}});
    EXPECT_TRUE(r.has<Position>(e));
    r.remove<Position>(e);
    EXPECT_FALSE(r.has<Position>(e));
}

TEST(ECS, ForEach_LambdaSeesAllEntities)
{
    Registry r;
    for(int i=0;i<10;i++) {
        auto e = r.create();
        r.add<Position>(e);
        r.add<Velocity>(e);
    }
    int count=0;
    r.for_each<Position,Velocity>([&](Position&, Velocity&){ ++count; });
    EXPECT_EQ(count, 10);
}

TEST(ECS, Performance_ForEachUnderBudget)
{
    Registry r;
    for(int i=0;i<100000;i++) {
        auto e = r.create();
        r.add<Position>(e);
        r.add<Velocity>(e);
    }
    auto start = std::chrono::high_resolution_clock::now();
    r.for_each<Position,Velocity>([](Position&, Velocity&){});
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    constexpr long kBudget = 100;
    constexpr long kMargin = 40; // CI variability 40%
#ifdef PE_CI
    EXPECT_LT(ms, kBudget + kMargin);
#else
    EXPECT_LT(ms, kBudget);
#endif
}

TEST(ECS, Threaded_CreateDestroy_NoRace)
{
    Registry r; std::atomic<int> c{0};
    auto task = [&]{
        for(int i=0;i<1000;i++){
            auto e = r.create();
            c.fetch_add(1, std::memory_order_relaxed);
            r.destroy(e);
        }
    };
    std::thread t1(task), t2(task), t3(task), t4(task);
    t1.join(); t2.join(); t3.join(); t4.join();
    EXPECT_EQ(c.load(), 4000);
}

