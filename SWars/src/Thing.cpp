#include "Thing.h"

///static members definitions
vector<Thing*> Thing::things;
int Thing::maxThing=0;
int Thing::maxSpawnedThing=0;
const int Thing::memExpand=1000;

///functions definitions
//constructors
Thing::Thing():Texture(){
    start(rrange(SCREEN_W),rrange(SCREEN_H),COLL_UNDEF,CFACT_UNDEF,rrange(1,10),rrange(1,3));
}
Thing::Thing(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam):Texture(t!=NULL ? t->sdlT() : NULL){
    start(x,y,ct,cf,hp,cdam);
}
Thing::Thing(Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam):Texture(t!=NULL ? t->sdlT() : NULL){
    start(0,0,ct,cf,hp,cdam);
}
//initiation function
void Thing::start(int x,int y,COLLISION ct,COLLFACTION cf,int hp,int cdam){
    //set ids
    spawnId=-1;
    id=maxThing;
    maxThing++;
    //set position
    repos(x,y);
    //collision box
    cBox={x,y,width,height};
    //collision type and faction
    collType=ct;
    collFaction=cf;
    //various stats
    this->hp=hp;
    crashDamage=cdam;
    //flags
    flags=0;
}
//change texture
void Thing::setTexture(Texture *t){
    set(t);
    cBox.w=t->getWidth();
    cBox.h=t->getHeight();
}
//spawn on the screen (add thing to things)
void Thing::spawn(){
    //reallocate memory
    if(maxSpawnedThing==0) things.resize(memExpand);
    else if(maxSpawnedThing%memExpand==0) things.resize(memExpand*(maxSpawnedThing/memExpand));
    //search empty space
    for(int j=0;j<maxSpawnedThing;j++){
        if(things.at(j)==NULL){
            spawnId=j;
            things[j]=this;
            return;
        }
    }
    //spawn in new place
    things.at(maxSpawnedThing)=this;
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
    cout<<id<<";"<<spawnId<<" despawned"<<endl;
    delete tmp;
}
//show on the screen
void Thing::show(){
    Texture::show(xPos,yPos);
}
//show relative to a field of view as big as the screen
void Thing::showRelativeToField(int fx,int fy){
    showRelativeToField(fx,fy,SCREEN_W,SCREEN_H);
}
//show relative to a field of view
void Thing::showRelativeToField(int fx,int fy,int fw,int fh){
    Texture::showRelativeToField(xPos,yPos,fx,fy,fw,fh);
}
/*
//show relative to a viewer
void Thing::show(Thing *viewer){
    int vx=viewer->xPos;
    /*int vy=viewer->yPos; (**)
    if(vx<SCREEN_W/2) showRelativeToField(0,BOARD_H-SCREEN_H);
    else if(vx>BOARD_W-SCREEN_W/2) showRelativeToField(BOARD_W-SCREEN_W,BOARD_H-SCREEN_H);
    else showRelativeToField(vx-SCREEN_W/2,BOARD_H-SCREEN_H);
}
*/
//check various stuff about the thing (ex: death)
void Thing::check(){
    //custom checks
    ccheck();
    //necessary checks
    //death
    if(hp<=0 && !(flags & Thing::INDESTRUCTIBLE)) despawn();
}
//think and act at the same time
void Thing::thkact(){
    think();
    if(things.at(spawnId)!=NULL) act();
}
//do everything
void Thing::live(){
    int spwId=spawnId;
    //!cout << "lived ,";
    think();
    //!cout << "thinked, ";
    if(things.at(spwId)!=NULL){
        act();
        //!cout << "acted, ";
        if(things.at(spwId)!=NULL){
            check();
            //!cout << "checked" << endl;
        }
    }
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
