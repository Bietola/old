#include "moves.h"

#include "Entity.h"

//execute
void smov::SpecialAction::execute(Entity& attacker, boost::optional<Entity&> defender) const {
    if(needsDefender() && !defender)
        std::cout << "ERROR! Could not perform special action without a specified defender!\n";
    else
        vExecute(attacker, defender);
}

void smov::PhysicalAttack::vExecute(Entity& attacker, boost::optional<Entity&> defender) const {
    defender->applyDamage(attacker.getStat("strength"));
    //!WIP
    std::cout << attacker.getName() << " attacks " << defender->getName() << std::endl;
}

void smov::PoisonousAttack::vExecute(Entity& attacker, boost::optional<Entity&> defender) const {
    ;
}
