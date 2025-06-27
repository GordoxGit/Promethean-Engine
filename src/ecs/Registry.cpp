#include "ecs/Registry.h"
#include "core/LogSystem.h"

namespace pe::ecs {

Registry::Registry() : m_nextId(1) {}

Entity Registry::create() {
    std::lock_guard<std::mutex> lock(m_mutex);
    EntityID id;
    if(!m_free.empty()) {
        id = m_free.back();
        m_free.pop_back();
        if(id % 256 == 0)
            LogSystem::Instance().Info("Recycled ID {}", id);
    } else {
        id = m_nextId++;
    }
    return Entity(this, id);
}

void Registry::destroy(EntityID id) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_positions.remove(id);
    m_velocities.remove(id);
    m_renderables.remove(id);
    m_free.push_back(id);
}

} // namespace pe::ecs
