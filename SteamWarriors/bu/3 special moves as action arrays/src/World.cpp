#include "World.h"

//constants
const float World::TILE_WIDTH  = 32;
const float World::TILE_HEIGHT = 32;

//constructors
World::World(unsigned int width, unsigned int height, int seed):
    mWidth(width),
    mHeight(height),
    mTileSheetTexture(gTextureProvider.getMediaPtr("tiles"))
{
    //initialize vertex array
    mVertices.resize(mWidth * mHeight * 4);
    mVertices.setPrimitiveType(sf::Quads);
    updateAllVertices();
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
    unsigned int maxRocks = rrange(width * height / 100, width * height / 50);
    for(int j = 0; j < maxRocks; j++){
        sf::IntRect rockR;
        rockR.width   = rrange(1, width / 5);
        rockR.height  = rrange(1, height / 5);
        rockR.left    = std::max(0.f, rrange(width) - rockR.width);
        rockR.top     = std::max(0.f, rrange(height) - rockR.height);
        if(!fillRect(rockR, [](MultiTile& tile){
            tile.up = UpTile::Rock;
        }))
            j--;
    }
}

//word querying functions
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
Entity* World::getEntityAtPos(const sf::Vector2i& position) {
    for(auto entity : mEntities) {
        if(entity->getPosition() == position)
            return entity;
    }
    return nullptr;
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
void World::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(mNeedToUpdateVertices){
        //update vertices if needed
        for(int x = mUpdateArea.left; x < mUpdateArea.left + mUpdateArea.width; x++){
            for(int y = mUpdateArea.top; y < mUpdateArea.top + mUpdateArea.height; y++){
                //determine which tile to show
                auto tile = getTile({x, y});
                sf::IntRect tRect;
                if(tile.up == UpTile::Air){
                    tRect = sf::IntRect(gTerrainTileModels[tile.terrain].info.textureRect);
                }
                else{
                    tRect = sf::IntRect(gUpTileModels[tile.up].info.textureRect);
                }
                //add up/terrain vertices
                updateQuadAtPosition(sf::Vector2i(x, y), tRect);
            }
        }
    }
    //apply transform
    states.transform = getTransform();
    //set texture
    states.texture = mTileSheetTexture;
    //reset update flag
    mNeedToUpdateVertices = false;
    //render vertices
    target.draw(mVertices, states);
    //render entities
    for(const auto& entity : mEntities){
        const auto& entPos = entity->getPosition();
        sf::RectangleShape entSprite(sf::Vector2f(TILE_WIDTH, TILE_HEIGHT));
        entSprite.setPosition(sf::Vector2f(entPos.x * TILE_WIDTH, entPos.y * TILE_HEIGHT));
        entSprite.setTexture(&entity->getTexture());
        entSprite.setTextureRect(entity->getTextureRect());
        target.draw(entSprite);
    }
}

void World::updateQuadAtPosition(const sf::Vector2i& pos, const sf::IntRect& textureRect) const {
    auto index = (pos.y * mWidth + pos.x) * 4;
    sf::FloatRect rr = {pos.x * World::TILE_WIDTH,
                        pos.y * World::TILE_HEIGHT,
                        World::TILE_WIDTH,
                        World::TILE_HEIGHT};
    const auto& tr = textureRect;
    //add up/terrain vertices
    mVertices[index]     = sf::Vertex({rr.left, rr.top},                        {tr.left, tr.top});
    mVertices[index + 1] = sf::Vertex({rr.left + rr.width, rr.top},             {tr.left + tr.width, tr.top});
    mVertices[index + 2] = sf::Vertex({rr.left + rr.width, rr.top + rr.height}, {tr.left + tr.width, tr.top + tr.height});
    mVertices[index + 3] = sf::Vertex({rr.left, rr.top + rr.height},            {tr.left, tr.top + tr.height});
}
