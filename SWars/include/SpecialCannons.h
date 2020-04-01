#ifndef SPECIALCANNONS_H
#define SPECIALCANNONS_H

#include "Cannon.h"
#include "SpecialShips.h"
#include "MaterialProduct.h"

/// FRIENDS ///
///bob cannon class
class BobCannon:public Cannon,public MaterialProduct{
    public:
        //constructors
        BobCannon();
        BobCannon(int x,int y);
        //virtual functions fulfillment
        Thing *retClone(){return new BobCannon(*this);}
};
///mitra-bob cannon class
class MitraBobCannon:public Cannon,public MaterialProduct{
    public:
        //constructor
        MitraBobCannon();
        MitraBobCannon(int x,int y);
};
///din cannon class
class HenryCannon:public Cannon,public MaterialProduct{
    public:
        //constructors
        HenryCannon();
        HenryCannon(int x,int y);
        //virtual functions fulfillment
        HenryCannon *retClone(){return new HenryCannon(*this);}
    private:
        //initialization functions
        void start();
};
///astra cannon class
class AstraCannon:public Cannon,public MaterialProduct{
    public:
        AstraCannon();
        AstraCannon(int x,int y);
        //virtual functions fulfillment
        Thing *retClone(){return new AstraCannon(*this);}
    private:
        //initialization functions
        void start();
};

#endif
