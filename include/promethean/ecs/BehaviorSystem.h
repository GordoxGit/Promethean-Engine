#pragma once
#include "ecs/System.h"
#include "ecs/BehaviorComponent.h"
#include "ecs/Component.h"

namespace pe::ecs {

class BehaviorSystem : public System {
public:
    explicit BehaviorSystem(Registry& reg) : System(reg) {}
    void Update(float dt) override;
};

} // namespace pe::ecs
