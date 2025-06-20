// Patch for StateStack to ensure OnExit is called on popped state and Resume on previous state
// File: src/core/StateStack.cpp

#include "core/StateStack.h"
#include "core/State.h"

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
                m_stack.back()->Pause();       // pause previous
            m_stack.push_back(std::move(req.state));
            m_stack.back()->OnEnter();       // enter new
            break;

        case StateAction::Pop:
            if (!m_stack.empty())
            {
                // call OnExit on state being popped
                m_stack.back()->OnExit();
                m_stack.pop_back();
                // resume underlying state if any
                if (!m_stack.empty())
                    m_stack.back()->Resume();
            }
            break;

        case StateAction::Replace:
            if (!m_stack.empty())
            {
                m_stack.back()->OnExit();      // exit old
                m_stack.pop_back();
            }
            m_stack.push_back(std::move(req.state));
            m_stack.back()->OnEnter();        // enter new
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
    if (auto s = Current())
        s->HandleEvent(ev);
}

void StateStack::Update(float dt)
{
    if (auto s = Current())
        s->Update(dt);
}

void StateStack::Render(BatchRenderer& renderer)
{
    if (auto s = Current())
        s->Render(renderer);
}

size_t StateStack::Size() const
{
    return m_stack.size();
}
