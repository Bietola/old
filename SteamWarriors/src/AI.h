#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include <iostream>

#include <SFML/Graphics.hpp>

#include "actions.h"
#include "utils.h"

class Entity;

namespace eai {

///Entity AI base class
class EntityAI {
    public:
        //constructor
        EntityAI() {}
        //descturctor
        virtual ~EntityAI() {}
        //function called when entity needs to decide what to do
        virtual void think(Entity& entity) const = 0;
};

///Dummy AI - just moves randomly every turn
class Dummy: public EntityAI {
    public:
        //constructor
        using EntityAI::EntityAI;
        //vff
        void think(Entity& entity) const override;
};

}

#endif // AI_H_INCLUDED
