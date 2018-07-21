#ifndef SHIP_H
#define SHIP_H

#include <string>
#include <sstream>
#include <vector>

#include "Thing.h"
#include "ShipBuilding.h"

#define INV_X_BUILDINGS 1
#define INV_X_CREW INV_X_BUILDINGS+33

///SHIP class
class Ship: public Merchandise{
    public:
        //constants
        static const int BRIDGE=0;
        static const int DECK=1;
        static const int BAY=2;
        static const int MAXSP=3;
        static const int CAPTAIN=0;
        static const int FIRSTMATE=1;
        //constructor
        Ship(std::string name,Person *captain,int deckHp,int bridgeHp,int deckSize,int agility,int cost);
        //destructor
        ~Ship(){}
        //static functions
        static int battle(const char *intro,Ship *s1,Ship *s2);
        //checkers
        bool canBeTarget();
        bool hasFirstMate(){return (buildings[BRIDGE]->crew.size()==1 ? false : true);}
        bool isSank(){return buildings[DECK]->isBroken();}
        //getters
        std::string getName(){return name;}
        int getDeckSize(){return deckSize;}
        int getAgility(){return agility;}
        int getFood(){return getBay()->food;}
        int getCannonballs(){return getBay()->cannonballs;}
        Deck *getDeck(){return deck;}
        Bridge *getBridge(){return bridge;}
        Bay *getBay(){return bay;}
        std::vector<Person*> getCrew(){return getDeck()->crew;}
        Person *getCrew(int i){return getDeck()->crew[i];}
        Person *getCaptain(){return getBridge()->crew[CAPTAIN];}
        Person *getFirstMate(){return (hasFirstMate() ? buildings[BRIDGE]->crew[FIRSTMATE] : NULL);}
        ShipBuilding *randBuilding();
        Person *randPerson();
        //setters
        void setFood(int food){getBay()->food=std::max(0,food);}
        void setCannonballs(int cballs){getBay()->cannonballs=std::max(0,cballs);}
        void changeFood(int food){setFood(getBay()->food+food);}
        void changeCannonballs(int cballs){setCannonballs(getBay()->cannonballs+cballs);}
        void toggleAutoFire(){autofire ? autofire=false : autofire=true;}
        //pure virtual functions
        virtual void fight(Ship *ene)=0;
        //functions
        void printInventory();
        bool addBuilding(ShipBuilding *building);
        bool addItem(Item *item);
        bool addPerson(Person *person);
        bool addCaptain(Person *person);
        bool addFirstMate(Person *person);
        void chargeCannons();
        void pauseCannons();
        void restartCannons();
        void fire(Ship *ene);
        void evade(Shot &shot,int rain);
        void update();
        void updateCrew();
        //virtual functions fulfillment
        const char *getMerchantString();
        Thing::THINGTYPE getThingType(){return Thing::SHIP;}
        //variables
        std::vector<Cannon*> cannons;
        std::vector<Sail*> sails;
        const char *intro;
    protected:
        //ids
        int nextCannonId;
        int nextSailId;
        int nextSpecialId;
        //other
        bool autofire;
        std::vector<ShipBuilding*> buildings;
        Bridge *bridge;
        Deck *deck;
        Bay *bay;
        std::string name;
        int deckSize;
        int occSize;
        int agility;
        Comm *comlog,*batlog,*enelog;
};

#endif
