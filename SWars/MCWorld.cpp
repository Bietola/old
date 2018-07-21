#include "MCWorld.h"

///main controllable world functions
//constructor
MCWorld::MCWorld(int x,int y,Texture *t,int hp,int cdam,std::vector<Orbiter*> os):Thing(x,y,t,COLL_UNDEF,CFACT_UNDEF,hp,cdam),MultiShip(MOVE_SPECIAL,os){}
//handle user input
void MCWorld::handleEvent(SDL_Event *e){
    ;
}
//show interface
void MCWorld::showInterface(){
    int dumNum=6;
    NumBox dumBox(255,255,255,2);
    dumBox.show(0,0,dumNum);
}
//show specified thing in relative to the MCShip
void MCWorld::showRelativeToThis(Thing *thing){
    int vx=xPos;
    /*int vy=yPos;*/
    if(vx<SCREEN_W/2) thing->showRelativeToField(0,BOARD_H-SCREEN_H);
    else if(vx>BOARD_W-SCREEN_W/2) thing->showRelativeToField(BOARD_W-SCREEN_W,BOARD_H-SCREEN_H);
    else thing->showRelativeToField(vx-SCREEN_W/2,BOARD_H-SCREEN_H);
}
