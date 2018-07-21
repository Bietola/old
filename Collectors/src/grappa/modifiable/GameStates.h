#ifndef GAMESTATES_H
#define GAMESTATES_H

#include<cstddef>

///state enumeration
enum GAMESTATE{
    STATE_NULL,
    STATE_INTRO,
    STATE_MAIN_MENU,
    STATE_END_SCREEN,
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
        ;
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
        ;
};

#endif
