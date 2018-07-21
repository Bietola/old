#ifndef SHIP_H
#define SHIP_H

#include "globals.h"
#include "Thing.h"

///error constants
//out of bounds error
#define OOBERR_NULL 0x0
#define OOBERR_PX 0x1
#define OOBERR_NX 0x2
#define OOBERR_PY 0x4
#define OOBERR_NY 0x8

///ship class (AI-controlled movable entity)
class Ship: public Thing{
    public:
        //constructors
        Ship(Texture *texture,int xPos,int yPos,COLLTYPE collisionType);
        //getters
        int getXvel(){return xVel;}
        int getYvel(){return yVel;}
        //pseudo-setters
        virtual void redir(int x,int y){xVel=x;yVel=y;}
        //pure virtual functions
        virtual bool handleCollision(Thing* collider)=0;
        //other functions
        void move();
        void move(float xVel,float yVel);
        bool handleCollisions();
        int outOfBounds();
        int velOutOfBounds();
        int velOutOfBounds(int velMult);
        int outOfBounds(int minx,int miny,int maxx,int maxy);
        int velOutOfBounds(int minx,int miny,int maxx,int maxy);
        int velOutOfBounds(int minx,int miny,int maxx,int maxy,int velMult);
        bool handleOutOfBounds();
        //virtual function fulfillment
        void act();
    protected:
        //initialization functions
        void start();
        //velocity (pixels per second)
        float xVel,yVel;
};

#endif
