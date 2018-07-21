#include "SpecialMaterialProducers.h"

///Vincent functions
//constructor
Vincent::Vincent():Thing(0,0,gTextures[TXTR_VINCENT],COLL_SHIP,CFACT_FRIEND,1,0),MaterialUser(0.0,100.0),MaterialSharer(NULL),MaterialProducer(10.0){}
Vincent::Vincent(int x,int y):Thing(x,y,gTextures[TXTR_VINCENT],COLL_SHIP,CFACT_FRIEND,1,0),MaterialUser(0.0,100.0),MaterialSharer(NULL),MaterialProducer(10.0){}//think
void Vincent::think(){}
void Vincent::act(){
    MaterialProducer::act();
    share();
}
