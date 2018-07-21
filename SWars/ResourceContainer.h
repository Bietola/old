#ifndef RESOURCECONTAINER_H
#define RESOURCECONTAINER_H

#include <vector>

#include "Thing.h"
#include "Resource.h"

///resource container class
class ResourceContainer:public Thing{
    public:
        //friend classes
        friend class ResourceExtractor;
        //constructor
        ResourceContainer(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam);
        //functions
        void addResource(Resource r);
        void addResource(Resource::TYPE t,int a);
    protected:
        //resources
        Resource resources[Resource::MAX];
};

#endif
