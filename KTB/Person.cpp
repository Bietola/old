#include "Person.h"

#include "ShipBuilding.h"

///MOUSTACHE class
//static variables
std::string Moustache::names[Moustache::MAX]={
    "none",
    "chevron",
    "dali",
    "english",
    "pencil",
    "handlebar",
    "petit handlebar",
    "brush",
    "pyramidal",
    "horseshoe",
    "toothbrush",
    "lampshade",
    "fumachu",
    "imperial",
    "walrus"
};
//set random stats
void Moustache::setRand(int level){
    int hairChance=10*level;
    if(rand()%100<hairChance){
        type=(TYPE)rrange(1,MAX);
        length=rand()%level+1;
    }
    else{
        type=(TYPE)0;
        length=0;
    }
}

///BEARD class
//static variables
std::string Beard::names[Beard::MAX]={
    "none",
    "normal",
    "abnormal"
};
//set random stats
void Beard::setRand(int level){
    int hairChance=10*level;
    if(rand()%100<hairChance){
        type=(TYPE)rrange(1,MAX);
        length=rand()%level+1;
    }
    else{
        type=(TYPE)0;
        length=0;
    }
}

///PERSON class
//static members initialization
int Person::nextId=0;
//random generation constructors
Person::Person(int level){
    id=nextId;
    nextId++;
    generate(level);
}
Person::Person(std::string name,int level){
    id=nextId;
    nextId++;
    generate(name,level);
}
//return person's name as if it was in a shop list
const char *Person::getMerchantString(){
    std::stringstream ss;
    ss << getFullName() << " " << moustache.getName() << " " << beard.getName() << " " << cost << "$";
    return ss.str().c_str();
}
//print person's name as if it was in an inventory list
void Person::invPrint(int x,int y){
    //bars
    Bar bloodbar,painbar;
    //BCLR - wounds
    short wClr;
    if(getWoundsLevel()==0) wClr=COLOR_GREEN;
    else if(getWoundsLevel()==1) wClr=COLOR_YELLOW;
    else if(getWoundsLevel()==2) wClr=COLOR_MAGENTA;
    else if(getWoundsLevel()==3) wClr=COLOR_RED;
    else if(getWoundsLevel()==4) wClr=COLOR_BLACK;
    //print symbol and status effects
    set_pair(COLOR_WHITE,wClr);
    mvaddch(y,x,'@');
    //print health bar
    std::string spaceName=" "+getAbbrName();
    sstart_bar(&bloodbar,BAR_HORIZONTAL,x+=1,y,spaceName.c_str(),20,maxBlood,blood,{' ',COLOR_WHITE,COLOR_RED},{' ',COLOR_WHITE,COLOR_GREEN});
    print_bar(bloodbar);
    //print pain bar
    if(getPainLevel()==5){
       set_pair(COLOR_RED,COLOR_YELLOW);
       mvaddstr(y,x+=20,"PAIN");
    }
    else{
        sstart_bar(&painbar,BAR_HORIZONTAL,x+=20,y,"1234",4,4,getPainLevel(),{' ',COLOR_BLUE,COLOR_BLUE},{' ',COLOR_BLACK,COLOR_YELLOW});
        print_bar(painbar);
    }
    //print manned building
    if(isFree()){
        set_pair(COLOR_WHITE,COLOR_RED);
        mvprintw(y,x+=4,"   ");
    }
    else{
        ClrGra sym=mannedBuilding->getSymbol();
        set_pair(sym.fclr,sym.bclr);
        mvprintw(y,x+=4,"%c%d",sym.gra,mannedBuilding->getId());
    }
    //return to default color
    set_pair(COLOR_WHITE,COLOR_BLACK);
}
//random generation initialization functions
void Person::generate(int level){
    generate(names::getRand(names::FIRST),level);
}
void Person::generate(std::string name,int level){
    //names
    firstName=name;
    if(perc(90)) lastName=names::getRand(names::ADJ,names::NAMES);
    else lastName=names::getRand(names::NAMES,names::NAMES);
    capitalize(lastName);
    //facial hair
    moustache.setRand(level);
    beard.setRand(level);
    //person statistics
    mannedBuilding=NULL;
    cost=level*10+rrange(level*10);
    money=rrange(200,300);
    //creature statistics
    maxBlood=rrange(2500,3000);
    blood=maxBlood;
    bloodLossRate=rrange(1,5);
    consciousness=rrange(2500,3000);
    strength=rrange(1,5);
    agility=rrange(10,30);
    toughness=rrange(50,100);
    painResistance=rrange(50,100);
    regeneration=rrange(1,3);
    painRecovery=rrange(1,4);
    selfcontrol=rrange(10,50);
    crazyness=rrange(1,12);
    wounds=0;
    pain=0;
}
//print a person's characteristics on the screen
void Person::describe(){
    clear();
    mvprintw(0,0,"name: %s",getFullName().c_str());
    mvprintw(1,0,"moustache: %s",moustache.getName().c_str());
    mvprintw(2,0,"beard: %s",beard.getName().c_str());
    refresh();
}
//free a person from a manned building
void Person::free(){
    if(mannedBuilding==NULL) return;
    for(int j=0;j<mannedBuilding->crew.size();j++){
        if(mannedBuilding->crew[j]->id==id){
            mannedBuilding->crew.erase(mannedBuilding->crew.begin()+j);
            break;
        }
    }
    mannedBuilding=NULL;
}

///CAPTAIN class
//random generation constructor
Captain::Captain(std::string name):Person(name,5){}
//make the captain say something
void Captain::say(const char *phrase){
    if(curWarnlog!=NULL) add_comm(curWarnlog,"%s: %s",getInitials().c_str(),phrase);
}
