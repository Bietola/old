#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED

#include <TGUI/TGUI.hpp>

class World;
class Entity;

namespace eact{

///base class
class Action{
    public:
        virtual ~Action() {}
        virtual Action* clone() const = 0;
        virtual void execute(Entity& entity) const = 0;
};

///walk to a given direction
class Walk: public Action{
    public:
        Walk(const sf::Vector2i& direction);
        void execute(Entity& entity) const override;
        Walk* clone() const {return new Walk(*this);}
    private:
        sf::Vector2i mDirection;
};

}

namespace eatt{

///base class
class AttackAction: public eact::Action {
    public:
        virtual ~AttackAction() {}
        virtual void execute(Entity& entity) const override;
        virtual void executeAttack(Entity& entity) const = 0;
        void setAttacker(Entity* attacker) {mAttacker = attacker;}
    protected:
        Entity* mAttacker = nullptr;
};

///Standard Physical Attack
class PhysicalAttack : public AttackAction {
    public:
        void executeAttack(Entity& entity) const override;
        PhysicalAttack* clone() const override {return new PhysicalAttack(*this);}
};

///Poisonous Attack
class PoisonousAttack : public AttackAction {
    public:
        void executeAttack(Entity& entity) const override;
        PoisonousAttack* clone() const override {return new PoisonousAttack(*this);}
};

}

#endif // ACTIONS_H_INCLUDED
