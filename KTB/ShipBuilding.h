#ifndef SHIPBUILDING_H
#define SHIPBUILDING_H

#include <sstream>

#include "bar.h"

#include "Timer.h"
#include "Thing.h"
#include "Person.h"
#include "Item.h"
#include "utils.h"

///forward declarations
class Ship;

///SHIP BUILDING base class
class ShipBuilding: public Storable{
    public:
        //enumerations
        enum TYPE{
            CANNON,
            SAIL,
            DECK,
            BRIDGE,
            BAY,
        };
        //friend classes
        friend class Ship;
        //constructor
        ShipBuilding(std::string name,int hp):name(name),hp(hp),maxHp(hp),personMax(0),selected(false){}
        ShipBuilding(std::string name,int hp,int pMax):name(name),hp(hp),maxHp(hp),personMax(pMax),selected(false){}
        //destructor
        virtual ~ShipBuilding();
        //getters
        int getId(){return id;}
        int getHp(){return hp;}
        int getMaxHp(){return maxHp;}
        ClrGra getSymbol(){return symbol;}
        std::string getSymId(){std::stringstream ss;ss<<symbol.gra<<id;return ss.str().c_str();}
        //checkers
        bool isSelected(){return selected;}
        bool isManned(){return (crew.size()>0 ? true : false);}
        bool isUsable(){return hp>0;}
        bool isBroken(){return !isUsable();}
        //setters
        void toggleSelect(){selected=(selected ? false : true);}
        //pure virtual functions
        virtual TYPE getType()=0;
        //virtual functions fulfillment
        virtual void invPrint(int x,int y);
        //other functions
        const char *getTypeName();
        virtual bool addPerson(Person *person);
        virtual void takeDamage(Shot shot);
        void free(int index);
        virtual void free();
        //variables
        std::vector<Person*> crew;
    protected:
        //variables
        ClrGra symbol;
        std::string name;
        bool selected;
        int size;
        int personMax;
        int maxHp,hp;
        int id;
};

/**SPECIAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
///DECK class
class Deck: public ShipBuilding{
    public:
        Deck(int hp,int pMax):ShipBuilding("deck",hp,pMax){size=0;}
        //virtual functions fulfillment
        TYPE getType(){return DECK;}
        bool addPerson(Person *person);
        void takeDamage(Shot shot);
};
///BRIDGE class
class Bridge: public ShipBuilding{
    public:
        Bridge(int hp):ShipBuilding("bridge",hp,2){size=0;}
        //virtual functions fulfillment
        TYPE getType(){return BRIDGE;}
        void free(){ShipBuilding::free(1);}
};
///BAY class
class Bay: public ShipBuilding,public ItemContainer{
    public:
        //friend classes
        friend class Ship;
        //constructor
        Bay(int hp,int itemMaxSize):ShipBuilding("bay",hp,0),ItemContainer(itemMaxSize),food(0),cannonballs(0){size=0;}
        //virtual functions fulfillment
        TYPE getType(){return BAY;}
    private:
        //variables
        int food;
        int cannonballs;
};

/**SAILS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
///SAIL base class
class Sail: public ShipBuilding{
    public:
        //constructor
        Sail(std::string name,int hp,int pMax):ShipBuilding(name,hp,pMax){}
        //virtual functions fulfillment
        TYPE getType(){return SAIL;}
        //pure virtual functions
        virtual void evade(Shot &shot,int rain)=0;
};

///BASIC SAIL class
class BasicSail: public Sail,public Merchandise{
    public:
        //constructor
        BasicSail(int level);
        //virtual functions fulfillment
        const char *getMerchantString();
        void evade(Shot &shot,int rain);
        Thing *clone(){return new BasicSail(*this);}
        Thing::THINGTYPE getThingType(){return Thing::SHIPBUILDING;}
    private:
        //variables
        int agility;
        int level;
        int protection;
};

/**CANNONS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
///CANNON base class
class Cannon: public ShipBuilding{
    public:
        //friend classes
        friend class Ship;
        //constructor
        Cannon(std::string name,int hp,int pMax,int chargeTime):ShipBuilding(name,hp,pMax),chargeTime(chargeTime){chargeTimer.reset();}
        //checkers
        bool isCharged(){return chargeTimer.getTicks()>=menChargeTime;}
        bool hasAmmo(){return true;}
        //virtual functions fulfillment
        TYPE getType(){return CANNON;}
        virtual void invPrint(int x,int y);
        //functions
        void shoot(Ship* ene,int rain,Comm *comlog=NULL,Comm *batlog=NULL,Comm *enelog=NULL);
        void recharge(/*!ammos*/);
    protected:
        //variables
        Timer chargeTimer;
        int chargeTime,menChargeTime;
        //pure virtual functions
        virtual Shot calcDamage(Ship *ene,int rain)=0;
        virtual void target(Ship *ene,Shot shot)=0;
};

///BASIC CANNON class
class BasicCannon: public Cannon,public Merchandise{
    public:
        //constructor
        BasicCannon(int level);
        //virtual functions fulfillment
        const char *getMerchantString();
        Shot shoot(Ship* ene);
        Thing *clone(){return new BasicCannon(*this);}
        Thing::THINGTYPE getThingType(){return Thing::SHIPBUILDING;}
    private:
        //variables
        int level;
        int power;
        int speed;
        int hp;
        //virtual functions fulfillment
        Shot calcDamage(Ship *ene,int rain);
        void target(Ship* ene,Shot shot);
};

/*!WIP///BEARDED CANNON class
class BeardedCannon: public Cannon{
    public:
        //constructor
        BeardedCannon(int level);
        //virtual functions fulfillment
        void shoot(Ship* eneShip);
    private:
        //variables
        Beard beard;
        Moustache moustache;
};*/

#endif
