// src/core/StateStack.h
#pragma once

#include <vector>
#include <memory>
#include <SDL.h>
#include "core/State.h"
#include "renderer/BatchRenderer.h"

namespace Promethean {

enum class StateAction { Push, Pop, Replace };

class StateStack {
public:
    void Request(StateAction act, std::unique_ptr<State> next = nullptr);
    void ApplyRequests();

    State* Current() const;
    size_t Size() const;

    void HandleEvent(const SDL_Event& ev);
    void Update(float dt);
    void Render(BatchRenderer& renderer);

private:
    struct Pending { StateAction action; std::unique_ptr<State> state; };

    std::vector<std::unique_ptr<State>> m_stack;
    std::vector<Pending>               m_pending;
    std::vector<std::unique_ptr<State>> m_garbage; // différée pour tests
};

} // namespace Promethean

//-------------------------------------------------------------
// src/core/StateStack.cpp
//-------------------------------------------------------------

#include "core/StateStack.h"

namespace Promethean {

void StateStack::Request(StateAction act, std::unique_ptr<State> next)
{
    m_pending.push_back({act, std::move(next)});
}

void StateStack::ApplyRequests()
{
    // libère les états sortants après la frame
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
                auto popped = std::move(m_stack.back());
                popped->OnExit();
                m_stack.pop_back();
                if (!m_stack.empty())
                    m_stack.back()->Resume();
                m_garbage.push_back(std::move(popped));
            }
            break;

        case StateAction::Replace:
            if (!m_stack.empty()) {
                auto oldTop = std::move(m_stack.back());
                oldTop->OnExit();
                m_stack.pop_back();
                m_garbage.push_back(std::move(oldTop));
            }
            m_stack.push_back(std::move(req.state));
            m_stack.back()->OnEnter();
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

} // namespace Promethean
