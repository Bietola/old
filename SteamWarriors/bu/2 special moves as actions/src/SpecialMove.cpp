#include "SpecialMove.h"

#include "Entity.h"

///Base class
void smov::SpecialMove::execute(Entity& entity) const {
    if(mAttacker)
        specialExecute(entity);
    else throw std::logic_error("calling special move on null attacker or defender");
}

///PhysicalAttack
void smov::PhysicalAttack::specialExecute(Entity& defender) const {
    defender.applyDamage(mAttacker->getStatus()["strength"]);
    Entity::messageBox->addText("defender attacker");
}

///PoisonousAttack
void smov::PoisonousAttack::specialExecute(Entity& defender) const {
    ;
}
