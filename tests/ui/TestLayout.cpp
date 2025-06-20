#include "ui/HorizontalLayout.h"
#include "ui/VerticalLayout.h"
#include <gtest/gtest.h>

class DummyWidget : public Widget {
public:
    glm::vec2 pos{0.f,0.f};
    glm::vec2 size{0.f,0.f};
    void Draw(BatchRenderer&) override {}
    bool HandleEvent(const SDL_Event&) override { return false; }
    void SetPosition(const glm::vec2& p) override { pos = p; }
    void SetSize(const glm::vec2& s) override { size = s; }
};

TEST(HorizontalLayout, BasicPositions)
{
    HorizontalLayout layout;
    DummyWidget w1, w2;
    layout.AddChild(&w1);
    layout.AddChild(&w2);
    layout.SetPosition({0.f,0.f});
    layout.SetSize({200.f,50.f});

    float expectedWidth = (200.f - 5.f) / 2.f;
    EXPECT_FLOAT_EQ(w1.pos.x, 0.f);
    EXPECT_FLOAT_EQ(w2.pos.x, expectedWidth + 5.f);
    EXPECT_FLOAT_EQ(w1.size.x, expectedWidth);
    EXPECT_FLOAT_EQ(w2.size.x, expectedWidth);
    EXPECT_FLOAT_EQ(w1.size.y, 50.f);
}

TEST(VerticalLayout, BasicPositions)
{
    VerticalLayout layout;
    DummyWidget w1, w2;
    layout.AddChild(&w1);
    layout.AddChild(&w2);
    layout.SetPosition({10.f,20.f});
    layout.SetSize({100.f,200.f});

    float expectedHeight = (200.f - 5.f) / 2.f;
    EXPECT_FLOAT_EQ(w1.pos.x, 10.f);
    EXPECT_FLOAT_EQ(w1.pos.y, 20.f);
    EXPECT_FLOAT_EQ(w2.pos.x, 10.f);
    EXPECT_FLOAT_EQ(w2.pos.y, 20.f + expectedHeight + 5.f);
    EXPECT_FLOAT_EQ(w1.size.y, expectedHeight);
    EXPECT_FLOAT_EQ(w2.size.y, expectedHeight);
    EXPECT_FLOAT_EQ(w1.size.x, 100.f);
}

TEST(Layout, Resizing)
{
    HorizontalLayout layout;
    DummyWidget w1, w2, w3;
    layout.AddChild(&w1);
    layout.AddChild(&w2);
    layout.AddChild(&w3);
    layout.SetPosition({0.f,0.f});
    layout.SetSize({300.f,60.f});

    layout.SetSize({150.f,30.f});
    float width = (150.f - 5.f*2) / 3.f;
    EXPECT_FLOAT_EQ(w3.pos.x, 2*(width + 5.f));
    EXPECT_FLOAT_EQ(w2.size.x, width);
}

TEST(Layout, Spacing)
{
    HorizontalLayout layout(10.f);
    DummyWidget w1, w2;
    layout.AddChild(&w1);
    layout.AddChild(&w2);
    layout.SetPosition({0.f,0.f});
    layout.SetSize({210.f,40.f});

    float width = (210.f - 10.f)/2.f;
    EXPECT_FLOAT_EQ(w2.pos.x, width + 10.f);
    EXPECT_FLOAT_EQ(w1.size.x, width);
}

