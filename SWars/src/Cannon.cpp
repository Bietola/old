#include "Cannon.h"

///cannon functions
//constructors
Cannon::Cannon():Thing(),Builder(){
    start(NULL,SHOOT_DUMMY);
}
Cannon::Cannon(Texture *t,COLLFACTION cf,SHOOT st,int hp,int cdam,double cs,Ship *proj):Thing(0,0,t,COLL_SHIP,cf,hp,cdam),Builder(proj,cs){
    start(proj,st);
}
Cannon::Cannon(int x,int y,Texture *t,COLLFACTION cf,SHOOT st,int hp,int cdam,double cs,Ship *proj):Thing(x,y,t,COLL_SHIP,cf,hp,cdam),Builder(proj,cs){
    start(proj,st);
}
//destructors
Cannon::~Cannon(){
    //!delete projectile;
    cout<<"destroying Cannon"<<endl;
}
//copy constructor
Cannon::Cannon(Cannon &cannon){
    *this=cannon;
    projectile=dynamic_cast<Ship*>(cannon.projectile->retClone());
    model=projectile;
}
//thing virtual inheritance dedicated constructors
Cannon::Cannon(Ship *proj,double cspeed,SHOOT st):Builder(proj,cspeed){
    start(proj,st);
}
Cannon::Cannon(Ship *proj,int msx,int msy,double cspeed,SHOOT st):Builder(proj,msx,msy,cspeed){
    start(proj,st);
}
//builder virtual inheritance dedicated constructors
Cannon::Cannon(Ship *proj,SHOOT st){
    start(proj,st);
}
//initialization
void Cannon::start(Ship *proj,SHOOT st){
    projectile=proj;
    shootType=st;
}
void Cannon::think(){
    if(shootType==SHOOT_NOTHINK){
        if(!isBuilding()) startBuilding();
    }
}
void Cannon::act(){
    if(buildTimer.getCycles()>=1000.0/chargeSpeed && canBuild){
        //!NB Cannons don't check for collisions when building (unlike builders)
        build();
    }
}
