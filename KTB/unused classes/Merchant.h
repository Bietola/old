#ifndef MERCHANT_H
#define MERCHANT_H

#include <vector>

#include "menu.h"

#include "Thing.h"

///forward declarations
class Player;

///MERCHANDISE base class
struct Merchandise{
    Merchandise(Thing *thing,int cost);
    const char* getMerchantString();
    int cost;
    Thing *thing;
};

///MERCHANT base class
class Merchant{
    public:
        //constructor
        Merchant(const char *introPhrase,const char *menuTitle);
        //pure virtual functions
        virtual void generateMerchandise()=0;
        virtual void sell(Player *pl,int choice)=0;
        //other functions
        void inter(int x,int &y,Player *pl);
        //variables
        std::vector<Merchandise> merchandise;
        const char *introPhrase,menuTitle;
};

///STARTING place MERCHANT class
class StartingMerchant: public Merchant{
    public:
        //constructor
        StartingMerchant(const char *introPhrase,const char *menuTitle):Merchant(introPhrase,menuTitle){}
        //virtual functions fulfillment
        void generateMerchandise();
        void sell(Player *pl,int choice);
};

#endif
