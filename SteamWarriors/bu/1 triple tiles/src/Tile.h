#ifndef TILE_H_INCLUDED
#define TILE_H_INCLUDED

#include <string>

#include <SFML/Graphics.hpp>

//structure containing general info about a tile
struct TileInfo
{
    //variables
    std::string name, description;
    sf::IntRect textureRect;
    //constructor
    TileInfo(const std::string& name, const std::string& description,
             const sf::IntRect& textureRect):
        name(name), description(description), textureRect(textureRect)
    {}
};

//various tile types
struct UpTile
{
    //types
    enum Model
    {
        Air,
        Rock,
        Max
    };
    //variables
    TileInfo info;
    bool passable;
    //constructor
    UpTile(const std::string& name, const std::string& description, sf::IntRect textureRect,
           bool passable):
        info{name, description, textureRect}, passable(passable)
    {}
};
struct TerrainTile
{
    enum Model
    {
        Sand,
        Max
    };
    TileInfo info;
    //constructor
    TerrainTile(const std::string& name, const std::string& description, sf::IntRect textureRect):
        info{name, description, textureRect}
    {}
};
struct AreaTile
{
    enum Model
    {
        Nothing,
        Max
    };
    TileInfo info;
    //constructor
    AreaTile(const std::string& name, const std::string& description, sf::IntRect textureRect):
        info{name, description, textureRect}
    {}
};
struct MultiTile
{
    UpTile::Model up;
    TerrainTile::Model terrain;
    AreaTile::Model area;
};

//global tile models
extern std::array<UpTile, UpTile::Max> gUpTileModels;
extern std::array<TerrainTile, TerrainTile::Max> gTerrainTileModels;
extern std::array<AreaTile, AreaTile::Max> gAreaTileModels;


#endif // TILE_H_INCLUDED
