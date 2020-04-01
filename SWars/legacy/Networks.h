#ifndef NETWORKS_H
#define NETWORKS_H

#include "Network.h"

///material network class
class MaterialNetwork:public Network{
    public:
        //virtual functions fulfillment
        void act();
    private:
        //net users
        Sharer *taker;
        std::vector<MaterialSharer*> givers;
};

///energy network class
//!WIP

#endif
