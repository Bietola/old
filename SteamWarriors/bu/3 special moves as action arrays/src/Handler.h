#ifndef HANDLERS_H_INCLUDED
#define HANDLERS_H_INCLUDED

#include <TGUI/TGUI.hpp>

#include "media.h"
#include "World.h"
#include "Player.h"
#include "DummyEntity.h"
#include "Animation.h"

///Handler base class
class Handler{
    public:
        ///constructor
        Handler(sf::RenderWindow& window): mWindow(&window) {}
        ///static functions
        //deallocate given handler and change it to point to the next one
        static void change(Handler*& handler);
        ///setters
        //sets next handler only if current next handler is null.
        //returns success.
        bool setNext(Handler* handler);
        ///interface functions
        //handle user event
        virtual void handleEvent(const sf::Event& e) = 0;
        //handle logical operations
        virtual void handleLogic(const sf::Time& time) = 0;
        //draw interface
        virtual void handleDrawing() = 0;
    protected:
        ///variables
        //window on which the interface is shown
        sf::RenderWindow* mWindow;
    private:
        ///static variables
        //next handler to be used
        static Handler* mNext;
};

///MatchHandler class
class MatchHandler: public Handler{
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
        bool mWorldDone = true;
        Player* mPlayer;
        tgui::Gui mPlayerInterface;
};

///EndScreenHandler class
class EndScreenHandler: public Handler{
    ;
};

#endif // HANDLERS_H_INCLUDED
