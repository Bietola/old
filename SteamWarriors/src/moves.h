#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED

#include <boost/optional.hpp>

#include "actions.h"

class Entity;

namespace smov {

///base class
class SpecialAction {
    public:
        //destructor
        virtual ~SpecialAction() {}
        //called on execute
        void execute(Entity& attacker, boost::optional<Entity&> defender) const;
        virtual void vExecute(Entity& attacker, boost::optional<Entity&> defender) const = 0;
        virtual bool needsDefender() const = 0;
        //clone
        virtual SpecialAction* clone() const = 0;
};

///basic physical attack based on strength
class PhysicalAttack : public SpecialAction {
    public:
        ~PhysicalAttack() {}
        void vExecute(Entity& attacker, boost::optional<Entity&> defender) const override;
        bool needsDefender() const override {return true;}
        PhysicalAttack* clone() const override {return new PhysicalAttack(*this);}
};

///weak strength based attack that inflicts poison
class PoisonousAttack : public SpecialAction {
    public:
        ~PoisonousAttack() {}
        void vExecute(Entity& attacker, boost::optional<Entity&> defender) const override;
        bool needsDefender() const override {return true;}
        PoisonousAttack* clone() const override {return new PoisonousAttack(*this);}
};

}

#endif // MOVES_H_INCLUDED
