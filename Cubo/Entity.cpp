#include "Entity.h"

///Entity functions
//constructors and deconstructors
Entity::Entity(){
    reset();
}
Entity::~Entity(){
    reset();
}
//reset entity
void Entity::reset(){
    xPos=0;
    yPos=0;
    xVel=0;
    yVel=0;
    speed=0;
    cBox={0,0,0,0};
    t.reset();
}
//set the texture
void Entity::setTexture(Texture t){
    this->t=t;
}
//show adv on the screen
void Entity::show(){
    t.show(xPos,yPos);
}
