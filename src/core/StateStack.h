#pragma once
#include <vector>
#include <memory>
#include "State.h"

/** Possible state operations. */
enum class StateAction { Push, Pop, Replace };

/**
 * \brief Stack of game states with deferred modifications.
 */
class StateStack {
public:
    /** Request a stack operation. */
    void Request(StateAction action, std::unique_ptr<State> next = nullptr);

    /** Apply pending requests. */
    void ApplyRequests();

    /** Current active state or nullptr. */
    State* Current() const;

    /** Forward event to current state. */
    void HandleEvent(const SDL_Event& ev);

    /** Update the current state. */
    void Update(float dt);

    /** Render the current state. */
    void Render(BatchRenderer& renderer);

    /** Size of the stack. */
    size_t Size() const;

private:
    std::vector<std::unique_ptr<State>> m_stack;
    struct Pending { StateAction act; std::unique_ptr<State> state; };
    std::vector<Pending> m_pending;
};
