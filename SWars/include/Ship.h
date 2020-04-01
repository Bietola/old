#ifndef SHIP_H
#define SHIP_H

#include "globals.h"
#include "Thing.h"
#include "Timer.h"

///movement type
enum MOVE{
    MOVE_NOTHINK,
    MOVE_DUMMY,
    MOVE_DUMATTACKER,
    MOVE_HSLIDER,
    MOVE_C_SLIDER,
    MOVE_SPECIAL,
};

///ship class (AI-controlled movable entity)
class Ship:public virtual Thing{
    public:
        //constructors
        Ship();
        Ship(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam);
        Ship(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam,int vx,int vy);
        Ship(Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam);
        Ship(Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam,int vx,int vy);
        //getters
        int getXvel(){return xVel;}
        int getYvel(){return yVel;}
        //pseudo-setters
        virtual void redir(int x,int y){xVel=x;yVel=y;}
        //other functions
        virtual void move();
        void move(float xVel,float yVel);
        bool handleCollision();
        int outOfBounds();
        int velOutOfBounds();
        int velOutOfBounds(int velMult);
        int outOfBounds(int minx,int miny,int maxx,int maxy);
        int velOutOfBounds(int minx,int miny,int maxx,int maxy);
        int velOutOfBounds(int minx,int miny,int maxx,int maxy,int velMult);
        bool handleOutOfBounds();
        //virtual function fulfillment
        virtual Thing *retClone(){return new Ship(*this);}
        void think();
        void act();
    protected:
        //virtual inheritance constructors
        Ship(MOVE mt);
        Ship(MOVE mt,int vx,int vy);
        //initialization functions
        void start(MOVE mt,int vx,int vy);
        //velocity (pixels per second)
        float xVel,yVel;
        //movement type
        MOVE moveType;
};

#endif
