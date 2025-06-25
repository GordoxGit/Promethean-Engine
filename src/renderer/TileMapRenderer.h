#pragma once
#include "assets/TileMap.h"
#include "assets/AssetManager.h"
#include "renderer/BatchRenderer.h"

namespace Promethean {

void RenderTileMap(BatchRenderer& renderer, AssetManager& assets, const TileMap& map);

}
