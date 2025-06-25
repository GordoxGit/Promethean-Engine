#include "physics/CollisionLayer.h"
#include "core/LogSystem.h"
#include <cmath>

namespace Promethean {

static long long Key(int x, int y) {
    return (static_cast<long long>(x) << 32) | static_cast<unsigned int>(y);
}

std::optional<CollisionError> CollisionLayer::Build(const TileMap& map)
{
    m_colliders.clear();
    m_hash.clear();
    const TileMap::ObjectGroup* group = nullptr;
    for(const auto& g : map.objectGroups)
        if(g.name == "collision") { group = &g; break; }
    if(!group)
    {
        LogSystem::Instance().Debug("Collision layer not found");
        return CollisionError::MissingCollisionLayer;
    }

    for(const auto& obj : group->objects)
    {
        AABBCollider c;
        c.min = glm::vec2(obj.pos);
        c.max = glm::vec2(obj.pos + obj.size);
        size_t index = m_colliders.size();
        m_colliders.push_back(c);

        int x0 = static_cast<int>(std::floor(c.min.x / CELL_SIZE));
        int y0 = static_cast<int>(std::floor(c.min.y / CELL_SIZE));
        int x1 = static_cast<int>(std::floor((c.max.x-1) / CELL_SIZE));
        int y1 = static_cast<int>(std::floor((c.max.y-1) / CELL_SIZE));
        for(int y=y0; y<=y1; ++y)
            for(int x=x0; x<=x1; ++x)
                m_hash[Key(x,y)].push_back(index);
    }
    return std::nullopt;
}

std::vector<AABBCollider> CollisionLayer::Query(const glm::vec2& p) const
{
    std::vector<AABBCollider> result;
    result.reserve(16);
    int cx = static_cast<int>(std::floor(p.x / CELL_SIZE));
    int cy = static_cast<int>(std::floor(p.y / CELL_SIZE));
    auto it = m_hash.find(Key(cx,cy));
    if(it != m_hash.end())
    {
        for(size_t idx : it->second)
        {
            const auto& c = m_colliders[idx];
            if(p.x >= c.min.x && p.x <= c.max.x &&
               p.y >= c.min.y && p.y <= c.max.y)
            {
                result.push_back(c);
                if(result.size() >= 16) break;
            }
        }
    }
    return result;
}

} // namespace Promethean
