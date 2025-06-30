#pragma once
#include "ecs/Component.h"
#include <glm/vec2.hpp>
#define JSON_HAS_FILESYSTEM 1
#include <nlohmann/json.hpp>

namespace pe::ecs {

enum class BehaviorState { Idle = 0, Seek, Flee, Patrol };

struct BehaviorComponent {
    BehaviorState state{BehaviorState::Idle};
    glm::vec2 target{0.f, 0.f};
    float timer{0.f};
    float idleDuration{1.f};
    float speed{1.f};

    nlohmann::json ToJSON() const;
    void FromJSON(const nlohmann::json& j);
};

} // namespace pe::ecs
