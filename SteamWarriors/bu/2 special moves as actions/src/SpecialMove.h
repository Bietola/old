#ifndef SPECIALMOVE_H_INCLUDED
#define SPECIALMOVE_H_INCLUDED

#include <vector>
#include <exception>

#include "Describable.h"
#include "Status.h"
#include "Effect.h"
#include "actions.h"

class Entity;

namespace smov {

///SpecialMove base class
class SpecialMove : public Describable, public eact::Action {
    public:
        using Describable::Describable;
        SpecialMove(const std::string& name, const std::string& description,
                    Entity* attacker):
                        Describable(name, description),
                        mAttacker(attacker) {}
        virtual ~SpecialMove() {}
        void execute(Entity& entity) const override;
        virtual void specialExecute(Entity& defender) const = 0;
        virtual SpecialMove* clone() const override = 0;
        void setAttacker(Entity* attacker) {mAttacker = attacker;}
    protected:
        Entity* mAttacker = nullptr;
};

///Physical Attack class
class PhysicalAttack : public SpecialMove {
    public:
        using SpecialMove::SpecialMove;
        ~PhysicalAttack() {}
        void specialExecute(Entity& defender) const override;
        PhysicalAttack* clone() const override {return new PhysicalAttack(*this);}
};

///Poisonous Attack class
class PoisonousAttack : public SpecialMove {
    public:
        using SpecialMove::SpecialMove;
        ~PoisonousAttack() {}
        void specialExecute(Entity& defender) const override;
        PoisonousAttack* clone() const override {return new PoisonousAttack(*this);}
};

}

#endif // SPECIALMOVE_H_INCLUDED
