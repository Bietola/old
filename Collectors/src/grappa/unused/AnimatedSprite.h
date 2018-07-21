#ifndef ANIMATEDSPRITE_H
#define ANIMATEDSPRITE_H

#include <vector>

#include "Sprite.h"
#include "Timer.h"

///animated texture class
class AnimatedSprite: public Sprite{
    public:
        //use previous functions
        using Sprite::show;
        //constructor
        AnimatedSprite(Texture *texture,SDL_Rect showBox,SDL_Rect frameBox,double animationSpeed,int frameNumber);
        //getters
        double getAnimationSpeed(){return animationSpeed;}
        int getFrameNumber(){return frameNumber;}
        SDL_Rect getFrameBox(){return frameBox;}
        //functions override
        void show();
    private:
        //initialization function
        void start(SDL_Rect fBox,double animeSpeed,int frameNum);
        //animation speed [FPS]
        double animationSpeed;
        //number of frames of which the animation is composed
        int frameNumber;
        //rectangle containing the dimensions of a single frame of the animation
        SDL_Rect frameBox;
        //animation timer
        Timer animationTimer;
};

#endif
