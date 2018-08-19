#ifndef SPECIALMOVE_H_INCLUDED
#define SPECIALMOVE_H_INCLUDED

#include <cassert>
#include <vector>
#include <exception>

#include "Describable.h"
#include "Status.h"
#include "Effect.h"
#include "actions.h"

class Entity;

///SpecialMove base class
class SpecialMove : public Describable{
    public:
        using Describable::Describable;
        SpecialMove(const std::string& name, const std::string& description):
                        Describable(name, description){}
        ~SpecialMove(){}
        void apply(Entity& attacker, Entity& defender) const;
        void addAction(eact::Action* action) {assert(action);mActions.push_back(action);}
    private:
        std::vector<eact::Action*> mActions;
};

#endif // SPECIALMOVE_H_INCLUDED
