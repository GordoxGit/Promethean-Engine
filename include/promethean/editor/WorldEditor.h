#pragma once
#include "assets/TileMap.h"
#include <string>

namespace Promethean {

/** Minimal 2D world editor handling a single tile layer. */
class WorldEditor {
public:
    WorldEditor() = default;

    /** Create a new blank map. */
    void NewMap(int width, int height, int tileW, int tileH);

    /** Set the tile gid at the given coordinates. */
    void SetTile(int x, int y, int gid);

    /** Remove a tile at the given coordinates. */
    void RemoveTile(int x, int y);

    /** Export the current map to a JSON file. */
    bool SaveJSON(const std::string& path) const;

    /** Load a map from a JSON file. */
    bool LoadJSON(const std::string& path);

    /** Access the underlying map. */
    const TileMap& GetMap() const { return m_map; }

private:
    TileMap m_map;
};

} // namespace Promethean
