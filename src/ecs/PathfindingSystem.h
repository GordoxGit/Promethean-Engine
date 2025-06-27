#pragma once
#include "ecs/System.h"
#include "pathfinding/AStar.h"
#include "ecs/NavComponent.h"

namespace pe::ecs {

class PathfindingSystem : public System {
public:
    PathfindingSystem(Registry& reg, const pathfinding::Grid* grid);

    void Update(float dt) override;
    void SetGrid(const pathfinding::Grid* grid) { m_grid = grid; }

private:
    const pathfinding::Grid* m_grid{nullptr};
};

} // namespace pe::ecs
