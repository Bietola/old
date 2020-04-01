#ifndef GAMESTATES_H
#define GAMESTATES_H

#include<cstddef>

#include "Texture.h"
#include "Interface.h"
#include "Thing.h"
#include "MCShip.h"
#include "SpecialShips.h"
#include "Network.h"
#include "BGResourceContainer.h"

/*TEMP*/#include "globals.h"

///state enumeration
enum GAMESTATE{
    STATE_NULL,
    STATE_INTRO,
    STATE_MAIN_MENU,
    STATE_END_SCREEN,
    STATE_LV_GENERATION,
    STATE_CONTROL_GAME_LOOP,
    STATE_EXIT,
};

///GameState class
class GameState{
    public:
        //current state id
        static GAMESTATE idCurrent;
        //next state id
        static GAMESTATE idNext;
        //change state
        static void change(GameState **gs);
        //handle events, do logic and show stuff on the screen
        virtual void handleEvents()=0;
        virtual void logic()=0;
        virtual void show()=0;
};

///intro (WIP)
class Intro:public GameState{
    public:
        //constructor
        Intro();
        //virtual functions fulfillment
        void handleEvents();
        void logic();
        void show();
    private:
        //background texture
        Texture bg;
};

///main menu (WIP)
class MainMenu:public GameState{
    public:
        //virtual functions fulfillment
        void handleEvents();
        void logic();
        void show();
    private:
        ;
};

///end screen
class EndScreen:public GameState{
    public:
        //constructor
        EndScreen();
        //virtual functions fulfillment
        void handleEvents();
        void logic();
        void show();
    private:
        //background
        Texture *bg;
};

///level generation (loading screen)
class LVGeneration:public GameState{
    public:
        //constructor
        LVGeneration();
        //virtual functions fulfillment
        void handleEvents();
        void logic();
        void show();
    private:
        //loading screen interface elements
        Texture loadText;
        NumBox loadNumBox;
        int pr;
};

///generic game loop
class GameLoop:public GameState{
    private:
        //current level
        static int curLevel;
        //spawn enemies
        void spawnEnemies();
    protected:
        //background texture
        Texture bg;
        //level timer
        Timer levelTimer;
    public:
        //friend classes
        friend class Ship; //!needed for base damaging
        //spawn enemies or not (used for debugging purposes)
        static bool enemiesSpawn;
        //duration of a level
        static const int LEVEL_TIME;
        //base hp
        static int baseHp;
        //monty rarity
        static int montyRarity;
        //constructor
        GameLoop();
        //static getters
        static int getCurLevel(){return curLevel;}
        static int getBaseHp(){return baseHp;}
        //virtual functions fulfillment
        virtual void logic();
};

///control mode generic game loop
class ControlGameLoop:public GameLoop{
    public:
        //constructor
        ControlGameLoop();
        ControlGameLoop(CThing *mct);
        //set main controllable ship
        static void setMcThing(CThing *mct);
        //virtual functions fulfillment
        void handleEvents();
        void logic();
        void show();
    protected:
        //main controllable ship
        static CThing *mcThing;
};

#endif
