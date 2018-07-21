#ifndef SPECIALBUILDINGS_H
#define SPECIALBUILDINGS_H

#include "Thing.h"
#include "MaterialProduct.h"
#include "GameStates.h"
#include "globals.h"

///spike trap class
class SpikeTrap:public MaterialProduct{
    public:
        //constructor
        SpikeTrap(int damage);
        SpikeTrap(Texture *t,int damage);
        //virtual functions fulfillment
        Thing *retClone(){return new SpikeTrap(*this);}
        void act(){}
        void think(){}
};

///rock temple class
class RockTemple:public MaterialProduct{
    public:
        //constructor
        RockTemple(Texture *t,int hp,int cost,int power);
        //virtual functions fulfillment
        Thing *retClone(){RockTemple tmp=*this;tmp.start();return new RockTemple(tmp);}
        void act(){}
        void think(){}
        void ccheck();
        //initialization functions
        void start();
    private:
        //monty rarity added at spawn
        int power;
};

///umbrella class
class Umbrella:public MaterialProduct{
    public:
        //constructor
        Umbrella(Texture *t,int hp,int cost);
        //virtual functions fulfillment
        Thing *retClone(){return new Umbrella(*this);}
        void act(){}
        void think(){}
};

#endif
