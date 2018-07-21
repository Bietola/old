#include "BGResourceContainer.h"

#include "ResourceExtractor.h"

///background resource container functions
//constructor
BGResourceContainer::BGResourceContainer(int x,int y):ResourceContainer(x,y,NULL,COLL_BG,CFACT_UNDEF,0,0){
    flags|=INDESTRUCTIBLE;
    flags|=EXTRACTABLE;
}
BGResourceContainer::BGResourceContainer(int x):ResourceContainer(x,0,NULL,COLL_BG,CFACT_UNDEF,0,0){
    flags|=INDESTRUCTIBLE;
    flags|=EXTRACTABLE;
}
//adapt current texture with the available resources
void BGResourceContainer::adaptTextureToResources(){
    //change texture according to contained resources
    //find resource which is most present
    int maxId=0;
    bool empty=true;
    for(int j=0;j<Resource::MAX;j++){
        if(resources[j].getAmount()>0) empty=false;
        if(resources[j].getAmount()==resources[maxId].getAmount()) maxId=j;
    }
    //switch texture to the one correspondent to the max resource
    if(empty) despawn();
    else{
        int am=resources[maxId].getAmount();
        if(am<=100) am=0;
        else if(am<=250) am=1;
        else if(am<=500) am=2;
        else if(am<=750) am=3;
        else if(am<=1000) am=4;
        else if(am<=2000) am=5;
        else am=6;
        setTexture(Resource::bgTextures[resources[maxId].getType()][am]);
        reposy(BOARD_H-height);
    }
}
//add a Resource (with texture update)
void BGResourceContainer::addResource(Resource r){
    //put resource in resources
    ResourceContainer::addResource(r);
    //update texture if necessary
    adaptTextureToResources();
}
void BGResourceContainer::addResource(Resource::TYPE t,int a){
    ResourceContainer::addResource(t,a);
    adaptTextureToResources();

}
//check for collisions with resource extractors
void BGResourceContainer::handleCollisions(){
    for(int j=0;j<maxSpawnedThing;j++){
        //collision with resource extractor
        if(j!=spawnId && checkCollision(this,things[j]) && dynamic_cast<ResourceExtractor*>(things[j])){
            dynamic_cast<ResourceExtractor*>(things[j])->availableResourceContainers.push_back(this);
        }
    }
}
//empty think functions
void BGResourceContainer::think(){}
//act function
void BGResourceContainer::act(){
    //handle collisions with resource extractors
    handleCollisions();
}
//custom check function
void BGResourceContainer::ccheck(){
    adaptTextureToResources();
}
