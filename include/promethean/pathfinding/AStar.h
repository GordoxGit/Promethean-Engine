#pragma once
#include <glm/vec2.hpp>
#include <vector>

namespace pe::pathfinding {

using Grid = std::vector<std::vector<int>>;

struct Node {
    glm::ivec2 pos{};
    int g{0};
    int f{0};
    int parent{-1};
    bool inOpen{false};
    bool closed{false};
};

std::vector<glm::ivec2> FindPath(const Grid& grid, const glm::ivec2& start, const glm::ivec2& goal);

} // namespace pe::pathfinding
