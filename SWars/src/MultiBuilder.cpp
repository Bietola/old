#include "MultiBuilder.h"

///MultiBuilder functions
//constructors
MultiBuilder::MultiBuilder():Builder(){
    start();
}
MultiBuilder::MultiBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,double cs):Thing(x,y,t,ct,cf,hp,cdam),Builder(NULL,cs){
    start();
}
MultiBuilder::MultiBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,vector<Thing*> ms,double cs):Thing(x,y,t,ct,cf,hp,cdam),Builder(NULL,cs){
    start(ms);
}
//N.I.D. constructors
MultiBuilder::MultiBuilder(double cs,vector<Thing*> ms):Builder(NULL,cs){
    start(ms);
}
MultiBuilder::MultiBuilder(double cs):Builder(NULL,cs){
    start();
}
//V.I.D. constructors
MultiBuilder::MultiBuilder(vector<Thing*> ms){
    start(ms);
}
//destructor(s)
MultiBuilder::~MultiBuilder(){
    ;
}
//set the model to be replaced with one from the models vector, whose index is specified
void MultiBuilder::setModel(unsigned int i){
    if(i>=0 && i<models.size()){
        model=models.at(i);
        curModelI=i;
    }
}
//set the model to be the next model in the models vector
void MultiBuilder::nextModel(){
    setModel(curModelI+1);
}
//set the model to be the previous model in the models vector
void MultiBuilder::prevModel(){
    setModel(curModelI-1);
}
//virtual functions fulfillment
void MultiBuilder::think(){
    ;
}
void MultiBuilder::act(){
    Builder::act();
}
//initialization functions
void MultiBuilder::start(){
    curModelI=0;
    model=models.at(0);
}
void MultiBuilder::start(vector<Thing*> ms){
    models=ms;
    for(int j=0;j<models.size();j++){
        models[j]->repos(xPos+modelSpawnXPos,yPos+modelSpawnYPos);
    }
    start();
}
