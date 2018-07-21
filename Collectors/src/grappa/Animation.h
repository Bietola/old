#ifndef ANIMATION_H
#define ANIMATION_H

#include "Texture.h"
#include "Timer.h"

///Animation class
class Animation{
    public:
        //constructor
        Animation(Texture *texture,SDL_Rect frameBox,double animationSpeed,int frameNumber);
        //getters
        double getAnimationSpeed(){return animationSpeed;}
        int getFrameNumber(){return frameNumber;}
        SDL_Rect getFrameBox(){return frameBox;}
        //other functions
        int update();
        void show(SDL_Rect *showRect);
        void show(int x,int y);
    private:
        //animation speed [FPS]
        double animationSpeed;
        //number of frames of which the animation is composed
        int frameNumber;
        //rectangle containing the dimensions of a single frame of the animation
        SDL_Rect frameBox;
        //animation timer
        Timer animationTimer;
        //texture containing all frames
        Texture *frameSheet;
        //initialization function
        void start(Texture *frmSheet,SDL_Rect fBox,double animeSpeed,int frameNum);
};


#endif // ANIMATION_H
