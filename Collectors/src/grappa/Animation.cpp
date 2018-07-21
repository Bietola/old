#include "Animation.h"

///Animated Sprite functions
//constructors
Animation::Animation(Texture *frameSheet,SDL_Rect frameBox,double animationSpeed,int frameNumber){
    start(frameSheet,frameBox,animationSpeed,frameNumber);
}
//initialization function
void Animation::start(Texture *frmSheet,SDL_Rect fBox,double animeSpeed,int frameNum){
    frameSheet=frmSheet;
    animationSpeed=animeSpeed;
    frameNumber=frameNum;
    frameBox=fBox;
    animationTimer.start();
}
//coordinate the current frame with the animation timer (update animation) - return the current frame index
int Animation::update(){
    int curFrameIndex=animationSpeed*animationTimer.getSeconds();
    int slctx=frameBox.x+(frameBox.w*curFrameIndex);
    int maxWidth=frameBox.w*frameNumber;
    if(curFrameIndex>=frameNumber){
        while(curFrameIndex>=frameNumber) curFrameIndex-=frameNumber;
        animationTimer.setSecondsReset(curFrameIndex/animationSpeed);
    }
    return curFrameIndex;
}
//show the animation by adapting it to a show rect (stretch it if necessary)
void Animation::show(SDL_Rect *showRect){
    int curFrameIndex=update();
    frameSheet->show(new SDL_Rect{frameBox.x+(frameBox.w*curFrameIndex),frameBox.y,frameBox.w,frameBox.h},showRect);
}
//show animation at given position
void Animation::show(int x,int y){
    int curFrameIndex=update();
    frameSheet->show(new SDL_Rect{frameBox.x+(frameBox.w*curFrameIndex),frameBox.y,frameBox.w,frameBox.h},NULL);
}
