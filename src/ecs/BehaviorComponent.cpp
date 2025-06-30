#include "ecs/BehaviorComponent.h"

namespace pe::ecs {

nlohmann::json BehaviorComponent::ToJSON() const {
    nlohmann::json j;
    j["state"] = static_cast<int>(state);
    j["target"] = { {"x", target.x}, {"y", target.y} };
    j["timer"] = timer;
    j["idleDuration"] = idleDuration;
    j["speed"] = speed;
    return j;
}

void BehaviorComponent::FromJSON(const nlohmann::json& j) {
    state = static_cast<BehaviorState>(j.value("state", 0));
    if(j.contains("target")) {
        const auto& t = j["target"];
        target.x = t.value("x", 0.f);
        target.y = t.value("y", 0.f);
    }
    timer = j.value("timer", 0.f);
    idleDuration = j.value("idleDuration", 1.f);
    speed = j.value("speed", 1.f);
}

} // namespace pe::ecs
