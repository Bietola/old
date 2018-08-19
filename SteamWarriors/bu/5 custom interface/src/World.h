#ifndef WORLD_H_INCLUDED
#define WORLD_H_INCLUDED

#include <cmath>
#include <cassert>
#include <string>
#include <set>
#include <functional>

#include "utils/graphics.h"
#include "Tile.h"
#include "Entity.h"
#include "media.h"
#include "globals.h"
#include "utils.h"

///World class
class World: public sf::Transformable, public sf::Drawable
{
public:
    ///friends
    friend Entity;
    ///aliases
    using EntityWeakPtr = std::weak_ptr<Entity>;
    using EntityPtr = std::shared_ptr<Entity>;
    ///constructors
    //makes random world
    World(unsigned int width, unsigned int height, int seed);
    ///getters
    //get player
    std::shared_ptr<const Entity> getPlayer() const {return mPlayer;}
    ///world querying functions
    bool posOutOfBounds(const sf::Vector2i& pos) const;
    bool rectOutOfBounds(const sf::IntRect& rect) const;
    bool posPassable(const sf::Vector2i& pos) const;
    bool posOccupied(const sf::Vector2i& pos) const;
    bool posSpawnable(const sf::Vector2i& pos) const;
    sf::Vector2u getSize() const {return {mWidth, mHeight};}
    EntityWeakPtr getEntityAtPos(const sf::Vector2i& position);
    ///checkers
    //world logic is ready for used input
    bool isReadyForInput() const {return mIsReadyForInput;}
    ///world editing functions
    //get a tile at the given position
    const MultiTile& getTile(const sf::Vector2i& pos) const;
    MultiTile& getTile(const sf::Vector2i& pos);
    //performs a given function over a given rectangle
    bool fillRect(const sf::IntRect& rect, const std::function<void(MultiTile&)>& func);
    ///entity handling functions
    //compare two entities to keep them ordered within the mEntities set
    static bool compareEntities(std::shared_ptr<const Entity> lhs,
                                std::shared_ptr<const Entity> rhs)
        {return lhs->getStat("speed") < rhs->getStat("speed");}
    //get a random spawn position
    sf::Vector2i getRandSpawnPosition() const;
    //spawn an entity on the world - returns false on failure
    bool spawnEntity(std::shared_ptr<Entity> entity);
    //spawn player - return false on failure
    bool spawnPlayer(std::shared_ptr<Entity> entity);
    //handle input handling for all entities
    void handleEvent(const sf::Event& event);
    //handle logic related to time (animation and *interface* - handled here for optimization purpose)
    void handleTimeLogic(const sf::Time& time);
    //handle logic related to time
    void handleTurnLogic();
    ///graphics functions
    //converts tile coordinates to window coordinates
    static sf::Vector2f tileToWindowPosition(const sf::Vector2i& position) {return sf::Vector2f(position.x * TILE_WIDTH, position.y * TILE_HEIGHT);}
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
    std::multiset<std::shared_ptr<Entity>, decltype(&compareEntities)> mEntities;
    //player
    std::shared_ptr<Entity> mPlayer = nullptr;
    ///logic variables
    //true if world is ready to receive user input
    bool mIsReadyForInput = true;
    //player is done with his turn
    bool mPlayerIsDone = false;
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
