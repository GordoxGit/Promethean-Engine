#include "ecs/Entity.h"
#include "ecs/Registry.h"

namespace pe::ecs {

void Entity::destroy() {
    if(m_registry) {
        m_registry->destroy(m_id);
        m_registry = nullptr;
        m_id = 0;
    }
}

} // namespace pe::ecs
