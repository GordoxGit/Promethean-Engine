#include "editor/WorldEditor.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace Promethean {

void WorldEditor::NewMap(int width, int height, int tileW, int tileH)
{
    m_map.mapSize = {width, height};
    m_map.tileW = tileW;
    m_map.tileH = tileH;
    m_map.tilesets.clear();
    TilesetInfo ts{};
    ts.firstGid = 1;
    ts.columns = 1;
    ts.tileW = tileW;
    ts.tileH = tileH;
    m_map.tilesets.push_back(ts);
    m_map.layers.clear();
    TileMapLayer layer{};
    layer.name = "Layer1";
    layer.visible = true;
    layer.size = {width, height};
    layer.tilesetId = 0;
    layer.gids.assign(width * height, 0);
    m_map.layers.push_back(std::move(layer));
}

void WorldEditor::SetTile(int x, int y, int gid)
{
    if(m_map.layers.empty()) return;
    auto& layer = m_map.layers[0];
    if(x < 0 || y < 0 || x >= layer.size.x || y >= layer.size.y) return;
    layer.gids[y * layer.size.x + x] = gid;
}

void WorldEditor::RemoveTile(int x, int y)
{
    SetTile(x, y, 0);
}

bool WorldEditor::SaveJSON(const std::string& path) const
{
    if(m_map.layers.empty()) return false;
    const auto& layer = m_map.layers[0];
    nlohmann::json j;
    j["width"] = m_map.mapSize.x;
    j["height"] = m_map.mapSize.y;
    j["tileW"] = m_map.tileW;
    j["tileH"] = m_map.tileH;
    j["gids"] = layer.gids;
    std::ofstream ofs(path);
    if(!ofs.good()) return false;
    ofs << j.dump(4);
    return true;
}

bool WorldEditor::LoadJSON(const std::string& path)
{
    std::ifstream ifs(path);
    if(!ifs.good()) return false;
    nlohmann::json j; ifs >> j;
    int w = j.value("width",0);
    int h = j.value("height",0);
    int tw = j.value("tileW",32);
    int th = j.value("tileH",32);
    if(!j.contains("gids")) return false;
    NewMap(w,h,tw,th);
    auto& layer = m_map.layers[0];
    layer.gids = j["gids"].get<std::vector<int>>();
    if(static_cast<int>(layer.gids.size()) != w*h)
        layer.gids.assign(w*h,0);
    return true;
}

} // namespace Promethean
