#ifndef NETWORK_H
#define NETWORK_H

#include <vector>

#include "Sharer.h"

///network class
class Network{
    public:
        //friend classes
        friend class Sharer;
        //static variables
        static std::vector<Network*> networks;
        static int maxId;
        static const int memExpand=10;
        static const int membersMemExpand=100;
        //constructor
        Network(Sharer::SHARERTYPE nt);
        //virtual destructor
        virtual ~Network();
        //getters
        Sharer::SHARERTYPE getNetworkType(){return networkType;}
        //pure virtual functions
        virtual void act()=0;
        //other functions
        void addMember(Sharer *member);
    protected:
        //members of the network
        std::vector<Sharer*> members;
        //net sharer-type (used for linking purposes)
        Sharer::SHARERTYPE networkType;
        //network id
        int id;
        //maximum member id
        int membersMaxId;
};

///boss network class
class BossNetwork:public Network{
    public:
        //constructor
        BossNetwork(Sharer *boss,Sharer::SHARERTYPE nt);
        //virtual functions fulfillment
        void act();
    private:
        //boss of the net
        Sharer *boss;
};

///even distribution network class
class EvenDistributionNetwork:public Network{
    public:
        //constructor
        EvenDistributionNetwork(Sharer::SHARERTYPE nt);
        //virtual functions fulfillment
        void act();
};

#endif
