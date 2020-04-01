#ifndef RESOURCE_H
#define RESOURCE_H

#include "Texture.h"

///resource class
class Resource{
    public:
        //enumerations
        enum TYPE{
            ROCK,
            MAX,
        };
        //static constants
        static const int MAX_STATES=6;
        //static variables
        static Texture *bgTextures[Resource::MAX][MAX_STATES];
        //constructors
        Resource();
        Resource(TYPE type,double amount);
        //getters
        TYPE getType(){return type;}
        double getAmount(){return amount;}
        //setters
        bool setAmount(double a){if(a<0) return false; else{amount=a;return true;}}
        //changers
        bool addAmount(double a){return setAmount(amount+a);}
        //extraction changers and setters
        double extractFrom(double amout);
    private:
        //variables
        TYPE type;
        double amount;
};

#endif
