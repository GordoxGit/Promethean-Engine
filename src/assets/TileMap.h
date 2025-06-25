#ifndef PROMETHEAN_TILEMAP_H
#define PROMETHEAN_TILEMAP_H

#include <glm/vec2.hpp>
#include <string>
#include <vector>
#include <memory>

namespace Promethean {

struct TilesetInfo {
    int firstGid{};
    std::string imagePath;
    int columns{};
    int tileW{};
    int tileH{};
};

struct TileMapLayer {
    std::string name;
    bool visible{true};
    glm::ivec2 size{};
    std::vector<int> gids;
    int tilesetId{-1};
};

struct TileMap {
    glm::ivec2 mapSize{};
    int tileW{};
    int tileH{};
    std::vector<TilesetInfo> tilesets;
    std::vector<TileMapLayer> layers;
};

std::shared_ptr<TileMap> LoadTileMap(const std::string& path);

} // namespace Promethean

#endif // PROMETHEAN_TILEMAP_H
