#ifndef CANNON_H
#define CANNON_H

#include "Texture.h"
#include "Timer.h"
#include "globals.h"
#include "Thing.h"
#include "Ship.h"
#include "Builder.h"

///shooting type
enum SHOOT{
    SHOOT_DUMMY,
    SHOOT_NOTHINK,
    SHOOT_SPREAD,
    SHOOT_C_SLIDER,
    SHOOT_SPECIAL,
};

///cannon class
class Cannon:public virtual Thing,public Builder{
    public:
        //constructors
        Cannon();
        Cannon(SDL_Texture *t,COLLFACTION cf,SHOOT st,int hp,int cdam,double cs,Ship *proj);
        Cannon(int x,int y,SDL_Texture *t,COLLFACTION cf,SHOOT st,int hp,int cdam,double cs,Ship *proj);
        Cannon(Texture *t,COLLFACTION cf,SHOOT st,int hp,int cdam,double cs,Ship *proj);
        Cannon(int x,int y,Texture *t,COLLFACTION cf,SHOOT st,int hp,int cdam,double cs,Ship *proj);
        //copy constructor
        Cannon(Cannon &cannon);
        //destructors
        virtual ~Cannon();
        //virtual function fulfillment
        virtual Thing *retClone(){return new Cannon(*this);}
        void think();
        void act();
    protected:
        //ship virtual inheritance dedicated constructors
        Cannon(Ship *proj,double cspeed,SHOOT st);
        Cannon(Ship *proj,int msx,int msy,double cspeed,SHOOT st);
        //builder virtual inheritance constructors
        Cannon(Ship *proj,SHOOT st);
        //initialization functions
        void start(Ship *proj,SHOOT st);
        //specified ship model
        Ship *projectile;
        //shoot type
        SHOOT shootType;
};

#endif
