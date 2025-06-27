#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <utility>

namespace pe::ecs {

using EntityID = uint32_t;

struct Position { float x{0.f}; float y{0.f}; };
struct Velocity { float x{0.f}; float y{0.f}; };
struct Renderable { uint32_t texture_id{0}; float u{0.f}; float v{0.f}; float w{1.f}; float h{1.f}; float z{0.f}; };

// Basic component pool storing components per entity
template<typename C>
class ComponentPool {
public:
    template<typename... Args>
    C& emplace(EntityID id, Args&&... args) {
        if(auto it = m_lookup.find(id); it != m_lookup.end())
            return m_data[it->second];
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
        size_t last = m_data.size() - 1;
        if(idx != last) {
            m_data[idx] = m_data[last];
            m_entities[idx] = m_entities[last];
            m_lookup[m_entities[idx]] = idx;
        }
        m_data.pop_back();
        m_entities.pop_back();
        m_lookup.erase(it);
    }

    bool has(EntityID id) const { return m_lookup.find(id) != m_lookup.end(); }

    C* try_get(EntityID id) {
        auto it = m_lookup.find(id);
        if(it == m_lookup.end()) return nullptr;
        return &m_data[it->second];
    }

    const std::vector<EntityID>& entities() const { return m_entities; }

private:
    std::vector<C> m_data;
    std::vector<EntityID> m_entities;
    std::unordered_map<EntityID, size_t> m_lookup;
};

} // namespace pe::ecs
