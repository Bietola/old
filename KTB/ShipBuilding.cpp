#include "ShipBuilding.h"

#include "Ship.h"

///SHIP BUILDING base class
//destructor
ShipBuilding::~ShipBuilding(){
    ;
}
//return the type name
const char *ShipBuilding::getTypeName(){
    TYPE t=getType();
    if(t==CANNON) return "cannon";
    if(t==SAIL) return "sail";
}
//print the building as if it was in an inventory
void ShipBuilding::invPrint(int x,int y){
    //health bar
    Bar hpbar;
    //print symbol
    set_pair(symbol.fclr,symbol.bclr);
    mvaddch(y,x,symbol.gra);
    //print id
    char* barName;
    barName = (char*)malloc(sizeof(char)*100);
    sprintf(barName," %s",name.c_str());
    sstart_bar(&hpbar,BAR_HORIZONTAL,x+=1,y,barName,20,maxHp,hp,{' ',COLOR_WHITE,COLOR_RED},{' ',COLOR_WHITE,COLOR_GREEN});
    print_bar(hpbar);
    //print number of crew members
    if(personMax==crew.size()) set_pair(COLOR_WHITE,COLOR_RED);
    else set_pair(COLOR_WHITE,COLOR_YELLOW);
    mvprintw(y,x+=20,"%d",crew.size());
    //print if selected
    if(selected) set_pair(COLOR_WHITE,COLOR_GREEN);
    else set_pair(COLOR_WHITE,COLOR_RED);
    mvprintw(y,x+=1," ");
    //return to default color
    set_pair(COLOR_WHITE,COLOR_BLACK);
}
//add a person to the building - return if success
bool ShipBuilding::addPerson(Person *person){
    if(crew.size()+1>personMax || !person->isFree()) return false;
    crew.push_back(person);
    person->mannedBuilding=this;
    return true;
}
//take crew damage
void ShipBuilding::takeDamage(Shot shot){
    for(int j=0;j<crew.size();j++){
        crew[j]->takeDamage(shot);
    }
    hp-=shot.getDamage();
    if(hp<=0) hp=0;
}
//free the building from a crew member
void ShipBuilding::free(int index){
    crew.erase(crew.begin()+index);
}
//free the building from all crew
void ShipBuilding::free(){
    for(int j=0;j<crew.size();j++){
        crew[j]->mannedBuilding=NULL;
    }
    crew.clear();
}

/**SPECIAL!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
///DECK class
//add a person without un-freeing it
bool Deck::addPerson(Person *person){
    if(crew.size()+1>personMax || !person->isFree()) return false;
    person->mannedBuilding=NULL;
    crew.push_back(person);
    return true;
}
//damage only free crew members
void Deck::takeDamage(Shot shot){
    for(int j=0;j<crew.size();j++){
        if(crew[j]->isFree()) crew[j]->takeDamage(shot);
    }
    hp-=shot.getDamage();
    if(hp<=0) hp=0;
}

/**SAILS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
///BASIC SAIL class
//constructor
BasicSail::BasicSail(int level):Sail("basic sail",level*2500+rrange(1000),1),level(level){
    symbol={'b',COLOR_BLACK,COLOR_WHITE};
    size=10+rrange(level);
    protection=level*50+rrange(level*50);
    agility=level*2+rrange(level*5);
    cost=level*30+(rrange(20));
}
//get merchant string
const char *BasicSail::getMerchantString(){
    std::stringstream ss;
    ss << "basic sail " << size << "s " << level << "lv ";
    return ss.str().c_str();
}
//evade enemy cannon shot
void BasicSail::evade(Shot &shot,int rain){
    shot.changeDamage(-protection);
    shot.changeAccuracy(-agility);
}

/**CANNONS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!*/
///CANNON base class
//get inventory string
void Cannon::invPrint(int x,int y){
    ShipBuilding::invPrint(x,y);
    x+=23;
    //print if charged
    if(isCharged()){
        set_pair(COLOR_BLUE,COLOR_GREEN);
        mvprintw(y,x+=1,"READY");
    }
    else{
        set_pair(COLOR_BLUE,COLOR_RED);
        mvprintw(y,x+1,"     ");
        mvprintw(y,x+=1,"%d",menChargeTime-chargeTimer.getTicks());
    }
    //return to default color
    set_pair(COLOR_WHITE,COLOR_BLACK);
}
//shoot a cannon round
void Cannon::shoot(Ship *ene,int rain,Comm *comlog=NULL,Comm *batlog=NULL,Comm *enelog=NULL){
    Shot shot;
    chargeTimer.reset();
    chargeTimer.pause();
    shot=calcDamage(ene,rain);
    ene->evade(shot,rain);
    target(ene,shot);
}
//recharge cannon - start charge timer and insert ammo
void Cannon::recharge(/*!ammos*/){
    if(!chargeTimer.isStarted()){
        int toteff=0;
        for(int j=0;j<crew.size();j++){
            toteff+=crew[j]->getEfficency();
        }
        toteff=std::max((int)(100-(toteff/crew.size())),0);
        menChargeTime=chargeTime+chargeTime*toteff/100;
        int DB_menChargeTime=menChargeTime,DB_chargeTime=chargeTime;
        chargeTimer.start();
    }
}

///BASIC CANNON class
//constructor
BasicCannon::BasicCannon(int level):Cannon("basic cannon",level*1000+rrange(250),1,5000),level(level){
    int pr,hpr;
    symbol={'b',COLOR_BLACK,COLOR_WHITE};
    size=level+rrange(3);
    hpr=hp;
    power=level*200+(pr=rrange(250));
    speed=level*100+(rrange(500)-pr-hpr);
    cost=level*30+(rrange(20)+pr+hpr);
}
//get merchant string
const char *BasicCannon::getMerchantString(){
    std::stringstream ss;
    ss << "basic cannon " << size << "s " << level << "lv ";
    return ss.str().c_str();
}
//calculate damage and shot type
Shot BasicCannon::calcDamage(Ship* ene,int rain){
    Shot shot;
    //calculate damage
    shot.setDamage(power+rrange(level*20));
    //calculate accuracy
    shot.setAccuracy(100);
    shot.setMinAccuracy(50);
    //decide shot type
    shot.type=Shot::METAL;
    return shot;
}
//determine target buildings and damage them
void BasicCannon::target(Ship* ene,Shot shot){
    if(ene->canBeTarget()) ene->randBuilding()->takeDamage(shot);
}

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
