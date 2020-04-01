#ifndef MCWORLD_H
#define MCWORLD_H

#include <vector>

#include "Interface.h"
#include "Texture.h"
#include "CThing.h"
#include "Orbiter.h"
#include "MultiShip.h"

///main controllable world class
class MCWorld: public MultiShip,public CThing{
    public:
        //constructors
        MCWorld(int x,int y,Texture *t,int hp,int cdam,std::vector<Orbiter*> os);
        //virtual functions fulfillment
        Ship *retClone(){return new MCWorld(*this);}
        void handleEvent(SDL_Event *e);
        void showRelativeToThis(Thing *thing);
        void showInterface();
};

#endif

