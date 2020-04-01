#include "NetworkMultiBuilder.h"

///network multi builder functions
//constructors
NetworkMultiBuilder::NetworkMultiBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,std::vector<Thing*> ms,int cs,std::vector<Network*> bnets):Thing(x,y,t,ct,cf,hp,cdam),MultiBuilder(cs,ms){start(bnets);}
NetworkMultiBuilder::NetworkMultiBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,int cs,std::vector<Network*> bnets):Thing(x,y,t,ct,cf,hp,cdam),MultiBuilder(cs){start(bnets);}
//virtual inheritance dedicated constructor
NetworkMultiBuilder::NetworkMultiBuilder(){start();}
NetworkMultiBuilder::NetworkMultiBuilder(std::vector<Network*> bnets){start(bnets);}
NetworkMultiBuilder::NetworkMultiBuilder(int cs,std::vector<Thing*> ms):MultiBuilder(cs,ms){start();}
NetworkMultiBuilder::NetworkMultiBuilder(int cs,std::vector<Thing*> ms,std::vector<Network*> bnets):MultiBuilder(cs,ms){start(bnets);}
//initialization functions
void NetworkMultiBuilder::start(std::vector<Network*> bnets){
    buildNetworks=bnets;
    start();
}
void NetworkMultiBuilder::start(){
    for(std::vector<Thing*>::iterator sq=models.begin();sq!=models.end();sq++){
        if((*sq)->hasFlag(Thing::USES_NET)){
            for(std::vector<Network*>::iterator nq=buildNetworks.begin();nq!=buildNetworks.end();nq++){
                if(dynamic_cast<Sharer*>(*sq)->getSharerType()==(*nq)->getNetworkType()) dynamic_cast<Sharer*>(*sq)->network=(*nq);
            }
        }
    }
}
//add an additional network to the building nets
void NetworkMultiBuilder::addBuildNetwork(Network *bnet){
    buildNetworks.push_back(bnet);
    start();
}
