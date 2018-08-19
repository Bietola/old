#ifndef MATCHHANDLER_H_INCLUDED
#define MATCHHANDLER_H_INCLUDED

#include "Handler.h"
#include "World.h"
#include "Entity.h"
#include "EntityMaker.h"

#include "Interface/TextBox.h"
#include "Interface/PictureBox.h"

///MatchHandler class
class MatchHandler: public Handler {
    public:
        ///constructor
        MatchHandler(sf::RenderWindow& window, World* world);
        ///ui functions
        //handle user event
        void handleEvent(const sf::Event& e) override;
        //handle logical operations
        void handleLogic(const sf::Time& time) override;
        //draw interface
        void handleDrawing() override;
    private:
        ///variables
        World* mWorld;
        bool mWorldReadyForInput = true;
};

#endif // MATCHHANDLER_H_INCLUDED
