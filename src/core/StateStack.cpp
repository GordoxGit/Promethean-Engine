#include "core/StateStack.h"

void StateStack::Request(StateAction action, std::unique_ptr<State> next)
{
    m_pending.push_back({action, std::move(next)});
}

void StateStack::ApplyRequests()
{
    for (auto& req : m_pending)
    {
        switch (req.act)
        {
            case StateAction::Push:
                if (!m_stack.empty())
                    m_stack.back()->Pause();
                m_stack.push_back(std::move(req.state));
                if (m_stack.back())
                    m_stack.back()->OnEnter();
                break;

            case StateAction::Pop:
                if (!m_stack.empty())
                {
                    m_stack.back()->OnExit();
                    m_stack.pop_back();
                    if (!m_stack.empty())
                        m_stack.back()->Resume();
                }
                break;

            case StateAction::Replace:
                if (!m_stack.empty())
                {
                    m_stack.back()->OnExit();
                    m_stack.pop_back();
                }
                if (req.state)
                {
                    m_stack.push_back(std::move(req.state));
                    m_stack.back()->OnEnter();
                }
                break;
        }
    }
    m_pending.clear();
}

State* StateStack::Current() const
{
    return m_stack.empty() ? nullptr : m_stack.back().get();
}

void StateStack::HandleEvent(const SDL_Event& ev)
{
    if (auto* cur = Current())
        cur->HandleEvent(ev);
}

void StateStack::Update(float dt)
{
    if (auto* cur = Current())
        cur->Update(dt);
}

void StateStack::Render(BatchRenderer& renderer)
{
    if (auto* cur = Current())
        cur->Render(renderer);
}

size_t StateStack::Size() const
{
    return m_stack.size();
}
