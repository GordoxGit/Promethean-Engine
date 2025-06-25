#include "assets/TileMap.h"
#include "core/LogSystem.h"
#include "assets/AssetManager.h"
#include <tinyxml2.h>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace Promethean {

static TilesetInfo ParseTileset(tinyxml2::XMLElement* tsEl, const std::filesystem::path& base)
{
    TilesetInfo info{};
    tsEl->QueryIntAttribute("firstgid", &info.firstGid);
    info.columns = tsEl->IntAttribute("columns");
    info.tileW   = tsEl->IntAttribute("tilewidth");
    info.tileH   = tsEl->IntAttribute("tileheight");

    tinyxml2::XMLElement* imgEl = tsEl->FirstChildElement("image");
    if (imgEl)
    {
        const char* src = imgEl->Attribute("source");
        if(src) info.imagePath = (base / src).string();
    }
    return info;
}

static bool AssignTileset(TileMapLayer& layer, const std::vector<TilesetInfo>& sets)
{
    for(size_t i=0;i<layer.gids.size();++i)
    {
        int gid = layer.gids[i];
        if(!gid) continue;
        for(size_t t=sets.size(); t>0; --t)
        {
            if(gid >= sets[t-1].firstGid)
            {
                layer.tilesetId = static_cast<int>(t-1);
                return true;
            }
        }
    }
    return false;
}

std::shared_ptr<TileMap> LoadTileMap(const std::string& path)
{
    auto map = std::make_shared<TileMap>();
    tinyxml2::XMLDocument doc;
    if(doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS)
    {
        LogSystem::Instance().Error("Failed to load TMX {}", path);
        return nullptr;
    }

    auto dir = std::filesystem::path(path).parent_path();

    tinyxml2::XMLElement* mapEl = doc.FirstChildElement("map");
    if(!mapEl) return nullptr;
    const char* orient = mapEl->Attribute("orientation");
    if(orient && std::string(orient) != "orthogonal")
    {
        LogSystem::Instance().Error("Unsupported orientation {}", orient);
        return nullptr;
    }
    mapEl->QueryIntAttribute("width", &map->mapSize.x);
    mapEl->QueryIntAttribute("height", &map->mapSize.y);
    mapEl->QueryIntAttribute("tilewidth", &map->tileW);
    mapEl->QueryIntAttribute("tileheight", &map->tileH);

    for(auto ts = mapEl->FirstChildElement("tileset"); ts; ts = ts->NextSiblingElement("tileset"))
    {
        if(const char* source = ts->Attribute("source"))
        {
            tinyxml2::XMLDocument tsx;
            std::filesystem::path tsxPath = dir / source;
            if(tsx.LoadFile(tsxPath.string().c_str()) != tinyxml2::XML_SUCCESS)
                continue;
            tinyxml2::XMLElement* tsRoot = tsx.FirstChildElement("tileset");
            if(!tsRoot) continue;
            TilesetInfo info = ParseTileset(tsRoot, tsxPath.parent_path());
            ts->QueryIntAttribute("firstgid", &info.firstGid);
            map->tilesets.push_back(info);
        }
        else
        {
            map->tilesets.push_back(ParseTileset(ts, dir));
        }
    }

    for(auto layerEl = mapEl->FirstChildElement("layer"); layerEl; layerEl = layerEl->NextSiblingElement("layer"))
    {
        TileMapLayer layer{};
        const char* vis = layerEl->Attribute("visible");
        layer.visible = !vis || std::string(vis) != "0";
        if(const char* name = layerEl->Attribute("name")) layer.name = name;
        layerEl->QueryIntAttribute("width", &layer.size.x);
        layerEl->QueryIntAttribute("height", &layer.size.y);
        auto dataEl = layerEl->FirstChildElement("data");
        if(!dataEl) continue;
        const char* encoding = dataEl->Attribute("encoding");
        if(!encoding || std::string(encoding) != "csv") continue;
        std::string csv = dataEl->GetText() ? dataEl->GetText() : "";
        std::stringstream ss(csv);
        std::string item;
        while(std::getline(ss,item,','))
        {
            layer.gids.push_back(std::stoi(item));
        }
        if(!AssignTileset(layer, map->tilesets))
            layer.tilesetId = 0;
        map->layers.push_back(std::move(layer));
    }

    return map;
}

} // namespace Promethean
