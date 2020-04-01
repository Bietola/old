#ifndef MCSHIP_H
#define MCSHIP_H

#include <cstddef>
#include <SDL.h>

#include "CThing.h"
#include "Ship.h"
#include "Cannon.h"
#include "NetworkMultiBuilder.h"
#include "MaterialProductMultiBuilder.h"
#include "Users.h"
#include "Interface.h"

///Main Controllable Ship class
class MCShip:public Ship,public Cannon,public NetworkMultiBuilder,public MaterialProductMultiBuilder,public MaterialSharer,public CThing{
    public:
        //constants
        static const int MAX_BUILDING=10;
        //constructors
        MCShip();
        MCShip(int x,int y,Texture *t,MOVE mt,SHOOT st,int hp,int cdam,int cspeed,int maxmat,Ship *proj,std::vector<Thing*> ms);
        //other functions
        void start();
        void start(std::vector<Network*> bnets);
        void move();
        //virtual functions fulfillment
        void repos(int x,int y){Ship::repos(x,y);Cannon::repos(x,y);MultiBuilder::repos(x,y);}
        Ship *retClone(){return new MCShip(*this);}
        void handleEvent(SDL_Event *e);
        void showInterface();
        void showRelativeToThis(Thing *thing);
        void think();
        void act();
    private:
        //last obtained input
        SDL_Event *lastE;
        //interface elements
        NumBox *lvBox,*buildSlotBox,*materialBox,*baseHpBox,*hpBox,*montyBox;
        TextureBox *buildTextureBox;
};

#endif
