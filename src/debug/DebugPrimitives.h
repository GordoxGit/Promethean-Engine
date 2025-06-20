#pragma once
#include <glm/vec2.hpp>

/** Primitive line with duration in seconds. */
struct DebugLine {
    glm::vec2 start;  ///< Line start point
    glm::vec2 end;    ///< Line end point
    float duration;   ///< Remaining lifetime
};

/** Primitive circle with duration in seconds. */
struct DebugCircle {
    glm::vec2 center; ///< Circle center
    float radius;     ///< Circle radius
    float duration;   ///< Remaining lifetime
};

/** Axis-aligned box with duration in seconds. */
struct DebugBox {
    glm::vec2 min;    ///< Minimum corner
    glm::vec2 max;    ///< Maximum corner
    float duration;   ///< Remaining lifetime
};
