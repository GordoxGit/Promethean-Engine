#include "ui/UIManager.h"
#include "renderer/BatchRenderer.h"

void UIManager::PushWindow(UIWindow* window)
{
    if(!window) return;
    window->Show();
    m_stack.push_back(window);
}

void UIManager::PopWindow()
{
    if(m_stack.empty()) return;
    UIWindow* w = m_stack.back();
    w->Hide();
    m_stack.pop_back();
}

UIWindow* UIManager::TopWindow() const
{
    return m_stack.empty() ? nullptr : m_stack.back();
}

void UIManager::HandleEvent(const SDL_Event& event)
{
    if(auto* w = TopWindow())
        w->HandleEvent(event);
}

void UIManager::Render(BatchRenderer& renderer)
{
    for(auto* w : m_stack)
        w->Draw(renderer);
}
