#include "GameStates.h"

///game state base class
//static variables declaration
GAMESTATE GameState::idCurrent=STATE_NULL;
GAMESTATE GameState::idNext=STATE_NULL;

//change state
void GameState::change(GameState **gs){
    GAMESTATE idNext=(*gs)->idNext;
    if((*gs)->idNext!=STATE_NULL && (*gs)->idNext!=STATE_EXIT){
        if((*gs)->idNext==STATE_INTRO){
            (*gs)=new Intro();
        }
        else if((*gs)->idNext==STATE_MAIN_MENU){
            (*gs)=new MainMenu();
        }
        else if((*gs)->idNext==STATE_END_SCREEN){
            (*gs)=new EndScreen();
        }
        (*gs)->idCurrent=idNext;
        (*gs)->idNext=STATE_NULL;
    }
}

///Intro
//constructor
Intro::Intro(){
    ;
}
//handleEvents
void Intro::handleEvents(){
    ;
}
//logic
void Intro::logic(){
    ;
}
//show
void Intro::show(){
    ;
}

///main menu
//handleEvents
void MainMenu::handleEvents(){
    ;
}
//logic
void MainMenu::logic(){
    ;
}
//show
void MainMenu::show(){
    ;
}

///end screen
//constructor
EndScreen::EndScreen(){
    ;
}
//handleEvents
void EndScreen::handleEvents(){
    ;
}
//logic
void EndScreen::logic(){
    ;
}
//show
void EndScreen::show(){
    ;
}
