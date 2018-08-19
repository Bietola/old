#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "Entity.h"
#include "World.h"
#include "actions.h"

///Player class
class Player: public Entity{
    public:
        //constructor
        using Entity::Entity;
        //input handling
        void handleEvent(const sf::Event& event) override;
        //custom logic
        void customLogic() override;
        //calculates the player view from position and other stats
        sf::View calcView() const;
    private:
        ;
};


#endif // PLAYER_H_INCLUDED
