#include "Ship.h"

///ship functions
//constructors
Ship::Ship(Texture *texture,int xPos,int yPos,COLLTYPE collisionType):Thing(texture,xPos,yPos,collisionType){
    start();
}
//initialization
void Ship::start(){
    xVel=0;
    yVel=0;
}
//apply velocity to position
void Ship::move(){
    //handle out of bounds position
    if(handleOutOfBounds()){;}
    //move thing and handle collisions
    else{
        //apply velocity to position and check for collisions
        int xxVel=xVel,yyVel=yVel;
        int xFut=getXPos()+xVel;
        int yFut=getYPos()+yVel;
        while(getXPos()!=xFut || getYPos()!=yFut){
            if(xFut>getXPos()) cBox.x++;
            else if(xFut<getXPos()) cBox.x--;
            if(yFut>getYPos()) cBox.y++;
            else if(yFut<getYPos()) cBox.y--;
            cBox.x=getXPos();
            cBox.y=getYPos();
            if(handleCollisions()) break; //(exit loop if thing has been de-spawned)
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
bool Ship::handleCollisions(){
    bool retVal=false;
    for(int j=0;j<maxSpawnedThing;j++){
        if(j!=spawnId && checkCollision(this,things.at(j))){
            retVal = retVal ? true : handleCollision(things.at(j));
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
    if(getXPos()+cBox.w>maxx) err+=OOBERR_PX;
    if(getXPos()<minx) err+=OOBERR_NX;
    if(getYPos()+cBox.h>maxy) err+=OOBERR_PY;
    if(getYPos()<miny) err+=OOBERR_NY;
    return err;
}
//check if ship will be out of bounds when velocity is applied
int Ship::velOutOfBounds(int minx,int miny,int maxx,int maxy){
    int err=0;
    if(getXPos()+xVel+cBox.w>maxx) err+=OOBERR_PX;
    if(getXPos()+xVel<minx) err+=OOBERR_NX;
    if(getYPos()+yVel+cBox.h>maxy) err+=OOBERR_PY;
    if(getYPos()+yVel<miny) err+=OOBERR_NY;
    return err;
}
//check if ship will be out of bounds when velocity is applied
int Ship::velOutOfBounds(int minx,int miny,int maxx,int maxy,int velMult){
    int err=0;
    if(getXPos()+velMult*xVel+cBox.w>maxx) err+=OOBERR_PX;
    if(getXPos()+velMult*xVel<minx) err+=OOBERR_NX;
    if(getYPos()+velMult*yVel+cBox.h>maxy) err+=OOBERR_PY;
    if(getYPos()+velMult*yVel<miny) err+=OOBERR_NY;
    return err;
}
//handle out of bounds collisions
bool Ship::handleOutOfBounds(){
    if(int err=velOutOfBounds()){
        /*if(containsError(err,OOBERR_NY)) reposy(0);
        if(containsError(err,OOBERR_PY)) reposy(BOARD_H-cBox.h);
        if(containsError(err,OOBERR_NX)) reposx(0);
        if(containsError(err,OOBERR_PX)) reposx(BOARD_W-cBox.w);*/
        repos(BOARD_W-getXPos()-getWidth(),BOARD_H-getYPos()-getHeight());
        return true; //out of bounds
    }
    return false; //not out of bounds
}
void Ship::act(){
    //move thing and handle collisions (including out of bounds)
    move();
}
