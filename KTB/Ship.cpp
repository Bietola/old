#include "Ship.h"

///SHIP base class
//constructor
Ship::Ship(std::string name,Person *captain,int deckHp,int bridgeHp,int deckSize,int agility,int cost):
    //initialize ship
    autofire(false),
    name(name),
    deckSize(deckSize),
    agility(agility),
    nextCannonId(0),
    nextSailId(0),
    nextSpecialId(0){
    this->cost=cost;
    occSize=0;
    //initialize special buildings
    bridge=new Bridge(bridgeHp);
    addBuilding(bridge);
    addCaptain(captain);
    deck=new Deck(deckHp,deckSize);
    addBuilding(deck);
    bay=new Bay(20000,6000);
    addBuilding(bay);
}
//print the ship's inventory
void Ship::printInventory(){
    Person *manToMan=NULL;
    int y=0;
    bool stCannons=false,stSails=false,stCrew=false;
    clear();
    mvprintw(y,0,"captain: %s",getCaptain()->getFullName().c_str());
    //print ship
    mvprintw(y+=2,0,"ship: %s",getName().c_str());
    //cannons menu
    Menu nm;
    if(!cannons.empty()){
        stCannons=true;
        int my=y;
        start_menu(&nm,INV_X_BUILDINGS,y+=2,"cannons:",COLOR_WHITE,COLOR_BLACK,menuSelKey,menuUpKey,menuDownKey,menuBackKey,COLOR_WHITE,COLOR_YELLOW,"break",menuRightKey,'e','c','t','f','\0');
        y++;
        for(int j=0;j<cannons.size();j++){
            add_menuMember(&nm,"*",COLOR_WHITE,COLOR_WHITE);
        }
        y=my;
    }
    else mvprintw(y+2,INV_X_BUILDINGS,"you have no cannons...");
    //sails menu
    Menu sm;
    if(!sails.empty()){
        stSails=true;
        int my=y;
        start_menu(&sm,INV_X_BUILDINGS,y+=2,"sails:",COLOR_WHITE,COLOR_BLACK,menuSelKey,menuUpKey,menuDownKey,menuBackKey,COLOR_WHITE,COLOR_YELLOW,"break",menuRightKey,'e','c','t','f','\0');
        y++;
        for(int j=0;j<sails.size();j++){
            add_menuMember(&sm,"*",COLOR_WHITE,COLOR_WHITE);
        }
        y=my;
    }
    else mvprintw(y+2,INV_X_BUILDINGS,"you have no sails...");
    //crew menu
    Menu cm;
    if(!getCrew().empty()){
        stCrew=true;
        start_menu(&cm,INV_X_CREW,y+=2,"crew:",COLOR_WHITE,COLOR_BLACK,menuSelKey,menuUpKey,menuDownKey,menuBackKey,COLOR_WHITE,COLOR_YELLOW,"break",menuLeftKey,'e','f','\0');
        y++;
        for(int j=0;j<getCrew().size();j++){
            add_menuMember(&cm,"*",COLOR_WHITE,COLOR_WHITE);
            getCrew(j)->invPrint(INV_X_CREW+1,y+=1);
        }
        print_menu(cm);
    }
    else mvprintw(y+2,INV_X_CREW,"you have no crew members...");
    //quit if necessary
    if(!stCrew || (!stCannons && !stSails)){
        refresh();
        getch();
        return;
    }
    //print ship placement menu
    Menu *curm=(stCannons ? &nm : &sm);
    Menu *curbm=curm;
    int choice;
    do{
        if(curbm==&nm){
            for(int j=0;j<cannons.size();j++){
                cannons[j]->invPrint(INV_X_BUILDINGS+1,nm.y+2+j);
            }
        }
        else if(curbm==&sm){
            for(int j=0;j<sails.size();j++){
                sails[j]->invPrint(INV_X_BUILDINGS+1,sm.y+2+j);
            }
        }
        for(int j=0;j<getCrew().size();j++){
            getCrew(j)->invPrint(INV_X_CREW+1,cm.y+2+j);
        }
        choice=idinter_menu(curm);
        char com=curm->lastBreakKey;
        //change menu - from buildings to crew
        if(com==menuRightKey) curm=&cm;
        //change menu - from crew to buildings
        else if(com==menuLeftKey){
            curm=curbm;
        }
        //change build menu - toggle cannons/sails
        else if(com=='c'){
            if(curm==&nm && stSails){
                erase_menu(nm);
                draw_rectangle(INV_X_BUILDINGS,nm.y+2,INV_X_CREW-1,sm.y+lines_menu(nm),COLOR_BLACK,COLOR_BLACK,' ');
                curm=&sm;
                curbm=&sm;
            }
            else if(curm==&sm && stCannons){
                erase_menu(sm);
                draw_rectangle(INV_X_BUILDINGS,sm.y+2,INV_X_CREW-1,sm.y+lines_menu(sm),COLOR_BLACK,COLOR_BLACK,' ');
                curm=&nm;
                curbm=&nm;
            }
        }
        //do stuff with selection
        else if(com==menuSelKey){
            //crew menu - put selection in man to man
            if(curm==&cm){
                manToMan=getCrew(choice);
            }
        //building menus - man the selection with the man to man
        else if(manToMan!=NULL){
            if(!manToMan->isFree()) manToMan->free();
                if(curm==&nm) cannons[choice]->addPerson(manToMan);
                else if(curm==&sm) sails[choice]->addPerson(manToMan);
                manToMan=NULL;
            }
        }
        //do stuff with selection
        else if(com=='t'){
            //toggle building selection
            if(curm==&nm) cannons[choice]->toggleSelect();
            else if(curm==&sm) sails[choice]->toggleSelect();
        }
        //do stuff with selection
        else if(com=='f'){
            //crew menu - free crew member
            if(curm==&cm) getCrew(choice)->free();
            //building menus - free building completely
            else if(curm==&nm) cannons[choice]->free();
            else if(curm==&sm) sails[choice]->free();
        }
        //do stuff with selection
        else if(com=='z'){
            //assign as first mate
            addFirstMate(getCrew(choice));
        }
        else if(com=='e') break;
    }while(true);
}
//determine if ship can  be targeted
bool Ship::canBeTarget(){
    for(int j=0;j<buildings.size();j++){
        if(buildings[j]->isUsable()) return true;
    }
    return false;
}
//select a random building from the ship
ShipBuilding *Ship::randBuilding(){
    if(!canBeTarget()) return NULL;
    ShipBuilding *b;
    while((b=buildings[rrange(buildings.size())])->isBroken());
    return b;
}
//select a random crew member from the ship
Person *Ship::randPerson(){
    if(getCrew().size()==0) return NULL;
    Person *p;
    while(!(p=getCrew(rrange(getCrew().size())))->isConscious());
    return p;
}
//add a SHIP BUILDING to the SHIP - return if the building has been successfully added
bool Ship::addBuilding(ShipBuilding *building){
    std::stringstream ss;
    ss << "size: " << building->size;
    if(building->size+occSize>deckSize) return false;
    else{
        if(building->getType()==ShipBuilding::CANNON){
            building->id=nextCannonId;
            nextCannonId++;
            cannons.push_back((Cannon*)building);
        }
        else if(building->getType()==ShipBuilding::SAIL) {
            building->id=nextSailId;
            nextSailId++;
            sails.push_back((Sail*)building);
        }
        else{
            building->id=nextSpecialId;
            nextSpecialId++;
        }
        occSize+=building->size;
        buildings.push_back(building);
    }
    return true;
}
//add an item to the ship's bat
bool Ship::addItem(Item *item){
    return getBay()->addItem(item);
}
//add a PERSON to the SHIP - return if the person's been successfully added
bool Ship::addPerson(Person *person){
    return getDeck()->addPerson(person);
}
//add the captain to the ship - return if successful
bool Ship::addCaptain(Person *capt){
    return getBridge()->addPerson(capt);
}
//add the first mate to the ship - return if successful
bool Ship::addFirstMate(Person *firstMate){
    if(getBridge()->crew.size()==1) return false;
    return getBridge()->addPerson(firstMate);
}
//charge the ship's cannons
void Ship::chargeCannons(){
    //make the captain say fire
    getCaptain()->say("recharge.");
    //make the first mate yell fire
    if(hasFirstMate()) getFirstMate()->say(names::getRand(names::FIRSTFIRE).c_str());
    //announce recharge errors and recharge rechargeable cannons
    for(int j=0;j<cannons.size();j++){
        Cannon *c=cannons[j];
        //cannon is not selected
        if(!c->isSelected()){
            continue;
        }
        //cannon not manned
        if(!c->isManned()){
            randPerson()->say("cannon " + c->getSymId() + " is not manned, sir!");
            continue;
        }
        //not enough ammos
        //!WIP
        //cannon's fine - start recharging process
        c->recharge(/*!ammos*/);
    }
}
//pause the ship's cannons
void Ship::pauseCannons(){
    for(int j=0;j<cannons.size();j++){
        if(cannons[j]->chargeTimer.isStarted()) cannons[j]->chargeTimer.pause();
    }
}
//restart the ship's cannons after a pause
void Ship::restartCannons(){
    for(int j=0;j<cannons.size();j++){
        cannons[j]->chargeTimer.restart();
    }
}
//fire the ship's cannons
void Ship::fire(Ship *ene){
    //announce fire
    if(comlog!=NULL){
        //make the captain say fire
        getCaptain()->say("fire.");
    }
    if(batlog!=NULL){
        //make the first mate yell fire
        if(hasFirstMate()) getFirstMate()->say(names::getRand(names::FIRSTFIRE).c_str());
    }
    //announce fire errors, decide which cannons will fire and calculate rain bonus
    int toFire[cannons.size()],rain=0;
    for(int j=0;j<cannons.size();j++){
        Cannon *c=cannons[j];
        //cannon is not selected
        if(!c->isSelected()){
            toFire[j]=false;
            continue;
        }
        //cannon is not manned
        if(!c->isManned()){
            randPerson()->say("cannon " + c->getSymId() + " is not manned, sir!");
            toFire[j]=false;
            continue;
        }
        //cannons is broken
        if(c->isBroken()){
            c->crew[0]->say("cannon " + c->getSymId() + " is broken, sir!");
            toFire[j]=false;
            continue;
        }
        //cannon does not have ammunitions
        if(!c->hasAmmo()){
            c->crew[0]->say("cannon " + c->getSymId() + " is out of ammo, sir!");
            toFire[j]=false;
            continue;
        }
        //cannon is not charged
        if(!c->isCharged()){
            c->crew[0]->say("cannon " + c->getSymId() + " is not charged, sir!");
            toFire[j]=false;
            continue;
        }
        //cannon is ready to fire
        rain++;
        toFire[j]=true;
    }
    //make some random dude from the other ship yells it
    int j;
    for(j=0;j<cannons.size();j++){
        if(toFire[j]==true) break;
    }
    if(j<cannons.size()){
       Person *p;
       if((p=ene->randPerson())!=NULL) p->say(enelog,"INCOMING!!!");
    }
    //fire selected cannons
    for(int j=0;j<cannons.size();j++){
        Cannon *c=cannons[j];
        if(toFire[j]){
            c->crew[0]->say(names::getRand(names::FIRE));
            c->shoot(ene,rain);
        }
    }
}
//reduce damage coming from enemy cannons using the ship's sails
void Ship::evade(Shot &shot,int rain){
    //evade
    for(int j=0;j<sails.size();j++){
        Sail *s=sails[j];
        //check if the sail is selected
        if(!s->isSelected()) continue;
        //check if the sail is manned
        if(!s->isManned()){
            randPerson()->say("sail " + s->getSymId() + " is not manned, sir!");
        }
        //check if sail is usable
        if(!s->isUsable()){
            s->crew[0]->say("sail " + s->getSymId() + " is broken, sir!");
        }
        //sail can be used - use it
        s->evade(shot,rain);
    }
}
//update the crew's stats
void Ship::updateCrew(){
    for(int j=0;j<getCrew().size();j++){
        getCrew(j)->update();
    }
}
//update all that is needed to be update inside the ship
void Ship::update(){
    //update the ship's crew
    updateCrew();
}
//get the ship's merchant description
const char *Ship::getMerchantString(){
    std::stringstream ss;
    ss << name << " " << deckSize << "s " << agility << "a " << cost << "$";
    return ss.str().c_str();
}

///BATTLE FUNCTION
int Ship::battle(const char *intro,Ship *s1,Ship *s2){
    //intro
    move(0,0);clear();
    para(intro);refresh();getch();
    move(0,0);clear();
    //battle
    nodelay(stdscr,TRUE);
    do{
        s1->fight(s2);
        s1->updateCrew();
        if(s1->isSank() || s2->isSank()) break;
        s2->fight(s1);
        s2->updateCrew();
    }while(!s1->isSank() && !s2->isSank());
    nodelay(stdscr,FALSE);
    //clean up
    ;
    //return winner
    if(s1->isSank()) return 1;
    if(s2->isSank()) return 2;
    return 0;
}
