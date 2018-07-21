#ifndef ORBITER_H
#define ORBITER_H

#include "Ship.h"

///forward declarations
class MultiShip;

///orbiter class
class Orbiter:public virtual Ship{
    public:
        //virtual functions replacement
        void despawn();
        //other functions
        void attach(MultiShip *ms);
    protected:
        //virtual inheritance constructors
        Orbiter(MultiShip *ms);
        //current MultiShip of the orbiter
        MultiShip *curMultiShip;
        //id inside the orbs vector of the current MultiShip
        int msId;
};

#endif
