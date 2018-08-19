#include "Player.h"

///Player functions

//input handling
void Player::handleEvent(const sf::Event& event){
    //move player
    if(isDone()){
        if(event.type == sf::Event::KeyPressed){
            auto direction = sf::Vector2i(0, 0);
            if(event.key.code == sf::Keyboard::W){
                direction.y = -1;
            }
            else if(event.key.code == sf::Keyboard::S){
                direction.y = 1;
            }
            else if(event.key.code == sf::Keyboard::A){
                direction.x = -1;
            }
            else if(event.key.code == sf::Keyboard::D){
                direction.x = 1;
            }
            if(direction.x != 0 || direction.y != 0) addAction(new eact::Walk(direction));
        }
    }
}

//logic handling
void Player::customLogic(){
    ;
}

//other functions
sf::View Player::calcView() const {
    const auto& pos = getPosition();
    sf::View view(sf::Vector2f(pos.x * World::TILE_WIDTH + World::TILE_WIDTH/2,
                               pos.y * World::TILE_HEIGHT + World::TILE_HEIGHT/2),
                  sf::Vector2f(10 * World::TILE_WIDTH, 10 * World::TILE_HEIGHT));
    return view;
}
