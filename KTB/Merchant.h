#ifndef MERCHANT_H
#define MERCHANT_H

#include <vector>

#include "menu.h"

#include "utils.h"

#include "Thing.h"
#include "Player.h"
#include "Ship.h"
#include "ships.h"
#include "ShipBuilding.h"
#include "Person.h"

///THING FOR SALE class wrapper
struct ThingForSale{
    int number;
    Merchandise *m;
    const char *getMerchantString();
};

///MERCHANT base class
class Merchant{
    public:
        //pure virtual functions
        virtual void restock(Player *pl=NULL)=0;
        //template functions
        void sell(int x,int &y,Player *pl);
    /*protected:*/
        //constructor
        Merchant(std::string title,std::string intro,std::string ending):title(title),intro(intro),ending(ending){}
        //variables
        std::string intro;
        std::string title;
        std::string ending;
        std::vector<ThingForSale> merchandise;
};

///CREEPY SHIP MERCHANT class
class CreepyShipMerchant: public Merchant{
    public:
        //constructor
        CreepyShipMerchant():Merchant("me ships",
                                      "As you enter the shop a merchant welcomes you: \"Welcome to me ship shop capt'n! I guess ye' need a ship hey?\"",
                                      "A huge grin appears on the merchant's face, \"Thanks a lot sir! Not that you had any choice, but still...\", suddenly the merchant points behind you with his eyes wide open. As you turn around to look, you hear a loud laugh and a strong gust of wind almost makes you lose your balance. When you turn around again, the merchant is gone along with his ships. Looks like this place is going to be a lot more interesting than you originally thought..."){}
        //VFF
        void restock(Player *player=NULL);
};

///TOWN MERCHANT classes
//slaves
class TownSlaveMerchant: public Merchant{
    public:
        //constructor
        TownSlaveMerchant():Merchant("me slaves",
                                     "Hello there!",
                                     "thanks..."){}
        //VFF
        void restock(Player *player=NULL);
};
//cannons
class TownCannonMerchant: public Merchant{
    public:
        //constructor
        TownCannonMerchant():Merchant("me cannons",
                                      "Hello there!",
                                      "thanks..."){}
        //VFF
        void restock(Player *player=NULL);
};
//sails
class TownSailMerchant: public Merchant{
    public:
        //constructor
        TownSailMerchant():Merchant("me sails",
                                    "Hello there!",
                                    "thanks..."){}
        //VFF
        void restock(Player *player=NULL);
};
//stuff
class TownStuffMerchant: public Merchant{
    public:
        //constructor
        TownStuffMerchant():Merchant("me stuffs",
                                     "Hello there!",
                                     "thanks..."){}
        //VFF
        void restock(Player *player=NULL);
};

#endif
