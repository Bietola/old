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

#endif // ACTIONS_H_INCLUDED
