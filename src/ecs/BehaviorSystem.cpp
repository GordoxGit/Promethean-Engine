#include "ecs/BehaviorSystem.h"
#include "ecs/Registry.h"
#include <glm/glm.hpp>

namespace pe::ecs {

void BehaviorSystem::Update(float dt) {
    registry().for_each<BehaviorComponent, Position>([dt](BehaviorComponent& beh, Position& pos) {
        switch(beh.state) {
        case BehaviorState::Idle:
            beh.timer += dt;
            if(beh.timer >= beh.idleDuration) {
                beh.timer = 0.f;
                beh.state = BehaviorState::Seek;
            }
            break;
        case BehaviorState::Seek: {
            glm::vec2 cur{pos.x, pos.y};
            glm::vec2 dir = beh.target - cur;
            float dist = glm::length(dir);
            if(dist < 0.1f) {
                beh.state = BehaviorState::Idle;
                beh.timer = 0.f;
            } else if(dist > 0.f) {
                dir /= dist;
                pos.x += dir.x * beh.speed * dt;
                pos.y += dir.y * beh.speed * dt;
            }
            break;
        }
        default: break;
        }
    });
}

} // namespace pe::ecs
