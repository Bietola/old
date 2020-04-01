#include "MaterialProductMultiBuilder.h"

///product multi-builder functions
//constructors
ProductMultiBuilder::ProductMultiBuilder():MultiBuilder(){}
ProductMultiBuilder::ProductMultiBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,vector<Thing*> ms,int cs):MultiBuilder(x,y,t,ct,cf,hp,cdam,ms,cs){}
ProductMultiBuilder::ProductMultiBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,int cs):MultiBuilder(x,y,t,ct,cf,hp,cdam,cs){}
//normal inheritance dedicated constructors
ProductMultiBuilder::ProductMultiBuilder(int cs,vector<Thing*> ms):MultiBuilder(cs,ms){}
ProductMultiBuilder::ProductMultiBuilder(int cs):MultiBuilder(cs){}
//virtual inheritance dedicated constructors
ProductMultiBuilder::ProductMultiBuilder(vector<Thing*> ms):MultiBuilder(ms){}
//overridden build() function (with new product payment functionality)
void ProductMultiBuilder::build(){
    //!BAD ARC: only works with material product because a generalized "Product" class does not exist for simplicity
    //!BAD ARC: uses dynamic_cast's downcast because of a missing generalized "Building" class and architecture, which should also substitute flags completely
    if(model->hasFlag(Thing::COSTS)){

    }
}
