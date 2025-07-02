#pragma once
#include <glm/vec2.hpp>
#include <vector>
#define JSON_HAS_FILESYSTEM 1
#include <nlohmann/json.hpp>

namespace pe::ecs {

struct NavComponent {
    glm::ivec2 position{};       ///< current cell
    glm::ivec2 destination{};    ///< goal cell
    std::vector<glm::ivec2> path;///< computed path
    size_t current{0};           ///< current step index

    nlohmann::json ToJSON() const {
        nlohmann::json j;
        j["position"] = { {"x", position.x}, {"y", position.y} };
        j["destination"] = { {"x", destination.x}, {"y", destination.y} };
        j["current"] = current;
        j["path"] = nlohmann::json::array();
        for(const auto& p : path)
            j["path"].push_back({ {"x", p.x}, {"y", p.y} });
        return j;
    }

    void FromJSON(const nlohmann::json& j) {
        auto pos = j.at("position");
        position.x = pos.value("x",0); position.y = pos.value("y",0);
        auto dest = j.at("destination");
        destination.x = dest.value("x",0); destination.y = dest.value("y",0);
        current = j.value("current",0u);
        path.clear();
        if(j.contains("path")) {
            for(const auto& step : j["path"]) {
                glm::ivec2 v{ step.value("x",0), step.value("y",0) };
                path.push_back(v);
            }
        }
    }
};

} // namespace pe::ecs
