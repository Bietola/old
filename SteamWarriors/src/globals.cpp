#include "globals.h"
#include "EntityMaker.h"
#include "MatchHandler.h"

///global window
sf::RenderWindow gWindow(sf::VideoMode(750,750),"Wars");
Handler* gGameState = nullptr;

///start game
void start_game() {
    //load assets
    assert(load_media());
    assert(SpecialMove::loadModels());
    assert(DialogTree::loadModels());
    assert(EntityMaker::loadModels());
    //generate world
    Entity::world = new World(100, 100, time(NULL));
    //spawn player
    auto player = EntityMaker::create("sword_man", true);
    player->setPosition(Entity::world->getRandSpawnPosition());
    assert(Entity::world->spawnPlayer(player));
    //spawn the tutorial guy
    auto tutorialGuy = EntityMaker::create("tutorial_guy");
    tutorialGuy->setPosition(sf::Vector2i(player->getPosition().x + 2, player->getPosition().y));
    assert(Entity::world->spawnEntity(tutorialGuy));
    //spawn some enemies
    auto dummy1 = EntityMaker::create("halo_mon");
    dummy1->setName("halo monster 1");
    dummy1->setPosition({Entity::world->getPlayer()->getPosition().x + 1, Entity::world->getPlayer()->getPosition().y});
    assert(Entity::world->spawnEntity(dummy1));
    auto dummy2 = EntityMaker::create("halo_mon");
    dummy2->setName("halo monster 2");
    dummy2->setPosition({Entity::world->getPlayer()->getPosition().x - 1, Entity::world->getPlayer()->getPosition().y});
    assert(Entity::world->spawnEntity(dummy2));
    auto dummy3 = EntityMaker::create("halo_mon");
    dummy3->setName("halo monster 3");
    dummy3->setPosition({Entity::world->getPlayer()->getPosition().x, Entity::world->getPlayer()->getPosition().y + 1});
    assert(Entity::world->spawnEntity(dummy3));
    auto dummy4 = EntityMaker::create("halo_mon");
    dummy4->setName("halo monster 4");
    dummy4->setPosition({Entity::world->getPlayer()->getPosition().x, Entity::world->getPlayer()->getPosition().y - 1});
    assert(Entity::world->spawnEntity(dummy4));
    //set up initial game state
    gGameState = new MatchHandler(gWindow, Entity::world);
}
