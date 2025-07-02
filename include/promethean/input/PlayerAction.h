#pragma once

/**
 * @brief Logical player actions decoupled from input devices.
 */
enum class PlayerAction {
    MoveLeft, MoveRight, MoveUp, MoveDown,
    Jump, Confirm, Cancel,
    Count
};

static constexpr std::size_t PlayerActionCount = static_cast<std::size_t>(PlayerAction::Count);
