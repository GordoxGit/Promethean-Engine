#include "save/SaveManager.h"
#include <fstream>
#include <unordered_set>

namespace Promethean {
using namespace pe::ecs;

nlohmann::json SaveManager::Serialize(const Registry& reg, Entity entity) {
    nlohmann::json j;
    j["id"] = entity.id();
    if(reg.has<Position>(entity.id())) {
        const auto* c = reg.pool<Position>().try_get(entity.id());
        j["Position"] = c->ToJSON();
    }
    if(reg.has<Velocity>(entity.id())) {
        const auto* c = reg.pool<Velocity>().try_get(entity.id());
        j["Velocity"] = c->ToJSON();
    }
    if(reg.has<Renderable>(entity.id())) {
        const auto* c = reg.pool<Renderable>().try_get(entity.id());
        j["Renderable"] = c->ToJSON();
    }
    if(reg.has<NavComponent>(entity.id())) {
        const auto* c = reg.pool<NavComponent>().try_get(entity.id());
        j["NavComponent"] = c->ToJSON();
    }
    if(reg.has<BehaviorComponent>(entity.id())) {
        const auto* c = reg.pool<BehaviorComponent>().try_get(entity.id());
        j["BehaviorComponent"] = c->ToJSON();
    }
    return j;
}

void SaveManager::Deserialize(Registry& reg, Entity entity, const nlohmann::json& data) {
    if(auto it = data.find("Position"); it != data.end()) {
        auto& c = reg.add<Position>(entity.id());
        c.FromJSON(*it);
    }
    if(auto it = data.find("Velocity"); it != data.end()) {
        auto& c = reg.add<Velocity>(entity.id());
        c.FromJSON(*it);
    }
    if(auto it = data.find("Renderable"); it != data.end()) {
        auto& c = reg.add<Renderable>(entity.id());
        c.FromJSON(*it);
    }
    if(auto it = data.find("NavComponent"); it != data.end()) {
        auto& c = reg.add<NavComponent>(entity.id());
        c.FromJSON(*it);
    }
    if(auto it = data.find("BehaviorComponent"); it != data.end()) {
        auto& c = reg.add<BehaviorComponent>(entity.id());
        c.FromJSON(*it);
    }
}

bool SaveManager::SaveToFile(const std::string& path, const Registry& reg) {
    nlohmann::json root;
    root["entities"] = nlohmann::json::array();
    std::unordered_set<EntityID> ids;
    for(auto id : reg.pool<Position>().entities()) ids.insert(id);
    for(auto id : reg.pool<Velocity>().entities()) ids.insert(id);
    for(auto id : reg.pool<Renderable>().entities()) ids.insert(id);
    for(auto id : reg.pool<NavComponent>().entities()) ids.insert(id);
    for(auto id : reg.pool<BehaviorComponent>().entities()) ids.insert(id);
    for(EntityID id : ids) {
        root["entities"].push_back(Serialize(reg, Entity{const_cast<Registry*>(&reg), id}));
    }
    std::ofstream ofs(path);
    if(!ofs.good()) return false;
    ofs << root.dump(4);
    return true;
}

bool SaveManager::LoadFromFile(const std::string& path, Registry& reg) {
    std::ifstream ifs(path);
    if(!ifs.good()) return false;
    nlohmann::json root; ifs >> root;
    if(!root.is_object() || !root.contains("entities")) return false;
    for(const auto& entData : root["entities"]) {
        Entity e = reg.create();
        Deserialize(reg, e, entData);
    }
    return true;
}

} // namespace Promethean
