#pragma once

namespace pe::ecs {
class Registry;

class System {
public:
    explicit System(Registry& reg) : m_registry(reg) {}
    virtual ~System() = default;
    virtual void Update(float dt) = 0;
protected:
    Registry& registry() { return m_registry; }
private:
    Registry& m_registry;
};

} // namespace pe::ecs
