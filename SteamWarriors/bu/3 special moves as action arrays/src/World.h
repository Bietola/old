#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <cmath>
#include <cassert>
#include <string>
#include <vector>
#include <functional>

#include <TGUI/TGUI.hpp>

#include "utils/graphics.h"
#include "Tile.h"
#include "Entity.h"
#include "media.h"
#include "utils.h"

///World class
class World: public sf::Transformable, public sf::Drawable
{
public:
    ///constants
    //tile default draw dimension
    static const float TILE_WIDTH;
    static const float TILE_HEIGHT;
    ///constructors
    //makes random world
    World(unsigned int width, unsigned int height, int seed);
    ///world querying functions
    bool posOutOfBounds(const sf::Vector2i& pos) const;
    bool rectOutOfBounds(const sf::IntRect& rect) const;
    bool posPassable(const sf::Vector2i& pos) const;
    bool posOccupied(const sf::Vector2i& pos) const;
    bool posSpawnable(const sf::Vector2i& pos) const;
    sf::Vector2u getSize() const {return {mWidth, mHeight};}
    Entity* getEntityAtPos(const sf::Vector2i& position);
    ///checkers
    //world logic is done and world is ready to receive user input
    bool isDone() const {return mIsDone;}
    ///world editing functions
    //get a tile at the given position
    const MultiTile& getTile(const sf::Vector2i& pos) const;
    MultiTile& getTile(const sf::Vector2i& pos);
    //performs a given function over a given rectangle
    bool fillRect(const sf::IntRect& rect, const std::function<void(MultiTile&)>& func);
    ///entity handling functions
    //get a random spawn position
    sf::Vector2i getRandSpawnPosition() const;
    //spawn an entity on the world - returns false on failure
    bool spawnEntity(Entity* entity);
    //handle input handling for all entities
    void handleEvent(const sf::Event& event);
    //handle logic for all entities
    void handleLogic(const sf::Time& time);
    ///graphics functions
    //signal world to update vertices at given positions
    void updateVerticesInArea(const sf::IntRect& area) const {mNeedToUpdateVertices = true; mUpdateArea = area;}
    //signal world to update all positions
    void updateAllVertices() const {mNeedToUpdateVertices = true; mUpdateArea = {0, 0, mWidth, mHeight};}
    //draw word to a render target
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
    ///tile variables
    //tiles
    std::vector<MultiTile> mTiles;
    //width and height
    unsigned int mWidth, mHeight;
    ///entity variables
    //all entities
    std::vector<Entity*> mEntities;
    ///logic variables
    //true if world is ready to receive user input
    bool mIsDone = true;
    ///graphics variables
    //tilesheet used for tiles
    const sf::Texture* mTileSheetTexture;
    //vertex array used to render tiles
    mutable sf::VertexArray mVertices;
    mutable bool mNeedToUpdateVertices = false;
    mutable sf::IntRect mUpdateArea;
    ///graphics functions
    //update a vertex quad at a given position
    void updateQuadAtPosition(const sf::Vector2i& pos, const sf::IntRect& textureRect) const;
};


#endif // WORLD_H_INCLUDED
