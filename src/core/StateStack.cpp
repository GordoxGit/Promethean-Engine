// File: src/core/StateStack.cpp  (updated)
#include "core/StateStack.h"
#include "core/State.h"

using namespace Promethean;

void StateStack::Request(StateAction act, std::unique_ptr<State> s)
{
    m_pending.push_back({act, std::move(s)});
}

void StateStack::ApplyRequests()
{
    // Libère la garbage d'il y a deux frames (les états déjà vérifiés dans les tests)
    m_garbage.clear();

    for (auto &req : m_pending)
    {
        switch (req.act)
        {
        case StateAction::Push:
            if (!m_stack.empty()) m_stack.back()->Pause();
            m_stack.push_back(std::move(req.state));
            m_stack.back()->OnEnter();
            break;

        case StateAction::Pop:
            if (!m_stack.empty()) {
                auto popped = std::move(m_stack.back());
                popped->OnExit();
                m_stack.pop_back();
                if (!m_stack.empty()) m_stack.back()->Resume();
                m_garbage.push_back(std::move(popped)); // différer la destruction
            }
            break;

        case StateAction::Replace:
            if (!m_stack.empty()) {
                auto replaced = std::move(m_stack.back());
                replaced->OnExit();
                m_stack.pop_back();
                m_garbage.push_back(std::move(replaced));
            }
            m_stack.push_back(std::move(req.state));
            m_stack.back()->OnEnter();
            break;
        }
    }
    m_pending.clear();
}

State *StateStack::Current() const { return m_stack.empty() ? nullptr : m_stack.back().get(); }
size_t  StateStack::Size()    const { return m_stack.size(); }

void StateStack::HandleEvent(const SDL_Event &e) { if (auto s = Current()) s->HandleEvent(e); }
void StateStack::Update(float dt)                { if (auto s = Current()) s->Update(dt);    }
void StateStack::Render(BatchRenderer &r)        { if (auto s = Current()) s->Render(r);     }
