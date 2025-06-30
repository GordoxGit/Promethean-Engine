#include "ui/UIWindow.h"

void UIWindow::Show()
{
    if(!m_visible)
    {
        m_visible = true;
        if(m_onShow) m_onShow();
    }
}

void UIWindow::Hide()
{
    if(m_visible)
    {
        m_visible = false;
        if(m_onHide) m_onHide();
    }
}

void UIWindow::Draw(BatchRenderer& renderer)
{
    if(!m_visible) return;
    WidgetContainer::Draw(renderer);
}

bool UIWindow::HandleEvent(const SDL_Event& event)
{
    if(!m_visible) return false;
    return WidgetContainer::HandleEvent(event);
}
