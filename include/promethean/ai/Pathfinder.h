#pragma once
#include "physics/CollisionLayer.h"
#include <glm/vec2.hpp>
#include <vector>

namespace pe::ai {

enum class PathError { None, NoPath, InvalidEndpoint };

struct PathResult {
    std::vector<glm::ivec2> cells;
    PathError error{PathError::None};
};

class Pathfinder {
public:
    void Init(const Promethean::CollisionLayer* collision);
    PathResult FindPath(const glm::ivec2& start, const glm::ivec2& goal) const;

    struct Node {
        glm::ivec2 pos{};
        int g{0};
        int f{0};
        int parent{-1};
        bool inOpen{false};
        bool closed{false};
    };

private:
    const Promethean::CollisionLayer* m_collision{nullptr};
    int m_width{0};
    int m_height{0};
    mutable std::vector<Node> m_nodes;
    mutable std::vector<int>  m_heap;
};

}
