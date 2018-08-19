#include "World.h"

//constants
const float World::TILE_WIDTH  = 32;
const float World::TILE_HEIGHT = 32;

//constructors
World::World(unsigned int width, unsigned int height, int seed):
    mWidth(width),
    mHeight(height),
    mTileSheetTexture(gTextureProvider.getMediaPtr("tiles")),
    mPositionsToUpdate(mWidth * mHeight, false)
{
    //initialize vertex array
    mGroundVertices.resize(mWidth * mHeight * 4);
    mGroundVertices.setPrimitiveType(sf::Quads);
    mAreaVertices.resize(mWidth * mHeight * 4);
    mAreaVertices.setPrimitiveType(sf::Quads);
    mEntityVertices.resize(mWidth * mHeight * 4);
    mEntityVertices.setPrimitiveType(sf::Quads);
    //use seed
    srand(seed);
    //fill the world with the basic tiles before generating anything
    mTiles.resize(width * height);
    fillRect({0, 0, mWidth - 1, mHeight - 1}, [](MultiTile& tile){
        tile.up = UpTile::Air;
        tile.terrain = TerrainTile::Sand;
        tile.area = AreaTile::Nothing;
    });
    //generate random "mountains" of rocks
    unsigned int maxRocks = rrange(width * height / 15, width * height / 7);
    for(int j = 0; j < maxRocks; j++){
        sf::IntRect rockR;
        rockR.width   = rrange(1, width / 10);
        rockR.height  = rrange(1, height / 10);
        rockR.left    = std::max(0.f, rrange(width) - rockR.width);
        rockR.top     = std::max(0.f, rrange(height) - rockR.height);
        if(!fillRect(rockR, [](MultiTile& tile){
            tile.up = UpTile::Rock;
        }))
            j--;
    }
}

//word checking functions
inline bool World::posOutOfBounds(const sf::Vector2i& pos) const {
    if(pos.x < 0 || pos.y < 0 || pos.x >= mWidth || pos.y >= mHeight)
        return true;
    return false;
}
inline bool World::rectOutOfBounds(const sf::IntRect& rect) const {
    if(rect.left < 0 || rect.top < 0 || rect.left + rect.width >= mWidth || rect.top + rect.height >= mHeight)
        return true;
    return false;
}
inline bool World::posPassable(const sf::Vector2i& pos) const {
    const auto& tile = getTile(pos);
    if(gUpTileModels[tile.up].passable) return true;
    return false;
}
inline bool World::posOccupied(const sf::Vector2i& pos) const {
    for(const auto& entity : mEntities){
        if(entity->getPosition().x == pos.x &&
           entity->getPosition().y == pos.y) return true;
    }
    return false;
}
bool World::posSpawnable(const sf::Vector2i& pos) const {
    return posPassable(pos) && !posOccupied(pos);
}

//word editing functions
sf::Vector2i World::getRandSpawnPosition() const {
    sf::Vector2i randPos;
    do{
        randPos = {rrange(mWidth - 1), rrange(mHeight - 1)};
    }while(!posSpawnable(randPos));
    return randPos;
}
inline const MultiTile& World::getTile(const sf::Vector2i& pos) const{
    return static_cast<const MultiTile&>(const_cast<World*>(this)->getTile(pos));
}
inline MultiTile& World::getTile(const sf::Vector2i& pos){
    assert(!posOutOfBounds(pos));
    return mTiles[pos.y * mWidth + pos.x];
}
bool World::fillRect(const sf::IntRect& rect, const std::function<void(MultiTile&)>& func){
    if(rectOutOfBounds(rect)) return false;
    for(int x = rect.left; x <= rect.left + rect.width; x++){
        for(int y = rect.top; y <= rect.top + rect.height; y++){
            func(getTile({x, y}));
        }
    }
    return true;
}

//entity handling functions
bool World::spawnEntity(Entity* entity){
    if(Entity::world->posOutOfBounds(entity->getPosition())) return false;
    mEntities.push_back(entity);
    return true;
}
void World::handleEvent(const sf::Event& event){
    for(auto& entity : mEntities){
        entity->handleEvent(event);
    }
}
void World::handleLogic(const sf::Time& time){
    bool isDone = true;
    for(auto& entity : mEntities){
        entity->handleLogic(time);
        isDone &= entity->isDone();
    }
    mIsDone = isDone;
}

//graphic functions
void World::updateVerticesAtPositions(const std::vector<sf::Vector2i>& positions) const {
    mNeedToUpdateVertices = true;
    if(!mNeedToUpdateAllVertices){
        for(const auto& pos : positions){
            assert(!posOutOfBounds(pos));
            mPositionsToUpdate[pos.y * mWidth + pos.x] = true;
        }
    }
}
void World::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(mNeedToUpdateVertices){
        //update vertices if needed
        for(int x = 0; x < mWidth; x++){
            for(int y = 0; y < mHeight; y++){
                if(mNeedToUpdateAllVertices || mPositionsToUpdate[y * mWidth + x]){
                    //determine which tile to show
                    auto tile = getTile({x, y});
                    sf::IntRect groundTR;
                    if(tile.up == UpTile::Air){
                        groundTR = sf::IntRect(gTerrainTileModels[tile.terrain].info.textureRect);
                    }
                    else{
                        groundTR = sf::IntRect(gUpTileModels[tile.up].info.textureRect);
                    }
                    //add up/terrain vertices
                    updateQuadAtPosition(sf::Vector2i(x, y), groundTR, mGroundVertices);
                    //add area vertices
                    updateQuadAtPosition(sf::Vector2i(x, y), gUpTileModels[tile.area].info.textureRect, mAreaVertices);
                }
            }
        }
        //render entities
        for(const auto& entity : mEntities){
            const auto& pos = entity->getPosition();
            if(mNeedToUpdateAllVertices || mPositionsToUpdate[pos.y * mWidth + pos.x])
                updateQuadAtPosition(pos, entity->getTextureRect(), mEntityVertices);
        }
    }
    //apply transform
    states.transform = getTransform();
    //set texture
    states.texture = mTileSheetTexture;
    //reset positions to update
    mNeedToUpdateVertices = false;
    mNeedToUpdateAllVertices = false;
    for(auto q = mPositionsToUpdate.begin(); q != mPositionsToUpdate.end(); ++q)
        *q = false;
    //render vertices
    target.draw(mGroundVertices, states);
    //target.draw(mAreaVertices, states);
    target.draw(mEntityVertices, states);
}

void World::updateQuadAtPosition(const sf::Vector2i& pos, const sf::IntRect& textureRect, sf::VertexArray& vertices) const {
    auto index = (pos.y * mWidth + pos.x) * 4;
    sf::FloatRect rr = {pos.x * World::TILE_WIDTH,
                        pos.y * World::TILE_HEIGHT,
                        World::TILE_WIDTH,
                        World::TILE_HEIGHT};
    const auto& tr = textureRect;
    //add up/terrain vertices
    vertices[index]     = sf::Vertex({rr.left, rr.top},                        {tr.left, tr.top});
    vertices[index + 1] = sf::Vertex({rr.left + rr.width, rr.top},             {tr.left + tr.width, tr.top});
    vertices[index + 2] = sf::Vertex({rr.left + rr.width, rr.top + rr.height}, {tr.left + tr.width, tr.top + tr.height});
    vertices[index + 3] = sf::Vertex({rr.left, rr.top + rr.height},            {tr.left, tr.top + tr.height});
}
