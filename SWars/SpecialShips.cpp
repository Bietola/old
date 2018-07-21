#include "SpecialShips.h"

/// MODELS ///
///shooting ship (SShip) functions
//constructors
SShip::SShip(MOVE mt,SHOOT st,int cspeed,Ship *proj):Ship(mt),Cannon(proj,cspeed,st){
    ;
}
SShip::SShip(MOVE mt,SHOOT st,int vx,int vy,int cspeed,Ship *proj):Ship(mt,vx,vy),Cannon(proj,cspeed,st){
    ;
}
//virtual destructor
SShip::~SShip(){
    delete Cannon::projectile;
}
//virtual functions fulfillment
void SShip::move(){
    Ship::move();
    if(model!=NULL) model->repos(xPos+modelSpawnXPos,yPos+modelSpawnYPos);
}
void SShip::act(){
    Ship::act();
    if(things.at(spawnId)!=NULL) Cannon::act();
}

/// PROJECTILES ///
///basic projectile functions
//constructors
BasicProjectile::BasicProjectile():Thing(gTextures[TXTR_BASIC_PROJ],COLL_PROJ,CFACT_FRIEND,1,1),Ship(MOVE_NOTHINK,0,-50){}
BasicProjectile::BasicProjectile(int vx,int vy):Thing(gTextures[TXTR_BASIC_PROJ],COLL_PROJ,CFACT_FRIEND,1,1),Ship(MOVE_NOTHINK,vx,vy){}
BasicProjectile::BasicProjectile(int vx,int vy,int cdam):Thing(gTextures[TXTR_BASIC_PROJ],COLL_PROJ,CFACT_FRIEND,1,cdam),Ship(MOVE_NOTHINK,vx,vy){}
BasicProjectile::BasicProjectile(COLLFACTION cf):Thing(gTextures[TXTR_BASIC_PROJ],COLL_PROJ,cf,1,1),Ship(MOVE_NOTHINK,0,-50){}
BasicProjectile::BasicProjectile(COLLFACTION cf,int vx,int vy):Thing(gTextures[TXTR_BASIC_PROJ],COLL_PROJ,cf,1,1),Ship(MOVE_NOTHINK,vx,vy){}
BasicProjectile::BasicProjectile(COLLFACTION cf,int vx,int vy,int cdam):Thing(gTextures[TXTR_BASIC_PROJ],COLL_PROJ,cf,1,cdam),Ship(MOVE_NOTHINK,vx,vy){}


///dum projectile functions
//constructors
DumProjectile::DumProjectile():Thing(rrange(BOARD_W-getWidth()),0,gTextures[TXTR_BASIC_PROJ],COLL_PROJ,CFACT_FRIEND,1,1),Ship(MOVE_SPECIAL){
    xSpeed=20;
    ySpeed=20;
    yVel=ySpeed;
}
DumProjectile::DumProjectile(int xspeed,int yspeed,int cdam):Thing(gTextures[TXTR_BASIC_PROJ],COLL_PROJ,CFACT_FRIEND,1,cdam),Ship(MOVE_SPECIAL){
    xSpeed=xspeed;
    ySpeed=yspeed;
    yVel=ySpeed;
}
DumProjectile::DumProjectile(COLLFACTION cf):Thing(rrange(BOARD_W-getWidth()),0,gTextures[TXTR_BASIC_PROJ],COLL_PROJ,cf,1,1),Ship(MOVE_SPECIAL){
    xSpeed=20;
    ySpeed=20;
    yVel=ySpeed;
}
DumProjectile::DumProjectile(COLLFACTION cf,int xspeed,int yspeed,int cdam):Thing(gTextures[TXTR_BASIC_PROJ],COLL_PROJ,cf,1,cdam),Ship(MOVE_SPECIAL){
    xSpeed=xspeed;
    ySpeed=yspeed;
    yVel=ySpeed;
}
DumProjectile::DumProjectile(int xspeed,int yspeed){
    xSpeed=xspeed;
    ySpeed=yspeed;
    yVel=ySpeed;
}
//other functions
void DumProjectile::think(){
    xVel=rrange(-xSpeed,xSpeed);
    yVel=rrange(-ySpeed,ySpeed);
}

///temp projectile functions
//constructors
TempProjectile::TempProjectile(COLLFACTION cf,int xspeed,int yspeed,int cdam,int dur):Thing(gTextures[TXTR_BASIC_PROJ],COLL_PROJ,cf,1,cdam),DumProjectile(xspeed,yspeed){duration=dur;lifeTimer.start();}
TempProjectile::TempProjectile(Texture *t,COLLFACTION cf,int xspeed,int yspeed,int cdam,int dur):Thing(t,COLL_PROJ,cf,1,cdam),DumProjectile(xspeed,yspeed){duration=dur;lifeTimer.start();}
TempProjectile::TempProjectile(TempProjectile &tempProjectile){
    *this=tempProjectile;
    lifeTimer.reset();
}
//other functions
void TempProjectile::ccheck(){
    if(lifeTimer.getCycles()>=duration) hp=0;
}

/// ENEMIES ///
///bob ship functions
//constructors
BobShip::BobShip():Thing(rrange(BOARD_W-getWidth()),0,gTextures[TXTR_BOB],COLL_SHIP,CFACT_ENEMY,1,1),Ship(MOVE_SPECIAL){
    addFlag(Thing::HURTS_BASE);
    xSpeed=30;
    ySpeed=5;
    yVel=ySpeed;
}
BobShip::BobShip(int x,int y):Thing(x,y,gTextures[TXTR_BOB],COLL_SHIP,CFACT_ENEMY,1,1),Ship(MOVE_SPECIAL){
    addFlag(Thing::HURTS_BASE);
    xSpeed=30;
    ySpeed=5;
    yVel=ySpeed;
}
BobShip::BobShip(int x,int y,int xspeed,int yspeed):Thing(x,y,gTextures[TXTR_BOB],COLL_SHIP,CFACT_ENEMY,1,1),Ship(MOVE_SPECIAL){
    addFlag(Thing::HURTS_BASE);
    xSpeed=xspeed;
    ySpeed=yspeed;
    yVel=ySpeed;
}
//other functions
void BobShip::think(){
    xVel=rrange(-xSpeed,xSpeed);
    if(ySpeed!=yVel) yVel=ySpeed;
}

///slider ship functions
//constructors
SliderShip::SliderShip():Thing(),Ship(){
    speed=rrange(0,5);
    curDir=1;
}
SliderShip::SliderShip(int x,int y,Texture *t,int hp,int cdam,int speed):Thing(x,y,t,COLL_SHIP,CFACT_ENEMY,hp,cdam),Ship(MOVE_SPECIAL){
    this->speed=speed;
    if(rrange(100)<50) curDir=1;
    else curDir=-1;
}
//other functions
void SliderShip::think(){
    if(yVel>0) yVel=0;
    if(curDir==1 || curDir==-1){
        yVel=getHeight();
        xVel=curDir*speed;
        curDir=0;
    }
    if(velOutOfBounds()==OOBERR_PX) curDir=-1;
    else if(velOutOfBounds()==OOBERR_NX) curDir=1;
}

/// SHOOTING ENEMIES ///
///paul ship functions
//constructors
PaulShip::PaulShip():Thing(gTextures[TXTR_PAUL],COLL_SHIP,CFACT_ENEMY,5,1),SShip(MOVE_SPECIAL,SHOOT_SPECIAL,SPEED_CHARGE,new BasicProjectile(CFACT_ENEMY,0,SPEED_PROJ)){start(SPEED_SHIP,SHOOT_TIMES);}
PaulShip::PaulShip(int x,int y):Thing(x,y,gTextures[TXTR_PAUL],COLL_SHIP,CFACT_ENEMY,5,1),SShip(MOVE_SPECIAL,SHOOT_SPECIAL,SPEED_CHARGE,new BasicProjectile(CFACT_ENEMY,0,SPEED_PROJ)){start(SPEED_SHIP,SHOOT_TIMES);}
PaulShip::PaulShip(int x,int y,int hp,int speed,int cspeed,int st):Thing(x,y,gTextures[TXTR_PAUL],COLL_SHIP,CFACT_ENEMY,hp,1),SShip(MOVE_SPECIAL,SHOOT_SPECIAL,cspeed,new BasicProjectile(CFACT_ENEMY,0,SPEED_PROJ)){start(speed,st);}
//initialization functions
void PaulShip::start(int spd,int st){
    addFlag(Thing::HURTS_BASE);
    stopBuilding();
    phase=0;
    shootTimes=st;
    speed=spd;
}
//think virtual override
void PaulShip::think(){
    if(phase==0 || phase<phaseMove+PHASE_STAND+1000/chargeSpeed*shootTimes){
        //set random direction and move time
        if(phase==0){
            do{
                phaseMove=rrange(10,MAX_PHASE_MOVE);
            }while(phaseMove*speed>3*BOARD_W/4);
            do{
                redir(rrange(-1,1)*speed,0);
            }while((xVel==0) || velOutOfBounds(phaseMove)!=0);
        }
        //move in that direction for some cyclesSilone
        else if(phase>0 && phase<phaseMove);
        //stop
        else if(phase==phaseMove) redir(0,0);
        //stand still for some cycles
        else if(phase>phaseMove && phase<phaseMove+PHASE_STAND);
        //start shooting
        else if(phase==phaseMove+PHASE_STAND){
            startBuilding();
        }
        //continue shooting
        else if(phase>PHASE_STAND && phase<phaseMove+PHASE_STAND+1000/chargeSpeed*shootTimes);
        phase++;
    }
    else{
        stopBuilding();
        phase=0;
    }
}
///billy ship functions
//constructors
BillyShip::BillyShip():Thing(gTextures[TXTR_BILLY],COLL_SHIP,CFACT_ENEMY,3,1),SShip(MOVE_SPECIAL,SHOOT_SPECIAL,10,0,CHARGE_SPEED,new TempProjectile(CFACT_ENEMY,PROJ_SPEED_X,PROJ_SPEED_Y,1,PROJ_DURATION)){start(20,50,20,3);}
BillyShip::BillyShip(int x,int y):Thing(x,y,gTextures[TXTR_BILLY],COLL_SHIP,CFACT_ENEMY,3,1),SShip(MOVE_SPECIAL,SHOOT_SPECIAL,10,0,CHARGE_SPEED,new TempProjectile(CFACT_ENEMY,PROJ_SPEED_X,PROJ_SPEED_Y,1,PROJ_DURATION)){start(20,50,20,3);}
BillyShip::BillyShip(int x,int y,int hp,int cdam,int xVel,int yVelMult,int bombChargeTime,int bombNumber):Thing(x,y,gTextures[TXTR_BILLY],COLL_SHIP,CFACT_ENEMY,hp,cdam),SShip(MOVE_SPECIAL,SHOOT_SPECIAL,xVel,0,CHARGE_SPEED,new TempProjectile(CFACT_ENEMY,PROJ_SPEED_X,PROJ_SPEED_Y,1,PROJ_DURATION)){start(yVelMult,yVelMult,bombChargeTime,bombNumber);}
BillyShip::BillyShip(int x,int y,int hp,int cdam,int xVel,int yVelMinMult,int yVelMaxMult,int bombChargeTime,int bombNumber):Thing(x,y,gTextures[TXTR_BILLY],COLL_SHIP,CFACT_ENEMY,hp,cdam),SShip(MOVE_SPECIAL,SHOOT_SPECIAL,xVel,0,CHARGE_SPEED,new TempProjectile(CFACT_ENEMY,PROJ_SPEED_X,PROJ_SPEED_Y,1,PROJ_DURATION)){start(yVelMinMult,yVelMaxMult,bombChargeTime,bombNumber);}
//destructor(s)
BillyShip::~BillyShip(){
    cout<<"destroying BillyShip"<<endl;
}
//initialization functions
void BillyShip::start(int yVelMinMult,int yVelMaxMult,int bombChargeTime,int bombNumber){
    this->yVelMinMult=yVelMinMult;
    this->yVelMaxMult=yVelMaxMult;
    if(yVelMaxMult<yVelMinMult) yVelMaxMult=yVelMinMult;
    this->bombChargeTime=bombChargeTime;
    this->bombNumber=bombNumber;
    stopBuilding();
    bombTimer.start();
    yVelTimer.start();
    yVelCurMult=rrange(yVelMinMult,yVelMaxMult);
    yPosStart=yPos;
}
//think virtual override
void BillyShip::think(){
    //handle velocity (apply y acceleration)
    double yVelSin=sin((PI/50)*(yVelTimer.getCycles()-25));
    yVel=yVelCurMult*yVelSin;
    if(yVelTimer.getCycles()==100){
        yVelCurMult=rrange(yVelMinMult,yVelMaxMult);
        yVelTimer.reset();
    }
    //handle velocity (invert x)
    if(velOutOfBounds()) xVel*=-1;
    //adjust position and velocity
    if(yVelTimer.getCycles()-25==-25 || yVelTimer.getCycles()-25==25) yPos=yPosStart;
    if(yVelTimer.getCycles()-25==0) yVelCurMult+=1.5;
    if(yVelTimer.getCycles()-25==50) yVelCurMult-=1.5;
    //handle bombing
    if(bombTimer.getCycles()>=bombChargeTime){
        startBuildingNow();
        if(bombTimer.getCycles()>=bombChargeTime+(1000/chargeSpeed)*bombNumber){
            bombTimer.reset();
            stopBuilding();
        }
    }
}

/// FRIENDS ///
///henry ship functions
//constructors
HenryShip::HenryShip():Thing(gTextures[TXTR_HENRY],COLL_SHIP,CFACT_FRIEND,3,1),SShip(MOVE_SPECIAL,SHOOT_SPECIAL,SPEED_CHARGE,new BasicProjectile(0,-15)){
    start();
}
HenryShip::HenryShip(int x,int y):Thing(x,y,gTextures[TXTR_HENRY],COLL_SHIP,CFACT_FRIEND,3,1),SShip(MOVE_SPECIAL,SHOOT_SPECIAL,SPEED_CHARGE,new BasicProjectile(0,-15)){
    start();
}
//initialization functions
void HenryShip::start(){
    stopBuilding();
    phase=0;
}
//think virtual override
void HenryShip::think(){
    if(phase<=PHASE_STAND+1000/chargeSpeed*15){
        //set random direction
        if(phase==0){
            do{
                redir(rrange(-1,1)*rrange(10,20),rrange(-1,1)*rrange(5,10));
            }while((xVel==0 && yVel==0) || velOutOfBounds(0+LIMIT_SIDES,BOARD_H-SCREEN_H+LIMIT_MAXALTITUDE,BOARD_W-LIMIT_SIDES,BOARD_H-LIMIT_MINALTITUDE,PHASE_MOVE)!=0);
        }
        //move in that direction for some cycles
        else if(phase>0 && phase<PHASE_MOVE);
        //stop
        else if(phase==PHASE_MOVE) redir(0,0);
        //stand still for some cycles
        else if(phase>PHASE_MOVE && phase<PHASE_STAND);
        //shoot 8 projectiles in a 360 fashion
        else if(phase==PHASE_STAND){
            projectile->redir(SPEED_PROJ,0);                                                                    ///east
            startBuilding();
        }
        else if(phase==PHASE_STAND+1000/chargeSpeed*1) projectile->redir(SPEED_PROJ*COSP8,-SPEED_PROJ*SINP8);   ///north-east-east
        else if(phase==PHASE_STAND+1000/chargeSpeed*2) projectile->redir(SPEED_PROJ*COSP4,-SPEED_PROJ*SINP4);   ///north-east
        else if(phase==PHASE_STAND+1000/chargeSpeed*3) projectile->redir(SPEED_PROJ*SINP8,-SPEED_PROJ*COSP8);   ///north-north-east
        else if(phase==PHASE_STAND+1000/chargeSpeed*4) projectile->redir(0,-SPEED_PROJ);                        ///north
        else if(phase==PHASE_STAND+1000/chargeSpeed*5) projectile->redir(-SPEED_PROJ*SINP8,-SPEED_PROJ*COSP8);  ///north-north-west
        else if(phase==PHASE_STAND+1000/chargeSpeed*6) projectile->redir(-SPEED_PROJ*COSP4,-SPEED_PROJ*SINP4);  ///north-west
        else if(phase==PHASE_STAND+1000/chargeSpeed*7) projectile->redir(-SPEED_PROJ*COSP8,-SPEED_PROJ*SINP8);  ///north-west-west
        else if(phase==PHASE_STAND+1000/chargeSpeed*8) projectile->redir(-SPEED_PROJ,0);                        ///west
        else if(phase==PHASE_STAND+1000/chargeSpeed*9) projectile->redir(-SPEED_PROJ*COSP8,SPEED_PROJ*SINP8);   ///south-west-west
        else if(phase==PHASE_STAND+1000/chargeSpeed*10) projectile->redir(-SPEED_PROJ*COSP4,SPEED_PROJ*SINP4);  ///south-west
        else if(phase==PHASE_STAND+1000/chargeSpeed*11) projectile->redir(-SPEED_PROJ*SINP8,SPEED_PROJ*COSP8);  ///south-south-west
        else if(phase==PHASE_STAND+1000/chargeSpeed*12) projectile->redir(0,SPEED_PROJ);                        ///south
        else if(phase==PHASE_STAND+1000/chargeSpeed*13) projectile->redir(SPEED_PROJ*SINP8,SPEED_PROJ*COSP8);   ///south-south-east
        else if(phase==PHASE_STAND+1000/chargeSpeed*14) projectile->redir(SPEED_PROJ*COSP4,SPEED_PROJ*SINP4);   ///south-east
        else if(phase==PHASE_STAND+1000/chargeSpeed*15) projectile->redir(SPEED_PROJ*COSP8,SPEED_PROJ*SINP8);   ///south-east-east
        phase++;
    }
    else{
        stopBuilding();
        phase=0;
    }
    lifePhase++;
}
//ccheck virtual override
void HenryShip::ccheck(){
    if(lifePhase>1000){
        despawn();
    }
}

///baby destroyer ship functions
//constructor
BabyDestroyerShip::BabyDestroyerShip():Thing(gTextures[TXTR_BABY_DESTROYER],COLL_SHIP,CFACT_FRIEND,1,1),SShip(MOVE_SPECIAL,SHOOT_SPECIAL,0,-5,1000,new BasicProjectile(CFACT_ENEMY,0,20)),MaterialProduct(15.0){start();}
//initialization function
void BabyDestroyerShip::start(){
    stopBuilding();
    modelSpawnXPos=width/2;
    modelSpawnYPos=height+1;
    addFlag(Thing::NO_SPACE_NEEDED_WHEN_BUILT);
}
//think
void BabyDestroyerShip::think(){
    if(lifeTimer.getCycles()>=100) startBuildingNow();
}
//ccheck
void BabyDestroyerShip::ccheck(){
    if(lifeTimer.getCycles()>=100) despawn();
}

/// NEUTRAL ///
///Monty ship functions
//constructor
MontyShip::MontyShip(int x,int y,int hp,int cdam,int speed,int res,int modTime):Thing(x,y,gTextures[TXTR_MONTY],COLL_SHIP,CFACT_FRIEND,hp,cdam),SliderShip(x,y,gTextures[TXTR_MONTY],hp,cdam,speed){start(res,modTime);}
//initialization functions
void MontyShip::start(int res,int modTime){
    this->modTime=modTime;
    this->res=res;
    mod=false;
    lifeTimer.start();
}
//think
void MontyShip::think(){
    //switch modes
    if(lifeTimer.getCycles()>=modTime) mod=true;
    //slider mode
    if(mod==false) SliderShip::think();
    //suicide mode
    else{
        redir(0,speed);
    }
}
//ccheck
void MontyShip::ccheck(){
    //generate mountain if destroyed
    if(velOutOfBounds() & OOBERR_PY){
        float x;
        int i = (res<=100 ? 0 : 1);
        x=xPos-Resource::bgTextures[Resource::ROCK][i]->getWidth()/2;
        BGResourceContainer tmp(x<=0 ? 0 : x);
        tmp.addResource(Resource::ROCK,res);
        create(new BGResourceContainer(tmp));
        despawn();
    }
}
