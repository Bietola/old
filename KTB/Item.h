#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <sstream>

#include <curses.h>

#include "palette.h"

#include "Thing.h"

///SHOT class
struct Shot{
    //constants
    static const int ONEHIT=-1;
    //enumerations
    enum TYPE{
        METAL,
        EXPLOSIVE,
        COMPOSITE,
        MONSTER,
        TOXIC,
        ACID,
        HAIRY,
        PUREPAIN,
        PUREWOUNDS,
        PUREBUILDINGDAMAGE,
    }type;
    //constructor
    Shot():minAccuracy(0),accuracy(0),damage(0){}
    Shot(TYPE t,int dam):type(t),damage(dam),accuracy(0),minAccuracy(0){}
    //getters
    int getDamage(){return damage;}
    int getAccuracy(){return accuracy;}
    //setters
    bool setMinAccuracy(int a){if(a<0 || a>100) return false;minAccuracy=a;return true;}
    bool setAccuracy(int a){if(a<minAccuracy || a>100) return false;accuracy=a;return true;}
    bool setDamage(int d){if(d<0) return false;damage=d;return true;}
    bool changeAccuracy(int a){return setAccuracy(accuracy+a);}
    bool changeDamage(int d){return setDamage(damage+d);}
    private:
        //variables
        int damage;
        int accuracy;
        int minAccuracy;
};

///ITEM base class
class Item{
    public:
        //enumerations
        enum TYPE{
            CANNONBALL,
            LIQUIDCONTAINER,
            FRAGMENT,
            LIQUID,
        };
        //constructors
        Item(std::string name,int size):name(name),size(size){}
        //getters
        std::string getName(){return name;}
        int getSize(){return size;}
        //pure virtual functions
        virtual TYPE getType()=0;
    protected:
        //variables
        std::string name;
        int size;
};

///ITEM CONTAINER base class
class ItemContainer{
    public:
        //constructor
        ItemContainer(int maxSize):maxSize(maxSize),occSize(0){}
        //functions
        bool addItem(Item *item);
    protected:
        //variables
        std::vector<Item*> items;
        int maxSize;
        int occSize;
};

///CANNON BALL class
class Cannonball: public Item,public Merchandise,public Storable{
    public:
        //constructors
        Cannonball(std::string name,int size,int damage):Item(name,size),damage(damage){}
        //getters
        int getDamage(){return damage;}
        //Item VFF
        TYPE getType(){return CANNONBALL;}
        //Merchandise VFF
        Thing::THINGTYPE getThingType(){return Thing::ITEM;}
        Thing *clone(){return new Cannonball(*this);}
        const char *getMerchantString();
        //Storable VFF
        void invPrint(int x,int y);
    private:
        //variables
        int damage;
};

///LIQUID class
class Liquid: public Merchandise{
    public:
        //enumerations
        enum TYPE{
            TOXIC,
            POISONOUS,
            EXPLOSIVE,
            ALIVE,
            ACID,
        };
        //constructor
        Liquid(std::string name,TYPE type):name(name),type(type){}
        //destructor
        ~Liquid(){}
        //getters
        std::string getName(){return name;}
        TYPE getType(){return type;}
        //Merchandise VFF
        Thing::THINGTYPE getThingType(){return Thing::ITEM;}
        Thing *clone(){return new Liquid(*this);}
        const char *getMerchantString();
    private:
        //variables
        std::string name;
        TYPE type;
};

///LIQUID CONTAINER class
class LiquidContainer: public Item,public Merchandise,public Storable{
    public:
        //functions
        bool fillWith(Liquid *l,int v);
        void empty(){delete liquid;liquid=NULL;volume=0;}
        //constructors
        LiquidContainer(std::string name,int size,bool holdAcid,int volume,Liquid *liquid):Item(name,size),holdAcid(holdAcid),liquid(NULL){fillWith(liquid,volume);}
        //checkers
        bool isEmpty(){return volume==0;}
        bool isFull(){return volume==maxVolume;}
        bool canHoldAcid(){return holdAcid;}
        bool isCorroded(){return corroded;}
        //Item VFF
        TYPE getType(){return LIQUIDCONTAINER;}
        //Merchandise VFF
        Thing::THINGTYPE getThingType(){return Thing::ITEM;}
        Thing *clone(){return new LiquidContainer(*this);}
        const char *getMerchantString();
        //Storable VFF
        void invPrint(int x,int y);
    private:
        //variables
        int maxVolume,volume;
        bool holdAcid;
        bool corroded;
        Liquid *liquid;
};

///FRAGMENT class
class Fragment: public Item,public Thing,public Storable{
    public:
        //constructors
        Fragment(std::string name,int size):Item(name,size){}
        //getters
        int getSize(){return size;}
        //Item VFF
        TYPE getType(){return FRAGMENT;}
        //Thing VFF
        Thing::THINGTYPE getThingType(){return Thing::ITEM;}
        Thing *clone(){return new Fragment(*this);}
        //Storable VFF
        void invPrint(int x,int y);
};

#endif
