#ifndef BGRESOURCECONTAINER_H
#define BGRESOURCECONTAINER_H

#include "Resource.h"
#include "ResourceContainer.h"

///background resource container class
class BGResourceContainer:public ResourceContainer{
    public:
        //constructor
        BGResourceContainer(int x,int y);
        BGResourceContainer(int x);
        //virtual function fulfillment
        Thing *retClone(){return new BGResourceContainer(*this);}
        void ccheck();
        void act();
        void think();
        //function in act
        void handleCollisions();
        //overridden functions
        void addResource(Resource r);
        void addResource(Resource::TYPE t,int a);
        //other functions
        void adaptTextureToResources();
};

#endif
