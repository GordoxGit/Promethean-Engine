// ============================
// File: src/core/StateStack.h
// ============================
#pragma once

#include <vector>
#include <memory>
#include <SDL.h>

class BatchRenderer;
class State;

/// Lifecycle actions that can be queued for the state stack
enum class StateAction { Push, Pop, Replace };

/**
 * @brief LIFO stack managing game states with deferred mutation logic.
 *
 *  • Push      – pause current top state (if any) then push a new state and call OnEnter().
 *  • Pop       – call OnExit() on the top, pop it, resume the new top (if any).
 *  • Replace   – Pop + Push atomically (old state gets OnExit, new state gets OnEnter).
 *
 * Use Request() from gameplay code; the real mutations are applied on the next ApplyRequests()
 * call (once per frame) to avoid iterator invalidation while states execute.
 */
class StateStack {
public:
    /// Queue a mutation (executed on the next ApplyRequests call)
    void Request(StateAction act, std::unique_ptr<State> next = nullptr);

    /// Apply all queued mutations – call exactly once per frame (after Update)
    void ApplyRequests();

    /// Shortcut to the currently active state (nullptr if stack is empty)
    [[nodiscard]] State* Current() const;

    /// Number of active states
    [[nodiscard]] size_t Size() const;

    // ---------------------------------------------------------------------
    // Forward helpers – they simply propagate to the current state if any
    // ---------------------------------------------------------------------
    void HandleEvent(const SDL_Event& ev);
    void Update(float dt);
    void Render(BatchRenderer& renderer);

private:
    struct Pending {
        StateAction action;
        std::unique_ptr<State> state; // may be null for Pop
    };

    std::vector<std::unique_ptr<State>> m_stack;   ///< Active states (top = back())
    std::vector<Pending>                m_pending; ///< Deferred requests
    std::vector<std::unique_ptr<State>> m_garbage; ///< States awaiting final destruction
};
