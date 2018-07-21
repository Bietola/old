#ifndef THING_H
#define THING_H

#include <vector>

#include "menu.h"

#include "utils.h"

///THING base class
struct Thing{
    enum THINGTYPE{
        SHIP,
        SHIPBUILDING,
        ITEM,
        PERSON,
    };
    virtual Thing *clone()=0;
    virtual THINGTYPE getThingType()=0;
};

///SELLABLE base class
class Sellable{
    public:
        virtual const char *getMerchantString()=0;
    protected:
        int cost;
        int rareness;
};

///STORABLE base class
class Storable{
    public:
        virtual void invPrint(int x,int y)=0;
};

///MERCHANDISE base class
class Merchandise: public Thing,public Sellable{};

#endif
