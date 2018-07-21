#include "MCShip.h"

#include "GameStates.h"

///main controllable ship functions
//constructors
MCShip::MCShip():Thing(),Ship(),Cannon(),MultiBuilder(),NetworkMultiBuilder(),MaterialProductMultiBuilder(),MaterialUser(),MaterialSharer(){start();}
MCShip::MCShip(int x,int y,Texture *t,MOVE mt,SHOOT st,int hp,int cdam,int cspeed,int maxmat,Ship *proj,vector<Thing*> ms):Thing(x,y,t,COLL_SHIP,CFACT_FRIEND,hp,cdam),Ship(mt),Cannon(proj,cspeed,st),Sharer(),MultiBuilder(1000,ms),NetworkMultiBuilder(),MaterialProductMultiBuilder(),MaterialUser(0.0,100000.0),MaterialSharer(){start();}
//start MCShip
void MCShip::start(){
    lvBox=new NumBox(255,255,255,2);
    buildSlotBox=new NumBox(255,255,255,2);
    materialBox=new NumBox(255,255,255,5);
    baseHpBox=new NumBox(255,255,255,1);
    hpBox=new NumBox(255,255,255,1);
    buildTextureBox=new TextureBox(models);
    montyBox=new NumBox(255,255,255,2);
    network=new BossNetwork(this,Sharer::MATERIALSHARER);
    addBuildNetwork(network);
    addBuildNetwork(new EvenDistributionNetwork(Sharer::ENERGYSHARER));
    lastE=NULL;
}
//handle user input
void MCShip::handleEvent(SDL_Event *e){
    lastE=e;
}
//show interface
void MCShip::showInterface(){
    int x=0;
    buildTextureBox->setCurTextureI(curModelI);
    lvBox->show(x,0,GameLoop::getCurLevel());
    buildSlotBox->show(x+=110,0,curModelI);
    buildTextureBox->getCurTexture()->show(x+=110,70-buildTextureBox->getCurTexture()->getHeight());
    materialBox->show(x+=150,0,materials);
    baseHpBox->show(x+=530,0,GameLoop::getBaseHp());
    hpBox->show(x,100,hp);
    montyBox->show(x,200,GameLoop::montyRarity);
}
//show specified thing in relative to the MCShip
void MCShip::showRelativeToThis(Thing *thing){
    int vx=xPos;
    /*int vy=yPos;*/
    if(vx<SCREEN_W/2) thing->showRelativeToField(0,BOARD_H-SCREEN_H);
    else if(vx>BOARD_W-SCREEN_W/2) thing->showRelativeToField(BOARD_W-SCREEN_W,BOARD_H-SCREEN_H);
    else thing->showRelativeToField(vx-SCREEN_W/2,BOARD_H-SCREEN_H);
}
//modified version of move function
void MCShip::move(){
    Ship::move();
    Cannon::model->repos(xPos,BOARD_H-Cannon::model->getHeight());
    MultiBuilder::model->repos(xPos,BOARD_H-MultiBuilder::model->getHeight());
}
//virtual functions fulfillment
void MCShip::think(){
    //uncontrolled actions
    //reset cannon and builder
    Cannon::stopBuilding();
    MultiBuilder::stopBuilding();
    SDL_StartTextInput();
    //controlled actions
    int memModelI=-1;
    static bool keyIsDown=false;
    if(lastE!=NULL){
        if(lastE->type==SDL_KEYDOWN){
            SDL_Keycode k=lastE->key.keysym.sym;
            //movement
            if(moveType==MOVE_C_SLIDER){
                if(k==SDLK_a) redir(-11,0);
                else if(k==SDLK_d) redir(11,0);
                else if(k==SDLK_s) redir(0,0);
            }
            //shooting
            if(shootType==SHOOT_C_SLIDER){
                if(k==SDLK_w || k==SDLK_i){
                    Cannon::startBuilding();
                }
            }
            //building
            if(!keyIsDown){
                if(k==SDLK_0) curModelI=0;
                else if(k==SDLK_1) curModelI=1;
                else if(k==SDLK_2) curModelI=2;
                else if(k==SDLK_3) curModelI=3;
                else if(k==SDLK_4) curModelI=4;
                else if(k==SDLK_5) curModelI=5;
                else if(k==SDLK_6) curModelI=6;
                else if(k==SDLK_7) curModelI=7;
                else if(k==SDLK_8) curModelI=8;
                else if(k==SDLK_9) curModelI=9;
                else if(k==SDLK_DELETE) curModelI=10;
                else if(k==SDLK_k) MultiBuilder::startBuilding();
                else if(k==SDLK_l){
                    if(curModelI==models.size()-1) setModel(0);
                    else nextModel();
                }
                else if(k==SDLK_j){
                    if(curModelI==0) setModel(models.size()-1);
                    else prevModel();
                }
            }
            keyIsDown=true;
        }
        else if(lastE->type==SDL_KEYUP) keyIsDown=false;
    }
    SDL_StopTextInput();
}
void MCShip::act(){
    Ship::act();
    Cannon::act();
    NetworkMultiBuilder::act();
}
