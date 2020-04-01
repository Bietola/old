#include "SpecialCannons.h"

/// FRIENDS ///
///bob cannon functions
//constructors
BobCannon::BobCannon():Thing(gTextures[TXTR_BOBCANNON],COLL_SHIP,CFACT_FRIEND,2,1),Cannon(new BasicProjectile(0,-15),10,SHOOT_NOTHINK),MaterialProduct(50.0){}
BobCannon::BobCannon(int x,int y):Thing(x,y,gTextures[TXTR_BOBCANNON],COLL_SHIP,CFACT_FRIEND,1,1),Cannon(new BasicProjectile(0,-15),10,SHOOT_NOTHINK),MaterialProduct(50.0){}

///mitra-bob cannon functions
//constructors
MitraBobCannon::MitraBobCannon():Thing(gTextures[TXTR_FRANKCANNON],COLL_SHIP,CFACT_FRIEND,1,1),Cannon(new BasicProjectile(CFACT_FRIEND,0,-25),50,SHOOT_SPECIAL),MaterialProduct(100.0){}
MitraBobCannon::MitraBobCannon(int x,int y):Thing(x,y,gTextures[TXTR_FRANKCANNON],COLL_SHIP,CFACT_FRIEND,1,1),Cannon(new BasicProjectile(CFACT_FRIEND,0,-25),50,SHOOT_SPECIAL),MaterialProduct(100.0){}

///henry cannon functions
//constructors
HenryCannon::HenryCannon():Thing(gTextures[TXTR_HENRYCANNON],COLL_SHIP,CFACT_FRIEND,5,1),Cannon(new HenryShip(),0.5,SHOOT_SPECIAL),MaterialProduct(500.0){
    start();
}
HenryCannon::HenryCannon(int x,int y):Thing(x,y,gTextures[TXTR_HENRYCANNON],COLL_SHIP,CFACT_FRIEND,5,1),Cannon(new HenryShip(),0.5,SHOOT_SPECIAL),MaterialProduct(500.0){
    start();
}
//initialization functions
void HenryCannon::start(){
    startBuilding();
}

///astra cannon functions
//constructors
AstraCannon::AstraCannon():Thing(gTextures[TXTR_ASTRACANNON],COLL_SHIP,CFACT_FRIEND,1,1),Cannon(new TempProjectile(gTextures[TXTR_ASTRA_PROJ],CFACT_FRIEND,15,7,1,250),width/2,-50,50.0,SHOOT_SPECIAL),MaterialProduct(100.0){
    start();
}
AstraCannon::AstraCannon(int x,int y):Thing(x,y,gTextures[TXTR_ASTRACANNON],COLL_SHIP,CFACT_FRIEND,1,1),Cannon(new TempProjectile(gTextures[TXTR_ASTRA_PROJ],CFACT_FRIEND,15,7,1,250),width/2,-50,50.0,SHOOT_SPECIAL),MaterialProduct(100.0){
    start();
}
//initialization functions
void AstraCannon::start(){
    startBuilding();
}
