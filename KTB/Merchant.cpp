#include "Merchant.h"

///THING FOR SAIL class
const char *ThingForSale::getMerchantString(){
    std::stringstream ss;
    ss << m->getMerchantString() << " [" << number << "]\0";
    return ss.str().c_str();
}

///MERCHANT base class
void Merchant::sell(int x,int &y,Player *pl){
    y+=para(intro);
    //check if there's still something to sell
    if(merchandise.empty()){
        mvprintw(y+=1,x,"NOTHING LEFT...");
        refresh();
        getch();
        return;
    }
    set_pair(COLOR_YELLOW,COLOR_BLACK);
    mvprintw(y,x,"money: %d",pl->captain->money);
    set_pair(COLOR_WHITE,COLOR_BLACK);
    static Menu m;
    glstart_menu(&m,x,y+=2,(title+"\0").c_str(),"stop");
    for(int j=0;j<merchandise.size();j++){
        gladd_menuMember(&m,merchandise[j].getMerchantString());
    }
    y+=lines_menu(m)+1;
    int choice=idinter_menu(&m);
    bool check;
    //add chosen item to player
    Thing::THINGTYPE type=merchandise[choice].m->getThingType();
    if(type==Thing::SHIP) check=pl->ship=dynamic_cast<Ship*>(merchandise[choice].m->clone());
    else if(type==Thing::SHIPBUILDING) check=pl->ship->addBuilding(dynamic_cast<ShipBuilding*>(merchandise[choice].m->clone()));
    else if(type==Thing::PERSON) check=pl->ship->addPerson(dynamic_cast<Person*>(merchandise[choice].m->clone()));
    else if(type==Thing::ITEM) check=pl->ship->addItem(dynamic_cast<Item*>(merchandise[choice].m->clone()));
    //check if everything's ok
    if(!check){
        move(y,0);
        y+=para("you can't do that for some reason...\0");
    }
    //remove chosen item from merchandise
    else{
        merchandise.erase(merchandise.begin()+choice);
        //print ending
        move(y,0);
        y+=para(ending);
    }
    refresh();
    getch();
}

///CREEPY SHIP MERCHANT class
void CreepyShipMerchant::restock(Player *pl=NULL){
    for(int j=rrange(3,6);j>0;j--){
        int size,agility;
        merchandise.push_back({1,new UserShip(retCapitalize(names::getRand(names::NAMES,names::NAMES)),pl->captain->clone(),10000,5000,size=rrange(5,20),agility=rrange(21,31)-size,rrange(100,200)+size*2+agility)});
    }
}

///TOWN MECHANT classes
//slave merchant
void TownSlaveMerchant::restock(Player *pl=NULL){
    merchandise.clear();
    for(int j=rrange(1,10);j>0;j--){
        merchandise.push_back({1,new Person(rrange(1,3))});
    }
}
//cannons
void TownCannonMerchant::restock(Player *pl=NULL){
    merchandise.clear();
    for(int j=rrange(1,10);j>0;j--){
        merchandise.push_back({1,new BasicCannon(rrange(1,3))});
    }
}
//sails
void TownSailMerchant::restock(Player *pl=NULL){
    merchandise.clear();
    for(int j=rrange(1,10);j>0;j--){
        merchandise.push_back({1,new BasicSail(rrange(1,3))});
    }
}
//stuff
void TownStuffMerchant::restock(Player *pl=NULL){
    merchandise.clear();
    for(int j=rrange(5,20);j>0;j--){
        ThingForSale stuff;
        int r=rrange(100);
        //cannonballs
        if(r<50){
            stuff={50,new Cannonball("bronze cannonball",10,100)};
        }
        //liquid containers
        else if(r<100){
            stuff={10,new LiquidContainer("glass bottle",2,false,rrange(500),new Liquid("toxic water",Liquid::TOXIC))};
        }
        merchandise.push_back(stuff);
    }
}
