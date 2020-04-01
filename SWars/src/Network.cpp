#include "Network.h"

///network static members
//all networks that exist and that will act in a game loop
std::vector<Network*> Network::networks;
int Network::maxId=0;

///network functions
//constructor
Network::Network(Sharer::SHARERTYPE nt){
    //set network type
    networkType=nt;
    //initialize network
    membersMaxId=0;
    //add network to networks
    //reallocate memory
    if(maxId==0) networks.resize(memExpand);
    else if(maxId%memExpand==0) networks.resize(memExpand*(maxId/memExpand));
    //search for empty space
    for(int j=0;j<maxId;j++){
        if(networks.at(j)==NULL){
            id=j;
            networks[j]=this;
            return;
        }
    }
    //if no empty spaces, spawn at next available space
    networks.at(maxId)=this;
    id=maxId;
    maxId++;
}
//virtual destructor
Network::~Network(){
    Network *tmp=networks.at(id);
    networks[id]=NULL;
    cout<<"NET "<<id<<" despawned"<<endl;
    delete tmp;
}
//add a sharer to the network
void Network::addMember(Sharer *mem){
    //connect to network
    mem->network=this;
    //reallocate memory
    if(membersMaxId==0) members.resize(membersMemExpand);
    else if(membersMaxId%membersMemExpand==0) members.resize(membersMemExpand*(membersMaxId/membersMemExpand));
    //search for empty space
    for(int j=0;j<membersMaxId;j++){
        if(members.at(j)==NULL){
            mem->netId=j;
            members[j]=mem;
            return;
        }
    }
    //if no empty spaces, spawn at next available space
    members.at(membersMaxId)=mem;
    mem->netId=membersMaxId;
    membersMaxId++;
}

///boss network functions
//constructor
BossNetwork::BossNetwork(Sharer *boss,Sharer::SHARERTYPE nt):Network(nt){
    addMember(boss);
    this->boss=boss;
}
//act
void BossNetwork::act(){
    double totStuff=0;
    for(std::vector<Sharer*>::iterator q=members.begin();q!=members.end();q++){
        Sharer *m=*q;
        if(m!=NULL && m!=boss && m->getShared()>0){
            totStuff+=m->retrieveShared();
        }
    }
    boss->changeSharedVar(totStuff);
}

///even distribution network functions
//constructor
EvenDistributionNetwork::EvenDistributionNetwork(Sharer::SHARERTYPE nt):Network(nt){
    ;
}
//act
void EvenDistributionNetwork::act(){
    //!TEST give all to the first member of the network
    double totStuff=0;
    for(std::vector<Sharer*>::iterator q=members.begin();q!=members.end();q++){
        Sharer *m=*q;
        if(m!=NULL && q!=members.begin() && m->getShared()>0){
            totStuff+=m->retrieveShared();
        }
    }
    if(!members.empty()) members[0]->changeSharedVar(totStuff);
}
