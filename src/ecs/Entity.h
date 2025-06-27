#pragma once
#include <cstdint>

namespace pe::ecs {
class Registry;

class Entity {
public:
    using Id = uint32_t;

    Entity() = default;
    Entity(Registry* reg, Id id) : m_registry(reg), m_id(id) {}

    Id id() const { return m_id; }
    bool valid() const { return m_registry != nullptr && m_id != 0; }

    void destroy();

private:
    Registry* m_registry{nullptr};
    Id        m_id{0};
};

} // namespace pe::ecs
