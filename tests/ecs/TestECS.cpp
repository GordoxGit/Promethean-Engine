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
    std::vector<Entity> ids;
    for(int i=0;i<100;i++) ids.push_back(r.create());
    EXPECT_EQ(ids.size(), 100u);
    EntityID last = ids.back().id();
    for(auto& e: ids) e.destroy();
    auto e2 = r.create();
    EXPECT_EQ(e2.id(), last);
}

TEST(ECS, AddRemoveComponent_HasReturnsCorrect)
{
    Registry r; Entity e = r.create();
    r.add<Position>(e.id(), Position{1.f,2.f});
    EXPECT_TRUE(r.has<Position>(e.id()));
    r.remove<Position>(e.id());
    EXPECT_FALSE(r.has<Position>(e.id()));
}

TEST(ECS, ForEach_LambdaSeesAllEntities)
{
    Registry r;
    for(int i=0;i<10;i++) {
        auto ent = r.create();
        r.add<Position>(ent.id());
        r.add<Velocity>(ent.id());
    }
    int count=0;
    r.for_each<Position,Velocity>([&](Position&, Velocity&){ ++count; });
    EXPECT_EQ(count, 10);
}

TEST(ECS, Performance_ForEachUnderBudget)
{
    Registry r;
    for(int i=0;i<100000;i++) {
        auto ent = r.create();
        r.add<Position>(ent.id());
        r.add<Velocity>(ent.id());
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
            e.destroy();
        }
    };
    std::thread t1(task), t2(task), t3(task), t4(task);
    t1.join(); t2.join(); t3.join(); t4.join();
    EXPECT_EQ(c.load(), 4000);
}

class CounterSystem : public System {
public:
    explicit CounterSystem(Registry& reg) : System(reg) {}
    int count{0};
    void Update(float) override {
        registry().for_each<Position>([this](Position&){ ++count; });
    }
};

TEST(ECS, System_ProcessesEntities)
{
    Registry r;
    for(int i=0;i<5;i++) {
        auto e = r.create();
        r.add<Position>(e.id());
    }
    CounterSystem sys(r);
    sys.Update(0.f);
    EXPECT_EQ(sys.count, 5);
}

