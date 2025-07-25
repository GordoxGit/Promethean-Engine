#pragma once
#include <atomic>
#include <vector>
#include <mutex>
#include <tuple>
#include <type_traits>
#include "ecs/Component.h"
#include "ecs/Entity.h"
#include "ecs/NavComponent.h"
#include "ecs/BehaviorComponent.h"

namespace Promethean { class SaveManager; }

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

    size_t active() const;

    friend class Promethean::SaveManager;

private:
    std::atomic<EntityID> m_nextId;
    std::vector<EntityID> m_free;
    mutable std::mutex m_mutex;

    ComponentPool<Position>   m_positions;
    ComponentPool<Velocity>   m_velocities;
    ComponentPool<Renderable> m_renderables;
    ComponentPool<NavComponent> m_navs;
    ComponentPool<BehaviorComponent> m_behaviors;

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
    else if constexpr (std::is_same_v<C, NavComponent>) return m_navs;
    else if constexpr (std::is_same_v<C, BehaviorComponent>) return m_behaviors;
}

template<typename C>
inline const ComponentPool<C>& Registry::pool() const {
    if constexpr (std::is_same_v<C, Position>) return m_positions;
    else if constexpr (std::is_same_v<C, Velocity>) return m_velocities;
    else if constexpr (std::is_same_v<C, Renderable>) return m_renderables;
    else if constexpr (std::is_same_v<C, NavComponent>) return m_navs;
    else if constexpr (std::is_same_v<C, BehaviorComponent>) return m_behaviors;
}

inline size_t Registry::active() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    return static_cast<size_t>(m_nextId.load() - 1) - m_free.size();
}

} // namespace pe::ecs
