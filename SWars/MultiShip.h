#ifndef MULTISHIP_H
#define MULTISHIP_H

#include <vector>

#include "Ship.h"
#include "Orbiter.h"

///multi-ship class (orbitable movable entity)
class MultiShip:public Ship{
    public:
        //friend classes
        friend class Orbiter;
        //constructors
        MultiShip();
        MultiShip(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam);
        MultiShip(Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam);
        MultiShip(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam,std::vector<Orbiter*> os);
        MultiShip(Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam,std::vector<Orbiter*> os);
        //getters
        //!...
        //pseudo-setters
        //!...
        //other functions
        void move();
        //virtual function fulfillment
        Ship *retClone(){return new MultiShip(*this);}
    protected:
        //personal orbiters vector
        std::vector<Orbiter*> orbs;
        //virtual inheritance constructors
        MultiShip(MOVE mt,std::vector<Orbiter*> orbs);
        MultiShip(MOVE mt,int vx,int vy,std::vector<Orbiter*> orbs);
        //initialization functions
        void start();
        void start(std::vector<Orbiter*> os);
};

#endif
