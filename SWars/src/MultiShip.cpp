#include "MultiShip.h"

//constructors
MultiShip::MultiShip():Ship(){
    ;
}
MultiShip::MultiShip(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam):Ship(x,y,t,ct,cf,mt,hp,cdam){start();}
MultiShip::MultiShip(Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam):Ship(t,ct,cf,mt,hp,cdam){start();}
MultiShip::MultiShip(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam,std::vector<Orbiter*> os):Ship(x,y,t,ct,cf,mt,hp,cdam){start(os);}
MultiShip::MultiShip(Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam,std::vector<Orbiter*> os):Ship(t,ct,cf,mt,hp,cdam){start(os);}
//virtual inheritance constructors
MultiShip::MultiShip(MOVE mt,int vx,int vy,std::vector<Orbiter*> os):Ship(mt,vx,vy){start(os);}
MultiShip::MultiShip(MOVE mt,std::vector<Orbiter*> os):Ship(mt){start(os);}
//initialization functions
void MultiShip::start(){
    ;
}
void MultiShip::start(std::vector<Orbiter*> os){
    orbs=os;
    start();
}
//modified move function (moves orbs along with MultiShip)
void MultiShip::move(){
    for(std::vector<Orbiter*>::iterator j=orbs.begin();j!=orbs.end();j++){
        (*j)->move(xVel,yVel);
    }
    Ship::move();
}
