#include "actions.h"

#include "World.h"
#include "Entity.h"
#include "DialogHandler.h"

///walk a given direction
eact::Walk::Walk(const sf::Vector2i& direction):
    mDirection(direction)
{}
void eact::Walk::execute(Entity& entity) const {
    //calculate new position
    auto oldPosition = entity.getPosition();
    auto newPosition = oldPosition;
    if(mDirection.x != 0) newPosition.x += mDirection.x / std::abs(mDirection.x);
    if(mDirection.y != 0) newPosition.y += mDirection.y / std::abs(mDirection.y);
    std::shared_ptr<Entity> entityAtOccupiedSpot;
    //if position is out of bounds, print an error
    if(Entity::world->posOutOfBounds(newPosition)) {
        //!WIP
        std::cout << entity.getName() << " tries to fly off the map\n";
    }
    //if position is occupied, attack the entity occupying it
    else if(entityAtOccupiedSpot = Entity::world->getEntityAtPos(newPosition).lock()) {
        //if new position is occupied by another entity, fight it or talk to it
        if(entity.getFaction() != entityAtOccupiedSpot->getFaction())
            entity.attack(*entityAtOccupiedSpot);
        else if(entityAtOccupiedSpot->canTalk() && entity.isPlayer())
            //!DIRTY: converting shared pointer to raw pointer through reference parameter
            gGameState->setNext(new DialogHandler(gWindow, *Entity::world, *entityAtOccupiedSpot, entity));
        else
            std::cout<< entity.getName() << " awkwardly bumps into his friend, " << entityAtOccupiedSpot->getName() << "\n";
    }
    //if position is not occupied and is passable, walk to it
    else if(Entity::world->posPassable(newPosition)) {
        entity.setPosition(newPosition);
    }
    //if position is not passable send a "you can't go there" message
    else {
        //!WIP
        std::cout << "someone bumps against something...\n";
    }
}
