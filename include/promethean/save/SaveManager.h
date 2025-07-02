#pragma once
#define JSON_HAS_FILESYSTEM 1
#include <nlohmann/json.hpp>
#include "ecs/ecs.h"
#include <string>

namespace Promethean {

/**
 * @brief Utility responsible for game state persistence.
 */
class SaveManager {
public:
    /** Serialize and write the entire world to disk. */
    static bool SaveToFile(const std::string& path, const pe::ecs::Registry& reg);

    /** Load a world from disk, creating entities and components. */
    static bool LoadFromFile(const std::string& path, pe::ecs::Registry& reg);

    /** Convert a single entity to JSON representation. */
    static nlohmann::json Serialize(const pe::ecs::Registry& reg, pe::ecs::Entity entity);

    /** Recreate components for an entity from JSON data. */
    static void Deserialize(pe::ecs::Registry& reg, pe::ecs::Entity entity, const nlohmann::json& data);
};

} // namespace Promethean
