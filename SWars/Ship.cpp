#include "Ship.h"

#include "GameStates.h" //!needed for base damaging

///ship functions
//constructors
Ship::Ship():Thing(){
    start(MOVE_DUMMY,0,0);
}
Ship::Ship(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam):Thing(x,y,t,ct,cf,hp,cdam){
    start(mt,0,0);
}
Ship::Ship(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam,int vx,int vy):Thing(x,y,t,ct,cf,hp,cdam){
    start(mt,vx,vy);
}
Ship::Ship(Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam):Thing(0,0,t,ct,cf,hp,cdam){
    start(mt,0,0);
}
Ship::Ship(Texture *t,COLLISION ct,COLLFACTION cf,MOVE mt,int hp,int cdam,int vx,int vy):Thing(0,0,t,ct,cf,hp,cdam){
    start(mt,vx,vy);
}
//constructors dedicated to virtual inheritance
Ship::Ship(MOVE mt){
    start(mt,0,0);
}
Ship::Ship(MOVE mt,int vx,int vy){
    start(mt,vx,vy);
}
//initialization
void Ship::start(MOVE mt,int vx,int vy){
    //velocity
    redir(vx,vy);
    //move type
    moveType=mt;
}
//apply velocity to position
void Ship::move(){
    //handle out of bounds position
    if(handleOutOfBounds()){;}
    //move thing and handle collisions
    else{
        //apply velocity to position and check for collisions
        int xFut=xPos+xVel;
        int yFut=yPos+yVel;
        while(xPos!=xFut || yPos!=yFut){
            if(xFut>xPos) xPos++;
            else if(xFut<xPos) xPos--;
            if(yFut>yPos) yPos++;
            else if(yFut<yPos) yPos--;
            cBox.x=xPos;
            cBox.y=yPos;
            if(handleCollision()) break; //(exit loop if thing has been de-spawned)
        }
    }
}
//apply given velocity to position
void Ship::move(float vx,float vy){
    int mxVel=xVel,myVel=yVel;
    xVel=vx;
    yVel=vy;
    move();
    xVel=mxVel;
    yVel=myVel;
}
//handle collisions - return true if the thing that called the function has de-spawned from the game
bool Ship::handleCollision(){
    bool retVal=false;
    for(int j=0;j<maxSpawnedThing;j++){
        if(j!=spawnId && checkCollision(this,things.at(j))){
            COLLISION collType2=things.at(j)->collType;
            COLLFACTION collFaction2=things.at(j)->collFaction;
            if(collFaction!=collFaction2){
                //universal collisions
                //ship collisions
                if(collType==COLL_SHIP){
                    //collision with a projectile
                    if(collType2==COLL_PROJ){
                        if(DEBUG) cout<<"SHIP-PROJECTILE COLLISION: "<<spawnId<<","<<things.at(j)->spawnId<<endl;
                        Thing *proj=things.at(j);
                        hp-=proj->crashDamage;
                        proj->despawn();
                        retVal=false;
                    }
                    //collision with other ship
                    if(collType2==COLL_SHIP){
                        if(DEBUG) cout<<"SHIP-SHIP COLLISION: "<<spawnId<<","<<things.at(j)->spawnId<<endl;
                    }
                    //collision with cannon
                    else if(collType2==COLL_CANNON){
                        if(DEBUG) cout<<"SHIP-CANNON COLLISION: "<<spawnId<<","<<things.at(j)->spawnId<<endl;
                    }
                }
                //projectile collisions
                else if(collType==COLL_PROJ){
                    //collision with a ship
                    if(collType2==COLL_SHIP){
                        if(DEBUG) cout<<"PROJECTILE-SHIP COLLISION: "<<spawnId<<","<<things.at(j)->spawnId<<endl;
                        Thing *ship=things.at(j);
                        ship->hp-=crashDamage;
                        despawn();
                        return true;
                    }
                    //collision with other projectile
                    else if(collType2==COLL_PROJ){
                        if(DEBUG) cout<<"PROJ-PROJ COLLISION: "<<spawnId<<","<<things.at(j)->spawnId<<endl;
                    }
                    //collision with cannon
                    else if(collType2==COLL_CANNON){
                        if(DEBUG) cout<<"PROJ-CANNON COLLISION: "<<spawnId<<","<<things.at(j)->spawnId<<endl;
                    }
                }
                //cannon collisions
                else if(collType==COLL_CANNON){
                    //collision with a ship
                    if(collType2==COLL_SHIP){
                        if(DEBUG) cout<<"CANNON-SHIP COLLISION: "<<spawnId<<","<<things.at(j)->spawnId<<endl;
                    }
                    //collision with other projectile
                    else if(collType2==COLL_PROJ){
                        if(DEBUG) cout<<"CANNON-PROJECTILE COLLISION: "<<spawnId<<","<<things.at(j)->spawnId<<endl;
                    }
                    //collision with cannon
                    else if(collType2==COLL_CANNON){
                        if(DEBUG) cout<<"CANNON-CANNON COLLISION: "<<spawnId<<","<<things.at(j)->spawnId<<endl;
                    }
                }
                //bg collisions
                else if(collType==COLL_BG){
                    //do nothing
                }
            }
            else{
                //!WIP
                ;
            }
        }
    }
    return retVal;
}
//check if ship is out of bounds
int Ship::outOfBounds(){
    return outOfBounds(0,0,BOARD_W,BOARD_H);
}
//check if ship will be out of bounds when velocity is applied
int Ship::velOutOfBounds(){
    return velOutOfBounds(0,0,BOARD_W,BOARD_H);
}
//check if ship will be out of bounds when velocity is applied
int Ship::velOutOfBounds(int velMult){
    return velOutOfBounds(0,0,BOARD_W,BOARD_H,velMult);
}
//check if ship is out of bounds
int Ship::outOfBounds(int minx,int miny,int maxx,int maxy){
    int err=0;
    if(xPos+cBox.w>maxx) err+=OOBERR_PX;
    if(xPos<minx) err+=OOBERR_NX;
    if(yPos+cBox.h>maxy) err+=OOBERR_PY;
    if(yPos<miny) err+=OOBERR_NY;
    return err;
}
//check if ship will be out of bounds when velocity is applied
int Ship::velOutOfBounds(int minx,int miny,int maxx,int maxy){
    int err=0;
    if(xPos+xVel+cBox.w>maxx) err+=OOBERR_PX;
    if(xPos+xVel<minx) err+=OOBERR_NX;
    if(yPos+yVel+cBox.h>maxy) err+=OOBERR_PY;
    if(yPos+yVel<miny) err+=OOBERR_NY;
    return err;
}
//check if ship will be out of bounds when velocity is applied
int Ship::velOutOfBounds(int minx,int miny,int maxx,int maxy,int velMult){
    int err=0;
    if(xPos+velMult*xVel+cBox.w>maxx) err+=OOBERR_PX;
    if(xPos+velMult*xVel<minx) err+=OOBERR_NX;
    if(yPos+velMult*yVel+cBox.h>maxy) err+=OOBERR_PY;
    if(yPos+velMult*yVel<miny) err+=OOBERR_NY;
    return err;
}
//handle out of bounds collisions
bool Ship::handleOutOfBounds(){
    if(int err=velOutOfBounds()){
        if(collType==COLL_PROJ) despawn();
        else{
            if(containsError(err,OOBERR_NY)) reposy(0);
            if(containsError(err,OOBERR_PY)){
                if(hasFlag(HURTS_BASE)){
                    GameLoop::baseHp-=crashDamage;
                    despawn();
                }
                else reposy(BOARD_H-height);
            }
            if(containsError(err,OOBERR_NX)) reposx(0);
            if(containsError(err,OOBERR_PX)) reposx(BOARD_W-width);
        }
        return true; //out of bounds
    }
    return false; //not out of bounds
}
//modify velocity according to movement type
void Ship::think(){
    //projectiles
    if(moveType==MOVE_NOTHINK){
        ;
    }
    //ships
    /*else if(moveType==MOVE_DUMMY){
        do{
            xVel=rrange(-60,60);
            yVel=rrange(-60,60);
        }while(velOutOfBounds());
    }
    else */if(moveType==MOVE_DUMATTACKER){
        xVel=rrange(-30,30);
        yVel=5;
    }
    else if(moveType==MOVE_HSLIDER){
        ;
    }
    else if(moveType==MOVE_SPECIAL){
        /*cout<<"special is thinking"<<endl;*/
    }
}
void Ship::act(){
    //move thing and handle collisions (including out of bounds)
    move();
}
