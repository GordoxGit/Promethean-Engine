#pragma once
#include "assets/TileMap.h"
#include <glm/vec2.hpp>
#include <unordered_map>
#include <vector>
#include <optional>

namespace Promethean {

struct AABBCollider {
    glm::vec2 min{0.f};
    glm::vec2 max{0.f};
};

enum class CollisionError {
    MissingCollisionLayer
};

struct CollisionEvent {
    AABBCollider collider;
};

class CollisionLayer {
public:
    std::optional<CollisionError> Build(const TileMap& map);
    std::vector<AABBCollider> Query(const glm::vec2& p) const;

private:
    static constexpr float CELL_SIZE = 256.f;
    std::vector<AABBCollider> m_colliders;
    std::unordered_map<long long, std::vector<size_t>> m_hash;
};

} // namespace Promethean
