#include "ecs/PathfindingSystem.h"
#include "ecs/Registry.h"

namespace pe::ecs {

PathfindingSystem::PathfindingSystem(Registry& reg, const pathfinding::Grid* grid)
    : System(reg), m_grid(grid) {}

void PathfindingSystem::Update(float)
{
    if(!m_grid) return;
    registry().for_each<NavComponent>([&](NavComponent& nav){
        if(nav.position == nav.destination) return;
        if(nav.path.empty() || nav.current >= nav.path.size()) {
            nav.path = pathfinding::FindPath(*m_grid, nav.position, nav.destination);
            nav.current = nav.path.empty() ? 0 : 1; // skip starting cell
        }
        if(!nav.path.empty() && nav.current < nav.path.size()) {
            nav.position = nav.path[nav.current];
            ++nav.current;
        }
    });
}

} // namespace pe::ecs
