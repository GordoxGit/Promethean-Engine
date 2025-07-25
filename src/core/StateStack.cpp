// ============================
// File: src/core/StateStack.cpp
// ============================
#include "core/StateStack.h"
#include "core/State.h"
#include "renderer/BatchRenderer.h"

void StateStack::Request(StateAction act, std::unique_ptr<State> next)
{
    m_pending.push_back({act, std::move(next)});
}

void StateStack::ApplyRequests()
{
    // Clean up states that were exited in the previous frame
    m_garbage.clear();

    for (auto &req : m_pending)
    {
        switch (req.action)
        {
        case StateAction::Push:
            if (!m_stack.empty())
                m_stack.back()->Pause();
            m_stack.push_back(std::move(req.state));
            m_stack.back()->OnEnter();
            break;

        case StateAction::Pop:
            if (!m_stack.empty()) {
                auto dead = std::move(m_stack.back());
                dead->OnExit();
                m_stack.pop_back();
                if (!m_stack.empty())
                    m_stack.back()->Resume();
                m_garbage.push_back(std::move(dead));
            }
            break;

        case StateAction::Replace:
            if (!m_stack.empty()) {
                auto old = std::move(m_stack.back());
                old->OnExit();
                m_stack.pop_back();
                m_garbage.push_back(std::move(old));
            }
            if (req.state) {
                m_stack.push_back(std::move(req.state));
                m_stack.back()->OnEnter();
            }
            break;
        }
    }
    m_pending.clear();
}

State* StateStack::Current() const { return m_stack.empty() ? nullptr : m_stack.back().get(); }
size_t StateStack::Size()   const { return m_stack.size(); }

void StateStack::HandleEvent(const SDL_Event &e) { if (auto s = Current()) s->HandleEvent(e); }
void StateStack::Update(float dt)               { if (auto s = Current()) s->Update(dt);    }
void StateStack::Render(BatchRenderer &r)       { if (auto s = Current()) s->Render(r);     }
