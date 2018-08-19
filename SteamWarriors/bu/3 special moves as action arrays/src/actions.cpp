#include "actions.h"

#include "World.h"
#include "Entity.h"

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
    //if new position is occupied by another entity, fight it or talk to it
    auto entityAtOccupiedSpot = Entity::world->getEntityAtPos(newPosition);
    if(entityAtOccupiedSpot){
        entity.attack(*entityAtOccupiedSpot);
    }
    //if position is not occupied and is passable, walk to it
    else if(Entity::world->posPassable(newPosition)){
        entity.setPosition(newPosition);
    }
    //if position is not passable send a "you can't go there" message
    else{
        Entity::messageBox->addText("you can't go there...\n");
    }
}

///base attack action class
void eatt::AttackAction::execute(Entity& entity) const {
    if(mAttacker)
        executeAttack(entity);
    else throw std::logic_error("calling special move on null attacker\n");
}

///physical attack
void eatt::PhysicalAttack::executeAttack(Entity& defender) const {
    defender.applyDamage(mAttacker->getStatus()["strength"]);
    Entity::messageBox->addText(mAttacker->getName() + " attacks " + defender.getName() + "\n");
}

///poisonous attack
void eatt::PoisonousAttack::executeAttack(Entity& defender) const {
    ;
}
