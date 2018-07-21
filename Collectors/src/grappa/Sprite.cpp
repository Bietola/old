#include "Sprite.h"

///Sprite functions
//constructor
Sprite::Sprite(SDL_Rect showBox){
    start(showBox);
}
//initialization function
void Sprite::start(SDL_Rect sBox){
    showBox=sBox;
}
