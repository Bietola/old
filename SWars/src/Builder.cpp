#include "Builder.h"

///builder functions
//constructors
Builder::Builder():Thing(){
    start(NULL,0,0,1);
}
Builder::Builder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,Thing *m,int msx,int msy,double cs):Thing(x,y,t,ct,cf,hp,cdam){
    start(m,msx,msy,cs);
}
//copy constructor
Builder::Builder(Builder &builder){
    *this=builder;
    model=builder.model->retClone();
    buildTimer.reset();
}
//destructor(s)
Builder::~Builder(){
    delete model;
    cout<<"destroying Builder"<<endl;
}
//inheritance dedicated constructors
Builder::Builder(Thing *m,double cs):Thing(){
    start(m,0,0,cs);
}
Builder::Builder(Thing *m,int msx,int msy,double cs):Thing(){
    start(m,msx,msy,cs);
}
//initialize builder
void Builder::start(Thing* m,int msx,int msy,double cs){
    modelSpawnXPos=msx;
    modelSpawnYPos=msy;
    model=m;
    if(model!=NULL) model->repos(xPos+msx,yPos+msy);
    chargeSpeed=cs;
    buildTimer.start();
    canBuild=true;
}
//start building process
void Builder::startBuilding(){
    canBuild=true;
    buildTimer.restart();
}
//stop building process
void Builder::stopBuilding(){
    canBuild=false;
    buildTimer.pause();
}
//start building process and advance build timer so that the builder will immediately build
void Builder::startBuildingNow(){
    startBuilding();
    buildTimer.addCycles(1000/chargeSpeed);
}
//build (or clone) model
void Builder::build(){
    model->clone();
    buildTimer.reset();
}
//build (or clone) model at given coos
void Builder::build(int x,int y){
    model->repos(x,y);
    build();
}
//virtual functions fulfillment
void Builder::think(){
    ;
}
void Builder::act(){
    if(buildTimer.getCycles()>=1000.0/chargeSpeed && canBuild){
        //check if building the model would cause a collision (any collision counts)
        if(!model->hasFlag(Thing::NO_SPACE_NEEDED_WHEN_BUILT)){
            for(int j=0;j<things.size();j++){
                //collision is caused - exit
                if(checkCollision(model,things[j]) && things[j]!=this && things[j]->getCollType()!=COLL_BG && !things[j]->hasFlag(Thing::NO_SPACE_NEEDED_WHEN_BUILT)) return;
            }
        }
        //no collision happened, clone the model normally
        build();
    }
}
