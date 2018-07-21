#ifndef SHIPS_H
#define SHIPS_H

#include "Ship.h"
#include "ShipContr.h"

///USER SHIP class
class UserShip: public UserSFC{
    public:
        //constructor
        UserShip(std::string name,Person *captain,int deckHp,int bridgeHp,int deckSize,int agility,int cost):UserSFC(name,captain,deckHp,bridgeHp,deckSize,agility,cost){}
};

///EXPLORER SHIP class
class ExplorerShip: public DummySFC{
    public:
        //constructor
        ExplorerShip();
};

///PIRATE SHIP class
class PirateShip: public DummySFC{
    public:
        //constructor
        PirateShip();
};

///ROYAL ARMY SHIP class
class RoyalArmyShip: public DummySFC{
    public:
        //constructor
        RoyalArmyShip();
};

///MERCENARY SHIP class
class MercenaryShip: public DummySFC{
    public:
        //constructor
        MercenaryShip();
};


///ROYAL WARSHIP class
class RoyalWarShip: public DummySFC{
    public:
        //constructor
        RoyalWarShip();
};


///PIRATE MOTHERSHIP class
class PirateMotherShip: public DummySFC{
    public:
        //constructor
        PirateMotherShip();
};
#endif
