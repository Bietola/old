#include "Thing.h"

///static members definitions
std::vector<Thing*> Thing::things;
int Thing::maxThing=0;
int Thing::maxSpawnedThing=0;
const int Thing::memExpand=1000;

///functions definitions
//constructors
Thing::Thing(Texture *texture,int xPos,int yPos,COLLTYPE collisionType){
    start(texture,xPos,yPos,collisionType);
}
//destructor
Thing::~Thing(){
    ;
}
//initiation function
void Thing::start(Texture* t,int x,int y,COLLTYPE ct){
    //set ids
    spawnId=-1;
    id=maxThing;
    maxThing++;
    //collision type
    collisionType=ct;
    //collision box
    cBox.x=x;
    cBox.y=y;
    //texture
    setTexture(t);
}
//spawn on the screen (add thing to things)
void Thing::spawn(){
    //reallocate memory
    if(maxSpawnedThing==0) things.resize(memExpand);
    else if(maxSpawnedThing%memExpand==0) things.resize(memExpand*(maxSpawnedThing/memExpand));
    //search empty space
    for(int j=0;j<maxSpawnedThing;j++){
        if(things[j]==NULL){
            spawnId=j;
            things[j]=this;
            return;
        }
    }
    //spawn in new place
    things[maxSpawnedThing]=this;
    spawnId=maxSpawnedThing;
    maxSpawnedThing++;
}
//spawn on the screen (add thing to things)
void Thing::spawn(int x,int y){
    repos(x,y);
    spawn();
}
//add a copy of thing to things
void Thing::clone(){
    create(retClone());
}
//add a copy of thing to things
void Thing::clone(int x,int y){
    repos(x,y);
    create(retClone());
}
//remove thing from things
void Thing::despawn(){
    Thing *tmp=things.at(spawnId);
    things[spawnId]=NULL;
    //!std::cout << id << ";" << spawnId << " despawned" << std::endl;
    delete tmp;
}
//show on the screen
void Thing::show(){
    assert(texture!=NULL);
    texture->show(cBox.x,cBox.y);
}
//return the next available spawn id
int Thing::nextSpawnId(){
    for(int j=0;j<maxSpawnedThing;j++){
        if(things.at(j)==NULL) return j;
    }
    return maxSpawnedThing;
}
//check if a collision with another thing has happened
bool checkCollision(Thing* thing1,Thing* thing2){
    if(thing1==NULL || thing2==NULL) return false;
    SDL_Rect box1=thing1->cBox;
    SDL_Rect box2=thing2->cBox;
    //check y collision
    if(box1.y+box1.h>box2.y && box2.y+box2.h>box1.y){
        //check x collision
        if(box1.x+box1.w>box2.x && box2.x+box2.w>box1.x){
            return true; //collision has happened
        }
    }
    return false; //no collisions has happened
}
//set texture
void Thing::setTexture(Texture* txtr){
    //collision box
    if(txtr!=NULL) cBox={cBox.x,cBox.y,txtr->getWidth(),txtr->getHeight()};
    else cBox={cBox.x,cBox.y,0,0};
    //texture
    texture=txtr;
}
