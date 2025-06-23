#pragma once
#include "PlayerAction.h"

/** Event published when a mapped action changes state. */
struct ActionStateChangedEvent {
    PlayerAction action;
    bool pressed;
};
