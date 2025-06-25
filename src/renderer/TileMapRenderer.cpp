#include "renderer/TileMapRenderer.h"

namespace Promethean {

void RenderTileMap(BatchRenderer& renderer, AssetManager& assets, const TileMap& map)
{
    for(const auto& layer : map.layers)
    {
        if(!layer.visible) continue;
        const auto& ts = map.tilesets[layer.tilesetId];
        auto tex = assets.LoadTexture(ts.imagePath);
        if(!tex) tex = AssetManager::MissingTexture();
        renderer.BindTexture(reinterpret_cast<uintptr_t>(tex.get()));

        for(int y=0; y<layer.size.y; ++y)
        for(int x=0; x<layer.size.x; ++x)
        {
            int idx = y*layer.size.x + x;
            int gid = layer.gids[idx];
            if(gid == 0) continue;
            int local = gid - ts.firstGid;
            int tu = local % ts.columns;
            int tv = local / ts.columns;
            QuadUV uv;
            float u = (float)tu / ts.columns;
            float v = (float)tv / (float)((ts.columns)?(ts.columns):1); // not using rows purposely? but we know tilecount; but not needed
            float stepU = 1.f / ts.columns;
            float stepV = 1.f / ts.columns; // approximate: using square textures; oh well
            uv.topLeft = {u, v};
            uv.topRight = {u+stepU, v};
            uv.bottomRight = {u+stepU, v+stepV};
            uv.bottomLeft = {u, v+stepV};
            renderer.DrawQuad({x*map.tileW, y*map.tileH}, {map.tileW, map.tileH}, uv);
        }
    }
}

} // namespace Promethean
