#include "core/StateStack.h"
#include <gtest/gtest.h>

class DummyState : public State {
public:
    bool entered = false;
    bool exited  = false;
    bool paused  = false;
    bool resumed = false;
    int  events  = 0;

    void OnEnter() override { entered = true; }
    void OnExit()  override { exited = true; }
    void Pause()   override { paused = true; }
    void Resume()  override { resumed = true; }
    void HandleEvent(const SDL_Event&) override { ++events; }
    void Update(float) override {}
    void Render(BatchRenderer&) override {}
};

TEST(StateStack, Push_AddsState)
{
    StateStack stack;
    auto st = std::make_unique<DummyState>();
    DummyState* ptr = st.get();
    stack.Request(StateAction::Push, std::move(st));
    stack.ApplyRequests();
    EXPECT_EQ(stack.Size(), 1u);
    EXPECT_TRUE(ptr->entered);
}

TEST(StateStack, Pop_RemovesState)
{
    StateStack stack;
    auto st = std::make_unique<DummyState>();
    DummyState* ptr = st.get();
    stack.Request(StateAction::Push, std::move(st));
    stack.ApplyRequests();
    stack.Request(StateAction::Pop);
    stack.ApplyRequests();
    EXPECT_EQ(stack.Size(), 0u);
    EXPECT_TRUE(ptr->exited);
}

TEST(StateStack, Replace_SwapsState)
{
    StateStack stack;
    auto s1 = std::make_unique<DummyState>();
    DummyState* p1 = s1.get();
    stack.Request(StateAction::Push, std::move(s1));
    stack.ApplyRequests();

    auto s2 = std::make_unique<DummyState>();
    DummyState* p2 = s2.get();
    stack.Request(StateAction::Replace, std::move(s2));
    stack.ApplyRequests();

    EXPECT_EQ(stack.Size(), 1u);
    EXPECT_TRUE(p1->exited);
    EXPECT_TRUE(p2->entered);
}

TEST(StateStack, PauseAndResume_Called)
{
    StateStack stack;
    auto s1 = std::make_unique<DummyState>();
    DummyState* p1 = s1.get();
    stack.Request(StateAction::Push, std::move(s1));
    stack.ApplyRequests();

    auto s2 = std::make_unique<DummyState>();
    DummyState* p2 = s2.get();
    stack.Request(StateAction::Push, std::move(s2));
    stack.ApplyRequests();
    EXPECT_TRUE(p1->paused);

    stack.Request(StateAction::Pop);
    stack.ApplyRequests();
    EXPECT_TRUE(p2->exited);
    EXPECT_TRUE(p1->resumed);
}

TEST(StateStack, HandleEvent_CurrentOnly)
{
    StateStack stack;
    auto s1 = std::make_unique<DummyState>();
    DummyState* p1 = s1.get();
    stack.Request(StateAction::Push, std::move(s1));
    stack.ApplyRequests();
    auto s2 = std::make_unique<DummyState>();
    DummyState* p2 = s2.get();
    stack.Request(StateAction::Push, std::move(s2));
    stack.ApplyRequests();

    SDL_Event ev{}; ev.type = SDL_USEREVENT;
    stack.HandleEvent(ev);
    stack.HandleEvent(ev);

    EXPECT_EQ(p1->events, 0);
    EXPECT_EQ(p2->events, 2);
}
