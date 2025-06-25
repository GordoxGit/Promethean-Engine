#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <utility>
#include <tuple>
#include <type_traits>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include "core/LogSystem.h"

namespace pe::ecs {
    using EntityID = uint32_t;

    struct Position { glm::vec2 value{0.f,0.f}; };
    struct Velocity { glm::vec2 value{0.f,0.f}; };
    struct Renderable { uint32_t texture_id{0}; glm::vec4 uv{0.f,0.f,1.f,1.f}; float z{0.f}; };

    template<typename C>
    class ComponentPool {
    public:
        C& add(EntityID id, const C& c) {
            if(m_lookup.find(id) != m_lookup.end())
                return m_data[m_lookup[id]];
            size_t idx = m_data.size();
            m_data.push_back(c);
            m_entities.push_back(id);
            m_lookup[id] = idx;
            return m_data.back();
        }

        template<typename... Args>
        C& emplace(EntityID id, Args&&... args) {
            if(m_lookup.find(id) != m_lookup.end())
                return m_data[m_lookup[id]];
            size_t idx = m_data.size();
            m_data.emplace_back(std::forward<Args>(args)...);
            m_entities.push_back(id);
            m_lookup[id] = idx;
            return m_data.back();
        }

        void remove(EntityID id) {
            auto it = m_lookup.find(id);
            if(it == m_lookup.end()) return;
            size_t idx = it->second;
            size_t last = m_data.size()-1;
            if(idx != last) {
                m_data[idx] = m_data[last];
                m_entities[idx] = m_entities[last];
                m_lookup[m_entities[idx]] = idx;
            }
            m_data.pop_back();
            m_entities.pop_back();
            m_lookup.erase(it);
        }

        bool has(EntityID id) const {
            return m_lookup.find(id) != m_lookup.end();
        }

        C* try_get(EntityID id) {
            auto it = m_lookup.find(id);
            if(it == m_lookup.end()) return nullptr;
            return &m_data[it->second];
        }
        const std::vector<EntityID>& entities() const { return m_entities; }
    private:
        std::vector<C> m_data;
        std::vector<EntityID> m_entities;
        std::unordered_map<EntityID,size_t> m_lookup;
    };

    class Registry {
    public:
        Registry();
        EntityID create();
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

    // ---- Template implementations ----
    inline Registry::Registry() : m_nextId(1) {}

    inline EntityID Registry::create() {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(!m_free.empty()) {
            EntityID id = m_free.back();
            m_free.pop_back();
            if(id % 256 == 0) LogSystem::Instance().Info("Recycled ID {}", id); // will compile once we include log
            return id;
        }
        return m_nextId++;
    }

    inline void Registry::destroy(EntityID id) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_positions.remove(id);
        m_velocities.remove(id);
        m_renderables.remove(id);
        m_free.push_back(id);
    }

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

