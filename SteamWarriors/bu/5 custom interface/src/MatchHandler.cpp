#include "MatchHandler.h"

///MatchHandler functions
//constructor
MatchHandler::MatchHandler(sf::RenderWindow& window, World* world):
    Handler(window),
    mWorld(world)
{
    ;
}
//ui functions
void MatchHandler::handleEvent(const sf::Event& e)
{
    if(mWorldReadyForInput){
        mWorld->handleEvent(e);
    }
}
void MatchHandler::handleLogic(const sf::Time& time)
{
    mWorld->handleTimeLogic(time);
    mWorld->handleTurnLogic();
    mWorldReadyForInput = mWorld->isReadyForInput();
}
void MatchHandler::handleDrawing()
{
    //draw player view of world
    auto standardView = mWindow->getDefaultView();
    auto playerView = mWorld->getPlayer()->calcView();
    playerView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    mWindow->setView(playerView);
    mWindow->draw(*mWorld);
}
