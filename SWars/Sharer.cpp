#include "Sharer.h"

#include "Network.h"

///sharer functions
//constructors
Sharer::Sharer(Network *net){start(net);}
//copy constructor
Sharer::Sharer(Sharer &sharer){
    *this=sharer;
    sharer.network->addMember(this);
}
//virtual destructor
Sharer::~Sharer(){
    network->members[netId]=NULL;
}
//initialization functions
void Sharer::start(Network *net){
    network=net;
    flags|=Thing::USES_NET;
}
//tell the network how much the sharer wants (positive value) or needs (negative value)
bool Sharer::share(double stuff){
    if(getSharedVar()-stuff<0){
        shared=getSharedVar();
        setSharedVar(0);
        return false;
    }
    changeSharedVar(-stuff);
    shared+=stuff;
    return true;
}
//share all that the sharer has to offer
bool Sharer::share(){
    shared=getSharedVar();
    setSharedVar(0);
}
//retrieve a given amount from the sharer
double Sharer::retrieveShared(double stuff){
    double retVal;
    if(shared-stuff<0){
        retVal=shared;
        shared=0;
    }
    else{
        shared-=stuff;
        retVal=stuff;
    }
    return retVal;
}
//retrieve all from the sharer
double Sharer::retrieveShared(){
    double retVal=shared;
    shared=0;
    return retVal;
}

///material sharer functions
//make the network know what it is sharing
double MaterialSharer::getSharedVar(){return materials;}
bool MaterialSharer::setSharedVar(double stuff){return setMaterials(stuff);}

///energy sharer functions
//make the network know what it is sharing
double EnergySharer::getSharedVar(){return energy;}
bool EnergySharer::setSharedVar(double stuff){return setEnergy(stuff);}
