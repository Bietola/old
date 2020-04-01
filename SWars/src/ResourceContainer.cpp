#include "ResourceContainer.h"

///resource container functions
//constructor
ResourceContainer::ResourceContainer(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam):Thing(x,y,t,ct,cf,hp,cdam){}
//add a Resource
void ResourceContainer::addResource(Resource r){
    //put resource in resources
    resources[r.getType()].addAmount(r.getAmount());
}
void ResourceContainer::addResource(Resource::TYPE t,int a){
    resources[t].addAmount(a);
}
