#include "SpecialBuildings.h"

///spike trap functions
//constructor
SpikeTrap::SpikeTrap(int damage):Thing(gTextures[TXTR_SPIKE_TRAP_1],COLL_PROJ,CFACT_FRIEND,1,damage),MaterialProduct(10.0){}
SpikeTrap::SpikeTrap(Texture *t,int damage):Thing(t,COLL_PROJ,CFACT_FRIEND,1,damage),MaterialProduct(10.0){}

///rock temple functions
//constructor
RockTemple::RockTemple(Texture *t,int hp,int cost,int power):Thing(t,COLL_SHIP,CFACT_FRIEND,hp,1),MaterialProduct(cost),power(power){}
//initialization functions
void RockTemple::start(){
    GameLoop::montyRarity+=power;
}
//ccheck
void RockTemple::ccheck(){
    if(hp<=0) GameLoop::montyRarity-=power;
}

///umbrella functions
//constructor
Umbrella::Umbrella(Texture *t,int hp,int cost):Thing(t,COLL_SHIP,CFACT_FRIEND,hp,1),MaterialProduct(cost){addFlag(Thing::NO_SPACE_NEEDED_WHEN_BUILT);}
