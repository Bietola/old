#ifndef PLACE_H
#define PLACE_H

#include <string>
#include <sstream>
#include <vector>

#include "names.h"
#include "utils.h"
#include "menu.h"

#include "Ship.h"
#include "ships.h"
#include "Merchant.h"

///forward declaration
class Player;

///PLACE base class
class Place{
    public:
        //place type
        enum TYPE{
            OPENSEA,
            ISLAND,
            STARTINGPLACE,
        };
        //destructor
        //!virtual ~Place();
        //pure virtual functions
        virtual void visit(Player *pl)=0;
        virtual TYPE getType()=0;
        virtual const char *getTypeName()=0;
        virtual const char *getDescription()=0;
        //getters
        std::string getName(){return name;}
    protected:
        //variables
        std::string name;
};

///PLACE MAP class
struct PlaceMap{
    //friend classes
    friend class Creature;
    //static variables
    static int maxID;
    //variables
    int id;
    Place *place;
    std::vector<PlaceMap*> paths;
    //constructor
    PlaceMap(int seed,int size);
    //recursive constructors
    PlaceMap(PlaceMap *back,int size);
};

///WORLD class
class World{
    public:
        //constructor
        World(int seed,int size);
        //getters
        PlaceMap *getHead(){return head;}
    private:
        //variables
        PlaceMap *head;
};

///STARTING PLACE class
class StartingPlace: public Place{
    public:
        //constructor
        StartingPlace();
        //virtual functions fulfillment
        void visit(Player *pl);
        TYPE getType(){return STARTINGPLACE;}
        const char *getTypeName(){return "starting point";}
        const char *getDescription();
    private:
        //variables
        bool visited;
        std::vector<Ship*> ships;
        //merchants
        CreepyShipMerchant shipMerchant;
        TownSlaveMerchant slaveMerchant;
        TownCannonMerchant cannonMerchant;
        TownSailMerchant sailMerchant;
        TownStuffMerchant stuffMerchant;
};

///OPEN SEA class
class OpenSea: public Place{
    public:
        //constructor
        OpenSea();
        //virtual functions fulfillment
        void visit(Player *pl);
        TYPE getType(){return OPENSEA;}
        const char *getTypeName(){return "open sea";}
        const char *getDescription();
    private:
        //variables
        int danger,luck;
};

///ISLAND class
class Island: public Place{
    public:
        //constructor
        Island();
        //virtual functions fulfillment
        void visit(Player *pl);
        TYPE getType(){return ISLAND;}
        const char *getTypeName(){return "island";}
        const char *getDescription();
};

#endif
