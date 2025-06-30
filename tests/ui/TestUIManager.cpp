#include "ui/UIManager.h"
#include "ui/UIWindow.h"
#include "ui/UIButton.h"
#include <gtest/gtest.h>

static SDL_Event MouseDown(int x,int y)
{
    SDL_Event e{}; e.type = SDL_MOUSEBUTTONDOWN; e.button.button=SDL_BUTTON_LEFT; e.button.x=x; e.button.y=y; return e;
}
static SDL_Event MouseUp(int x,int y)
{
    SDL_Event e{}; e.type = SDL_MOUSEBUTTONUP; e.button.button=SDL_BUTTON_LEFT; e.button.x=x; e.button.y=y; return e;
}

TEST(UIManager, PushPopCallbacks)
{
    UIManager mgr;
    UIWindow win;
    int shown=0, hidden=0, clicked=0;
    win.SetOnShow([&]{ ++shown; });
    win.SetOnHide([&]{ ++hidden; });

    UIButton btn("play");
    btn.SetOnClick([&]{ ++clicked; });
    btn.SetPosition({0.f,0.f});
    btn.SetSize({100.f,40.f});
    win.AddChild(&btn);

    mgr.PushWindow(&win);
    EXPECT_EQ(shown,1);
    EXPECT_EQ(mgr.WindowCount(),1u);

    mgr.HandleEvent(MouseDown(10,10));
    mgr.HandleEvent(MouseUp(10,10));
    EXPECT_EQ(clicked,1);

    mgr.PopWindow();
    EXPECT_EQ(hidden,1);
    EXPECT_EQ(mgr.WindowCount(),0u);
}
