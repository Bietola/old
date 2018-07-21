#ifndef NETWORKBUILDER_H
#define NETWORKBUILDER_H

#include "Builder.h"
#include "Network.h"
#include "Sharer.h"

///network builder class
class NetworkBuilder:public Builder,public Sharer{
    public:
        //constructor
        NetworkBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,Thing *m,int cs,Network *net);
    private:
        //initialization function
        void start();
};

#endif
