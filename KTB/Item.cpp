#include "Item.h"

///ITEM CONTAINER base class
//ad an item
bool ItemContainer::addItem(Item *item){
    if(item==NULL) return false;
    if(occSize+item->getSize()>maxSize) return false;
    occSize+=item->getSize();
    items.push_back(item);
    return true;
}

///CANNONBALL class
//Sellable VFF
const char *Cannonball::getMerchantString(){
    std::stringstream ss;
    ss << name << " (" << damage << ")\0";
    return ss.str().c_str();
}
//Storable VFF
void Cannonball::invPrint(int x,int y){
    set_pair(COLOR_BLACK,COLOR_WHITE);
    mvprintw(y,x,"%s (%d)",name.c_str(),damage);
    set_old_pair();
}

///LIQUID class
//Sellable VFF
const char *Liquid::getMerchantString(){
    return name.c_str();
}

///LIQUID CONTAINER class
//Sellable VFF
const char *LiquidContainer::getMerchantString(){
    std::stringstream ss;
    ss << name << " (" << volume << "mL " << liquid->getName() << " max: " << maxVolume;
    if(holdAcid) ss << " ACID";
    ss << ")\0";
    return ss.str().c_str();
}
//Storable VFF
void LiquidContainer::invPrint(int x,int y){
    set_pair(COLOR_BLACK,COLOR_WHITE);
    mvaddstr(y,x,getMerchantString());
    set_old_pair();
}
//fill the container with a Liquid - return if successful
bool LiquidContainer::fillWith(Liquid *l,int v){
    if(liquid!=NULL && l->getType()!=liquid->getType()) return false;
    volume=std::min(maxVolume,volume+v);
    liquid=l;
}

///FRAGMENT class
//Storable VFF
void Fragment::invPrint(int x,int y){
    std::stringstream ss;
    ss << name << " (" << size << ")\0";
    return ss.str().c_str();
}
