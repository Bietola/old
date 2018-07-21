#include "AnimationSprite.h"

///animation sprite functions
//constructor
AnimationSprite::AnimationSprite(Animation *animation,SDL_Rect showBox):Sprite(showBox){
    start(animation);
}
//initialization functions
void AnimationSprite::start(Animation *anim){
    animation=anim;
}
//show the animation through the show box (stretch it if necessary)
void AnimationSprite::show(){
    animation->show(&showBox);
}
