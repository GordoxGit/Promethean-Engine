#pragma once
#include <glm/vec2.hpp>
#include <vector>

namespace pe::ecs {

struct NavComponent {
    glm::ivec2 position{};       ///< current cell
    glm::ivec2 destination{};    ///< goal cell
    std::vector<glm::ivec2> path;///< computed path
    size_t current{0};           ///< current step index
};

} // namespace pe::ecs
