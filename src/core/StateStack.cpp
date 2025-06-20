// ============================
// File: src/core/StateStack.h
// ============================
#pragma once

#include <vector>
#include <memory>
#include <SDL.h>

class BatchRenderer;
class State;

// Actions requested on the stack
enum class StateAction { Push, Pop, Replace };

/**
 * @brief A simple LIFO stack for game states with deferred request processing.
 *
 *  - Push: pauses the current top (if any), pushes the new state and calls OnEnter().
 *  - Pop:  calls OnExit() on the top, pops it, resumes the new top (if any).
 *  - Replace: same as Pop followed by Push in one atomic operation.
 */
class StateStack {
public:
    // Request a stack mutation (executed at next ApplyRequests())
    void Request(StateAction act, std::unique_ptr<State> next = nullptr);

    // Execute all pending requests – must be called once per frame (typically from Engine::Run)
    void ApplyRequests();

    // Convenience accessors
    State* Current() const;
    size_t Size()   const;

    // Forward to current state (if any)
    void HandleEvent(const SDL_Event& ev);
    void Update(float dt);
    void Render(BatchRenderer& renderer);

private:
    struct Pending { StateAction action; std::unique_ptr<State> state; };

    std::vector<std::unique_ptr<State>> m_stack;    // active states
    std::vector<Pending>                m_pending;  // queued mutations
    std::vector<std::unique_ptr<State>> m_garbage;  // states awaiting destruction (after OnExit)
};

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
    // Destroy states that were already exited two frames ago
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
