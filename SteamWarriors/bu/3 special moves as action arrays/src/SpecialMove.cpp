#include "SpecialMove.h"

#include "Entity.h"

///special move functions
void SpecialMove::apply(Entity& attacker, Entity& defender) const {
    for(auto action : mActions) {
        eatt::AttackAction* attAction;
        if(attAction = dynamic_cast<eatt::AttackAction*>(action)) {
            attAction->setAttacker(&attacker);
        }
        defender.addAction(action->clone());
    }
}
