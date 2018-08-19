#ifndef DIALOGHANDLER_H_INCLUDED
#define DIALOGHANDLER_H_INCLUDED

#include <TGUI/tgui.hpp>

#include "Handler.h"
#include "MatchHandler.h"
#include "World.h"
#include "Entity.h"

///DialogHandler
class DialogHandler : public Handler {
    public:
        ///constructor
        DialogHandler(sf::RenderWindow& window, World& world, Entity& talker, Entity& listener);
        ///ui functions
        //handle user event
        void handleEvent(const sf::Event& e) override;
        //handle logical operations
        void handleLogic(const sf::Time& time) override;
        //draw interface
        void handleDrawing() override;
    private:
        ///variables
        Entity* mTalker;
        Entity* mListener;
        World* mWorld;
        tgui::Gui mDialogInterface;
        DialogIterator mCurrentDialog;
        ///functions
        //update conversation
        void updateConversation();
};

#endif // DIALOGHANDLER_H_INCLUDED
