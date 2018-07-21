#include "Merchant.h"

#include "Player.h"

///MERCHANDISE class
//constructor
Merchandise::Merchandise(Thing *thing,int cost):thing(thing),cost(cost){}
//get merchant string
const char *Merchandise::getMerchantString(){
    std::stringstream ss;
    ss << thing->getMerchantString() << " " << cost << "$";
    return ss.str().c_str();
}

///MERCHANT base class
//constructor
Merchant::Merchant(const char *introPhrase,const char *menuTitle):introPhrase(introPhrase),menuTitle(menuTitle){
    ///generateMerchandise();
}
//generate an interactive menu for the merchant's merchandise
void Merchant::inter(int x,int &y,Player* pl){
    Menu m;
    y+=para(introPhrase)+1;
    mvprintw(y,0,"money: %d",pl->captain->getMoney());
    glstart_menu(&m,x,y+=2,menuTitle,"stop");
    for(int j=0;j<merchandise.size();j++){
        gladd_menuMember(&m,merchandise[j].getMerchantString());
    }
    y+=lines_menu(m);
    sell(pl,idinter_menu(m));
}

///STARTING place MERCHANT class
//generate merchandise
void StartingMerchant::generateMerchandise(){
    for(int j=rrange(1,6);j>0;j--){
        int size,agility;
        Merchandise mer(new Ship(retCapitalize(names::getRand(names::NAMES,names::NAMES)),size=rrange(5,20),agility=rrange(21,31)-size),rrange(100,200)+size*2+agility);
        merchandise.push_back(mer);
    }
}
void StartingMerchant::sell(Player *pl,int choice){
    pl->ship=merchandise[choice].thing->clone();
}
