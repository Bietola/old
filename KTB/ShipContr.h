#ifndef SHIPCONTR_H
#define SHIPCONTR_H

#include <ctime>

#include "menu.h"
#include "comments.h"
#include "bar.h"

#include "Ship.h"
#include "Person.h"

///battle menu constants
#define BAT_INTRO_X 0
#define BAT_INTRO_Y 0

#define BAT_LMENU_X 0
#define BAT_LMENU_Y 2

#define BAT_RMENU_X 33
#define BAT_RMENU_Y 2

#define BAT_COMLOG_X 0
#define BAT_COMLOG_Y 15
#define BAT_COMLOG_LINES 9
#define BAT_COMLOG_LENGTH 20

#define BAT_LOG_X 22
#define BAT_LOG_Y 15
#define BAT_LOG_LINES 9
#define BAT_LOG_LENGTH 36

#define BAT_ENELOG_X 59
#define BAT_ENELOG_Y 15
#define BAT_ENELOG_LINES 9
#define BAT_ENELOG_LENGTH 20

#define BAT_HPBAR_X 64
#define BAT_HPBAR_Y 4
#define BAT_HPBAR_LENGTH 10

#define BAT_ENEHPBAR_X 66
#define BAT_ENEHPBAR_Y 4
#define BAT_ENEHPBAR_LENGTH 10

///USER Ship Fight Control class
class UserSFC: public Ship{
    public:
        static Comm *curComlog,*curBatlog,*curEnelog;
        UserSFC(std::string name,Person *captain,int deckHp,int bridgeHp,int deckSize,int agility,int cost):Ship(name,captain,deckHp,bridgeHp,deckSize,agility,cost),start(false){}
        void init(Ship *ene);
        void fight(Ship *ene);
        Thing *clone(){return new UserSFC(*this);}
    private:
        Person *manToMan;
        Menu cm,nm,sm,enm,esm,ecm,*curm,*curRm,*curLm;
        Bar hpbar,enehpbar;
        bool stCannons,stSails,stCrew,eneStCannons,eneStSails,eneStCrew,start;
        clock_t startTime;
};

///DUMMY Ship Fight Control class
class DummySFC: public Ship{
    public:
        DummySFC(std::string name,Person *captain,int deckHp,int bridgeHp,int deckSize,int agility,int cost):Ship(name,captain,deckHp,bridgeHp,deckSize,agility,cost),start(false){}
        void init(Ship *ene);
        void fight(Ship *ene);
        Thing *clone(){return new DummySFC(*this);}
    private:
        bool start;
};

///CHARGER Ship Fight Control class
class ChargerSFC: public Ship{
    public:
        ChargerSFC(std::string name,Person *captain,int deckHp,int bridgeHp,int deckSize,int agility,int cost):Ship(name,captain,deckHp,bridgeHp,deckSize,agility,cost){}
        void fight(Ship *ene);
        Thing *clone(){return new ChargerSFC(*this);}
};

#endif
