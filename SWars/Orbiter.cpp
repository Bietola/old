#include "Orbiter.h"

#include "MultiShip.h"

///orbiter functions
//constructors dedicated to virtual inheritance
Orbiter::Orbiter(MultiShip *ms){
    attach(ms);
}
//attach orbiter to a MultiShip
void Orbiter::attach(MultiShip *ms){
    curMultiShip=ms;
    //!msId=ms->attachOrbiter(this);
}
//modified despawn: advert multiShip of despawn before despawning
void Orbiter::despawn(){
    curMultiShip->orbs[msId]=NULL;
    Thing::despawn();
}
