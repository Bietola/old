#include "AnimatedSprite.h"

///Animated Sprite functions
//constructors
AnimatedSprite::AnimatedSprite(Texture *texture,SDL_Rect showBox,SDL_Rect frameBox,double animationSpeed,int frameNumber):Sprite(texture,showBox){
    start(frameBox,animationSpeed,frameNumber);
}
//initialization function
void AnimatedSprite::start(SDL_Rect fBox,double animeSpeed,int frameNum){
    animationSpeed=animeSpeed;
    frameNumber=frameNum;
    frameBox=fBox;
    animationTimer.start();
}
//show override from sprite class - show only the current frame
void AnimatedSprite::show(){
    int curFrameIndex=animationSpeed*animationTimer.getSeconds();
    int slctx=frameBox.x+(frameBox.w*curFrameIndex);
    int maxWidth=frameBox.w*frameNumber;
    if(curFrameIndex>=frameNumber){
        while(curFrameIndex>=frameNumber) curFrameIndex-=frameNumber;
        animationTimer.setSecondsReset(curFrameIndex/animationSpeed);
    }
    texture->show(new SDL_Rect{frameBox.x+(frameBox.w*curFrameIndex),frameBox.y,frameBox.w,frameBox.h},&showBox);
}
