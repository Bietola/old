#ifndef ANIMATIONSPRITE_H
#define ANIMATIONSPRITE_H

#include "Animation.h"
#include "Sprite.h"

///animation sprite class
class AnimationSprite: public Sprite{
    public:
        //constructor
        AnimationSprite(Animation *animation,SDL_Rect showBox);
        //virtual functions fulfillment
        void show();
    private:
        //animation
        Animation *animation;
        //initialization function
        void start(Animation *anim);
};


#endif // ANIMATIONSPRITE_H
