#include "NetworkBuilder.h"

///network builder functions
//constructor
NetworkBuilder::NetworkBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,Thing *m,int cs,Network *net):Thing(x,y,t,ct,cf,hp,cdam),Builder(m,cs),Sharer(net){
    start();
}
//initialization function
void NetworkBuilder::start(){
    if(model->hasFlag(Thing::USES_NET)){
        network->addMember(dynamic_cast<Sharer*>(model));
    }
}
