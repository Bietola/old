#ifndef ACTIONS_H_INCLUDED
#define ACTIONS_H_INCLUDED

#include <TGUI/TGUI.hpp>

#include "Entity.h"
#include "World.h"

namespace eact{

///walk to a given direction
class Walk{
    public:
        Walk(const sf::Vector2i& direction);
        void operator()(Entity& entity);
    private:
        sf::Vector2i mDirection;
};

}

#endif // ACTIONS_H_INCLUDED
