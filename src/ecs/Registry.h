#pragma once
#include <atomic>
#include <vector>
#include <mutex>
#include <tuple>
#include <type_traits>
#include "ecs/Component.h"
#include "ecs/Entity.h"

namespace pe::ecs {

class Registry {
public:
    Registry();

    Entity create();
    void destroy(EntityID id);

    template<typename C, typename... Args>
    C& add(EntityID id, Args&&... args);

    template<typename C>
    void remove(EntityID id);

    template<typename C>
    bool has(EntityID id) const;

    template<typename... Cs, typename Fn>
    void for_each(Fn&& fn);

private:
    std::atomic<EntityID> m_nextId;
    std::vector<EntityID> m_free;
    mutable std::mutex m_mutex;

    ComponentPool<Position>   m_positions;
    ComponentPool<Velocity>   m_velocities;
    ComponentPool<Renderable> m_renderables;

    template<typename C>
    ComponentPool<C>& pool();
    template<typename C>
    const ComponentPool<C>& pool() const;
};

// ----- Template implementations -----

template<typename C, typename... Args>
inline C& Registry::add(EntityID id, Args&&... args) {
    return pool<C>().emplace(id, std::forward<Args>(args)...);
}

template<typename C>
inline void Registry::remove(EntityID id) { pool<C>().remove(id); }

template<typename C>
inline bool Registry::has(EntityID id) const { return pool<C>().has(id); }

template<typename... Cs, typename Fn>
inline void Registry::for_each(Fn&& fn) {
    auto& primary = pool<std::tuple_element_t<0, std::tuple<Cs...>>>();
    for(size_t i=0;i<primary.entities().size();++i) {
        EntityID id = primary.entities()[i];
        if((pool<Cs>().has(id) && ...)) {
            fn(*pool<Cs>().try_get(id)...);
        }
    }
}

template<typename C>
inline ComponentPool<C>& Registry::pool() {
    if constexpr (std::is_same_v<C, Position>) return m_positions;
    else if constexpr (std::is_same_v<C, Velocity>) return m_velocities;
    else if constexpr (std::is_same_v<C, Renderable>) return m_renderables;
}

template<typename C>
inline const ComponentPool<C>& Registry::pool() const {
    if constexpr (std::is_same_v<C, Position>) return m_positions;
    else if constexpr (std::is_same_v<C, Velocity>) return m_velocities;
    else if constexpr (std::is_same_v<C, Renderable>) return m_renderables;
}

} // namespace pe::ecs
