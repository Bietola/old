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
        else if((*gs)->idNext==STATE_CONTROL_GAME_LOOP){
            (*gs)=new ControlGameLoop();
        }
        (*gs)->idCurrent=idNext;
        (*gs)->idNext=STATE_NULL;
    }
}

///Intro(WIP)
//constructor
Intro::Intro(){
    bg.load("background.bmp");
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
    bg.show();
}

///main menu (WIP)
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
    bg=gTextures[TXTR_END_SCREEN_BG_1];
}
//handleEvents
void EndScreen::handleEvents(){
    static SDL_Event e;
    while(SDL_PollEvent(&e)>0){
        //handle window events
        if(Texture::gWindow->handleEvent(&e)) idNext=STATE_EXIT;
    }
}
//logic
void EndScreen::logic(){
    ;
}
//show
void EndScreen::show(){
    bg->show(0,0,SCREEN_W,SCREEN_H);
}

///level generation (loading screen)
//constructor
LVGeneration::LVGeneration(){
    //load text
    loadText.loadFromText("LOADING... ");
}
//handle events
void LVGeneration::handleEvents(){
    ;
}
//logic
void LVGeneration::logic(){
    //generate random terrain
    for(pr=0;pr<BOARD_W;pr++){
        //spawn mountains
        if(!rrange(750)){
            BGResourceContainer tmp(pr);
            tmp.addResource(Resource::ROCK,rrange(50,2000));
            if(pr+tmp.getWidth()<=BOARD_W){
                pr+=tmp.getWidth();
                tmp.clone();
            }
        }
    }
    //initiate main game loop
    idNext=STATE_CONTROL_GAME_LOOP;
}
//show
void LVGeneration::show(){
    //show progress
    loadText.show(SCREEN_W/2,SCREEN_H/2);
    loadNumBox.show(SCREEN_W/2+100,SCREEN_H/2,pr);
}

///game loop base class
//declaration of static variables
const int GameLoop::LEVEL_TIME=2000;
int GameLoop::curLevel=1;
int GameLoop::baseHp=5;
int GameLoop::montyRarity=0;
bool GameLoop::enemiesSpawn=true;
//constructor
GameLoop::GameLoop(){
    bg.load("backgrounds/4.bmp");
    levelTimer.start();
}
//spawn enemies
void GameLoop::spawnEnemies(){
    //!PLACEHOLDER spawn enemies according to current level
    if(levelTimer.getCycles()%LEVEL_TIME==0){
        curLevel++;
        cout<<"LEVEL "<<curLevel<<"\n";
    }
    //Monty ship
    if(rrange(5000)<montyRarity){
        int x=rrange(BOARD_W-gTextures[TXTR_MONTY]->getWidth());
        int y=rrange((BOARD_H-SCREEN_H),(BOARD_H-SCREEN_H)+(BOARD_H-SCREEN_H)/4);
        int hp=1;
        int cdam=1;
        int speed=rrange(3,20)+rrange(montyRarity);
        int res=rrange(100,500)+montyRarity*5+rrange(montyRarity*15);
        int modTime=rrange(200,1000)-montyRarity*5-rrange(montyRarity*10);
        Thing::create(new MontyShip(x,y,hp,cdam,speed,res,modTime));
    }
    //enemies
    if(rrange(600)<curLevel){
        int ene=rrange(100);
        //Billy ship
        if(ene<20){
            int x=rrange(BOARD_W-gTextures[TXTR_PAUL]->getWidth());
            int y=rrange(BOARD_H-SCREEN_H,(BOARD_H-SCREEN_H)+SCREEN_H/2);
            int hp=2+curLevel/10+rrange(curLevel/3);
            int xVel=1+curLevel/10+rrange(curLevel/2);
            int yVelMinMult=2+curLevel/20+rrange(curLevel/5);
            int yVelMaxMult=3+curLevel/21+rrange(curLevel/6);
            if(yVelMaxMult<yVelMinMult) yVelMaxMult=yVelMinMult;
            int bombChargeTime=250-curLevel+rrange(curLevel)*3;
            int bombNumber=1+curLevel/50+rrange(curLevel/20);
            Thing::create(new BillyShip(x,y,hp,1,xVel,yVelMinMult,yVelMaxMult,bombChargeTime,bombNumber));
        }
        //Paul ship
        else if(ene<40){
            int x=rrange(BOARD_W-gTextures[TXTR_PAUL]->getWidth());
            int y=rrange(BOARD_H-SCREEN_H,(BOARD_H-SCREEN_H)+SCREEN_H/2);
            int hp=2+curLevel/5+rrange(curLevel/3);
            int speed=5+curLevel/5+rrange(curLevel/2);
            int cspeed=100;
            int shootTimes=1+curLevel/10+rrange(curLevel/5);
            Thing::create(new PaulShip(x,y,hp,speed,cspeed,shootTimes));
        }
        //Slider ship
        else if(ene<80){
            int x=rrange(BOARD_W-gTextures[TXTR_BOB]->getWidth());
            int y=rrange(BOARD_H-SCREEN_H,BOARD_H/2);
            int hp=1+curLevel/5+rrange(curLevel/2);
            int cdam=1;
            int speed=5+curLevel/5+rrange(curLevel/2);
            Thing::create(new SliderShip(x,y,gTextures[TXTR_BOB],hp,cdam,speed));
        }
        //Bob ship
        else{
            int x=BOARD_W-gTextures[TXTR_BOB]->getWidth();
            int y=0;
            int xspeed=10+curLevel/2+rrange(curLevel);
            int yspeed=2+curLevel/10+rrange(curLevel/5);
            Thing::create(new BobShip(x,y,xspeed,yspeed));
        }
    }
    //!PLACEHOLDER END
}
//logic
void GameLoop::logic(){
    //spawn enemies
    //!WIP
    if(enemiesSpawn) spawnEnemies();
    //make networks do their stuff
    for(int j=0;j<Network::maxId;j++){
        if(Network::networks.at(j)!=NULL){
            Network::networks.at(j)->act();
        }
    }
    //main loop: move  things around the screen
    for(int j=0;j<Thing::maxSpawnedThing;j++){
        if(Thing::things.at(j)!=NULL){
            Thing::things.at(j)->live();
            if(Thing::things.at(j)!=NULL) /*DEBUG*/cout<<Thing::things.at(j)->getID()<<";"<<Thing::things.at(j)->getSpawnID()<<"\t:\t"<<Thing::things.at(j)->getXpos()<<";"<<Thing::things.at(j)->getYpos()<<" | "<<Thing::things.at(j)->getWidth()<<";"<<Thing::things.at(j)->getHeight()<<"\n";
        }
    }
    //exit loop if base hp reaches 0
    if(baseHp<=0) idNext=STATE_END_SCREEN;
}

///control game loop base class
//static variables declaration
CThing *ControlGameLoop::mcThing=NULL;
//constructors
ControlGameLoop::ControlGameLoop(){
    ;
}
ControlGameLoop::ControlGameLoop(CThing *mct){
    setMcThing(mct);
}
//set global main controllable entity
void ControlGameLoop::setMcThing(CThing *mct){
    mcThing=mct;
}
//handleEvents
void ControlGameLoop::handleEvents(){
    //input loop: get user input for controllable entities and window
    static SDL_Event e;
    while(SDL_PollEvent(&e)>0){
        //handle window events
        if(Texture::gWindow->handleEvent(&e)) idNext=STATE_EXIT;
        //move MCShip
        mcThing->handleEvent(&e);
    }
}
//logic
void ControlGameLoop::logic(){
    GameLoop::logic();
    //check if mcShip is dead
    if(mcThing->getHP()<=0) idNext=STATE_END_SCREEN;
}
//show
void ControlGameLoop::show(){
    //show background
    for(int y=0;y<BOARD_H/bg.getHeight()+(BOARD_H%bg.getHeight()>0 ? 1 : 0);y++){
        for(int x=0;x<BOARD_W/bg.getWidth()+(BOARD_W%bg.getWidth()>0 ? 1 : 0);x++){
            bg.showRelativeToViewer(x*bg.getWidth(),y*bg.getHeight(),mcThing->getXpos(),mcThing->getYpos());
        }
    }
    //main loop: show things around the screen
    for(int j=0;j<Thing::maxSpawnedThing;j++){
        if(Thing::things.at(j)!=NULL){
            //!Thing::things.at(j)->show(mcThing);
            mcThing->showRelativeToThis(Thing::things.at(j));
        }
    }
    //show game text
    mcThing->showInterface();
}
