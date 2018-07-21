#ifndef NETWORKMULTIBUILDER_H
#define NETWORKMULTIBUILDER_H

#include "MultiBuilder.h"
#include "Network.h"
#include "Sharer.h"

///network multi builder class
class NetworkMultiBuilder:public virtual MultiBuilder{
    public:
        //constructors
        NetworkMultiBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,std::vector<Thing*> ms,int cs,std::vector<Network*> bnets);
        NetworkMultiBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,int cs,std::vector<Network*> bnets);
        //other functions
        void addBuildNetwork(Network *bnet);
    protected:
        //building networks
        std::vector<Network*> buildNetworks;
        //virtual inheritance dedicated constructors
        NetworkMultiBuilder();
        NetworkMultiBuilder(std::vector<Network*> bnets);
        NetworkMultiBuilder(int cs,std::vector<Thing*> ms);
        NetworkMultiBuilder(int cs,std::vector<Thing*> ms,std::vector<Network*> bnets);
        //initialization function
        void start();
        void start(std::vector<Network*> bnets);
};

#endif
