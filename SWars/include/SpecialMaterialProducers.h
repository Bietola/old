#ifndef SPECIALMATERIALPRODUCERS_H
#define SPECIALMATERIALPRODUCERS_H

#include "globals.h"
#include "Producer.h"
#include "Sharer.h"

///Vincent class
class Vincent:public MaterialProducer,public MaterialSharer{
    public:
        //constructors
        Vincent();
        Vincent(int x,int y);
        //virtual functions fulfillment
        Thing *retClone(){return new Vincent(*this);}
        void think();
        void act();
};

#endif
