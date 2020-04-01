#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>

/*
#undef private
#undef protected
#define private public
#define protected public
*/

#include "globals.h"
#include "GameStates.h"
#include "Timer.h"
#include "Thing.h"
#include "Ship.h"
#include "SpecialShips.h"
#include "Builder.h"
#include "MultiBuilder.h"
#include "Cannon.h"
#include "SpecialCannons.h"
#include "SpecialBuildings.h"
#include "SpecialMaterialProducers.h"
#include "SpecialMaterialResourceExtractors.h"
#include "MCShip.h"

#include "Resource.h"
#include "BGResourceContainer.h"

using namespace std;

///main
int main(int argc,char *args[]){
    start_game();
    ///Builder bobMitraBuilder(200,300,gTextures[TXTR_FRANKCANNON],COLL_SHIP,CFACT_FRIEND,1,1,new BasicProjectile(CFACT_FRIEND,0,-25),50);
    vector<Thing*> randModels;
    randModels.resize(11);
    randModels.at(0)=new Vincent();
    randModels.at(1)=new BobCannon();
    randModels.at(2)=new MitraBobCannon();
    randModels.at(3)=new SpikeTrap(gTextures[TXTR_SPIKE_TRAP_1],1);
    randModels.at(4)=new HenryCannon();
    randModels.at(5)=new AstraCannon();
    randModels.at(6)=new ShittyRocky();
    randModels.at(7)=new Rocky();
    randModels.at(8)=new RockTemple(gTextures[TXTR_ROCK_TEMPLE_1],5,250.0,1);
    randModels.at(9)=new Umbrella(gTextures[TXTR_UMBRELLA_1],2,75.0);
    randModels.at(10)=new BabyDestroyerShip();
    MCShip you(0,0,gTextures[TXTR_MCSHIP],MOVE_C_SLIDER,SHOOT_C_SLIDER,3,1,100,100,new BasicProjectile(CFACT_FRIEND,0,-20),randModels);
    you.repos(0,BOARD_H-you.getHeight());
    Ship bob(0,0,gTextures[TXTR_BOB],COLL_SHIP,CFACT_ENEMY,MOVE_DUMATTACKER,1,1);
    SliderShip frank(0,0,gTextures[TXTR_BOB],1,1,7);
    //!TEST
    ///BGResourceContainer bgrc(0,BOARD_H-Resource::bgTextures[Resource::ROCK]->getHeight());
    ///bgrc.addResource(Resource::ROCK,50);
    ///bgrc.spawn();
    ///Cannon bobCannon(200,300,gTextures[TXTR_BOBCANNON],SHOOT_DUMMY,3,10,new Ship(201,301,gTextures[TXTR_BASIC_PROJ],COLL_PROJ,MOVE_NOTHINK,1,1,0,1));
    ///MultiBuilder bobBuilder(200,300,gTextures[TXTR_BOBCANNON],COLL_CANNON,3,bobMultiBuilderModels,10);
    you.spawn();
    ///bobCannon.spawn();
    ///bobBuilder.spawn();
    ///Thing::create(new PaulShip(10,BOARD_H-2*SCREEN_H/3,20,5,800,10));
    //!Thing::create(new Vincent(0,BOARD_H-gTextures[TXTR_VINCENT]->getHeight()));
    //BillyShip(int x,int y,int hp,int cdam,int xVel,int yVelMult,int bombChargeTime,int bombNumber);
    //!Thing::create(new BillyShip(20,BOARD_H-SCREEN_H/2,5,1,2,/*YVEL*/10,100,3));
    //set main controllable entity for controllable game loops
    ControlGameLoop::setMcThing(&you);
    //current game state
    GameState *gameState=new LVGeneration();
    //!DEBUG handle enemy spawning
    GameLoop::enemiesSpawn=true;
    //!TEST try zooming
    Texture::zoom(1);
    ///MAIN LOOP
    while(gameState->idNext!=STATE_EXIT){
        //DEBUG: take track of the time
        int totalTime=SDL_GetTicks();
        //DEBUG: print the current cycle
        cout<<"CYCLE #"<<Timer::getCurrentCycle()<<endl;
        //clear the screen
        SDL_RenderClear(Texture::getRenderer());
        //input loop: get user input for controllable entities and window
        gameState->handleEvents();
        //main loop: move and show things around the screen
        gameState->logic();
        gameState->show();
        //change game state (if needed)
        GameState::change(&gameState);
        //present the scene
        /*if(SDL_GetTicks()-totalTime<30)*/ /*SDL_RenderPresent(Texture::getRenderer());*/ Texture::update();
        //increment timer
        Timer::increment();
        //increment timer and adjust FPS
        while((SDL_GetTicks()-totalTime)<18);
        //DEBUG: print the amount of time that took for this cycle to work
        cout<<"time: "<<SDL_GetTicks()-totalTime<<endl;
    }
    cout<<"thanks for playing SWars :)"<<endl<<"remember that pi = "<<PI<<endl;
    getch();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
