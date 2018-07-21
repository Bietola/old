#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>

#include "globals.h"

#include "Adv.h"

///adv functions
//constructors and deconstructors
Adv::Adv(){
    reset();
}
Adv::~Adv(){
    reset();
}
//handle event
void Adv::handleEvent(SDL_Event* e){
    //handle key presses
    if(e->type==SDL_KEYDOWN){
        //change direction of movement
        switch(e->key.keysym.sym){
            case SDLK_UP:
                xVel=0;
                yVel=-speed;
                break;
            case SDLK_RIGHT:
                xVel=speed;
                yVel=0;
                break;
            case SDLK_DOWN:
                xVel=0;
                yVel=speed;
                break;
            case SDLK_LEFT:
                xVel=-speed;
                yVel=0;
                break;
            case SDLK_SPACE:
                xVel=0;
                yVel=0;
                break;
        }
    }
}
//move adv according to direction, considering walls
void Adv::move(std::vector<SDL_Rect>* walls){
    //modify position
    xPos+=xVel;
    yPos+=yVel;
    cBox={xPos,yPos,t.getWidth(),t.getHeight()};
    //reset position if off screen
    if(xPos<0 || yPos<0 || xPos+t.getWidth()>SCREEN_W || yPos+t.getHeight()>SCREEN_H){
        if(xPos<0) xPos=0;
        if(yPos<0) yPos=0;
        if(xPos+t.getWidth()>SCREEN_W) xPos=SCREEN_W-t.getWidth();
        if(yPos+t.getHeight()>SCREEN_H) yPos=SCREEN_H-t.getHeight();
        xVel=0;
        yVel=0;
        cBox={xPos,yPos,t.getWidth(),t.getHeight()};
    }
    //reset position if collision
    if(walls!=NULL){
        for(int j=0;j<walls->size();j++){
            if(check_boxCollision(cBox,walls->at(j))){
                while(check_boxCollision(cBox,walls->at(j))){
                    if(xVel!=0) xPos-=xVel/speed;
                    if(yVel!=0) yPos-=yVel/speed;
                    cBox={xPos,yPos,t.getWidth(),t.getHeight()};
                }
                xVel=0;
                yVel=0;
            }
        }
    }
}
