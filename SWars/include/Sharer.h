#ifndef SHARER_H
#define SHARER_H

#include "Thing.h"
#include "Users.h"

///forward declarations
class Network;

///sharer class
class Sharer:public virtual Thing{
    public:
        //friend classes
        friend class Network;
        friend class NetworkBuilder;
        friend class NetworkMultiBuilder;
        //enumerations
        enum SHARERTYPE{
            MATERIALSHARER,
            ENERGYSHARER,
        };
        //copy constructor
        Sharer(Sharer &sharer);
        //virtual destructor
        virtual ~Sharer();
        //getters
        double getShared(){return shared;}
        SHARERTYPE getSharerType(){return sharerType;}
        //functions
        bool share(double stuff);
        bool share();
        double retrieveShared(double stuff);
        double retrieveShared();
        //pure virtual functions
        virtual double getSharedVar()=0;
        virtual bool setSharedVar(double stuff)=0;
        //functions that rely on pure virtual functions
        bool changeSharedVar(double delta){return setSharedVar(getSharedVar()+delta);}
    protected:
        //network
        Network *network;
        //type of the sharer
        SHARERTYPE sharerType;
        //id in the network
        int netId;
        //shared amount of stuff
        double shared;
        //virtual inheritance constructor
        Sharer(Network *net=NULL);
        //initialization function
        void start(Network *net);
};

///material sharer
class MaterialSharer:public virtual Sharer,public virtual MaterialUser{
    public:
        //constructor
        MaterialSharer(Network *net=NULL):Sharer(net){sharerType=MATERIALSHARER;}
        //virtual functions fulfillment
        double getSharedVar();
        bool setSharedVar(double stuff);
};

///energy sharer
class EnergySharer:public virtual Sharer,public virtual EnergyUser{
    public:
        //constructor
        EnergySharer(Network *net=NULL):Sharer(net){sharerType=ENERGYSHARER;}
        //virtual functions fulfillment
        double getSharedVar();
        bool setSharedVar(double stuff);
};

#endif
