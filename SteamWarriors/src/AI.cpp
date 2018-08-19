#include "AI.h"

#include "Entity.h"

///Dummy
void eai::Dummy::think(Entity& entity) const {
    sf::Vector2i direction;
    direction.x = rrange(-1, 1);
    direction.y = rrange(-1, 1);
    if(direction != sf::Vector2i(0, 0))
        entity.addAction(new eact::Walk(direction));
}
