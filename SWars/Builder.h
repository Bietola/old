#ifndef BUILDER_H
#define BUILDER_H

#include <cstddef>

#include "Timer.h"
#include "Thing.h"

///BUILDER class
class Builder:public virtual Thing{
    public:
        //constructors
        Builder();
        Builder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,Thing *m,int msx,int msy,double cs);
        //copy constructor
        Builder(Builder &builder);
        //destructor(s) (virtual)
        virtual ~Builder();
        //pseudo-setters
        void startBuilding();
        void stopBuilding();
        void startBuildingNow();
        //getters
        bool isBuilding(){return canBuild;}
        //other functions
        virtual void build();
        void build(int x,int y);
        //virtual functions fulfillment
        void repos(int x,int y){Thing::repos(x,y);model->repos(x+modelSpawnXPos,y+modelSpawnYPos);}
        Thing *retClone(){return new Builder(*this);}
        void think();
        void act();
    protected:
        //virtual inheritance constructors
        Builder(Thing *m,double cs);
        Builder(Thing *m,int msx,int msy,double cs);
        //indicator of the building process
        bool canBuild;
        //minimum amount of time passed in between the building of clones [clone/1000cycles]
        double chargeSpeed;
        //model spawn point relative position
        int modelSpawnXPos,modelSpawnYPos;
        //model(s) to build
        Thing *model;
        //build timer(s)
        Timer buildTimer;
        //other functions
        void start(Thing *m,int msx,int msy,double cs);
};

#endif // BUILDER_H
