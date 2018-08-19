#include "Handler.h"

///MatchHandler functions
//constructor
MatchHandler::MatchHandler(sf::RenderWindow& window, World* world):
    Handler(window),
    mWorld(world),
    mPlayer(nullptr),
    mPlayerInterface(window)
{
    //set up global world
    Entity::world = world;
    //spawn player
    mPlayer = new Player("player", "a player",
                         {0, 0},
                         gAnimationProvider.getMedia("player_idle"),
                         gAnimationProvider.getMedia("player_attack"),
                         Status{{"strength", 10}});
    mPlayer->setPosition(world->getRandSpawnPosition());
    assert(world->spawnEntity(mPlayer));
    //spawn some test guys
    auto dummy= new DummyEntity("dummy", "a dummy",
                                {0, 0},
                                gAnimationProvider.getMedia("halo_mon_idle"),
                                gAnimationProvider.getMedia("halo_mon_idle"),
                                Status{{"strength", 10}});
    dummy->setPosition({mPlayer->getPosition().x + 1, mPlayer->getPosition().y});
    assert(world->spawnEntity(dummy));
    ///initialize player interface
    //message box
    Entity::messageBox->setPosition(tgui::bindWidth(mPlayerInterface) / 2.f,
                                    3.f * tgui::bindHeight(mPlayerInterface) / 4.f);
    Entity::messageBox->setSize(tgui::bindWidth(mPlayerInterface) / 2.f,
                                tgui::bindHeight(mPlayerInterface) / 4.f);
    mPlayerInterface.add(Entity::messageBox, "message_box");
}
//ui functions
void MatchHandler::handleEvent(const sf::Event& e)
{
    if(mWorldDone){
        mPlayerInterface.handleEvent(e);
        mWorld->handleEvent(e);
    }
}
void MatchHandler::handleLogic(const sf::Time& time)
{
    mWorld->handleLogic(time);
    mWorldDone = mWorld->isDone();
}
void MatchHandler::handleDrawing()
{
    //draw player view of world
    auto standardView = mWindow->getDefaultView();
    auto playerView = mPlayer->calcView();
    playerView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    mWindow->setView(playerView);
    mWindow->draw(*mWorld);
    //draw player interface
    mWindow->setView(standardView);
    mPlayerInterface.draw();
}
