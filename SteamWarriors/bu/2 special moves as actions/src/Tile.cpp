#include "Tile.h"

//global tile models
std::array<UpTile, UpTile::Max> gUpTileModels =
{
    UpTile("air",  "empty space", {0, 0, 64, 64},  true),
    UpTile("rock", "rock", {64, 0, 64, 64}, false)
};
std::array<TerrainTile, TerrainTile::Max> gTerrainTileModels =
{
    TerrainTile("sand", "some sand", {128, 0, 64, 64})
};
std::array<AreaTile, AreaTile::Max> gAreaTileModels =
{
    AreaTile("nothing", "nothing", {0, 0, 0, 0})
};
