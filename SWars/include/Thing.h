#ifndef THING_H
#define THING_H

#include <vector>
#include <SDL.h>

#include "globals.h"
#include "Texture.h"

///collision type
enum COLLISION{
    COLL_UNDEF,
    COLL_PROJ,
    COLL_SHIP,
    COLL_CANNON,
    COLL_BG,
};

///collision faction
enum COLLFACTION{
    CFACT_UNDEF,
    CFACT_FRIEND,
    CFACT_ENEMY,
};

///THING class
class Thing:public Texture{
    public:
        //enumerations
        enum FLAG{
            USES_NET                     = 0x1,
            EXTRACTABLE                  = 0x2,
            COLLECTS_RESOURCES           = 0x4,
            INDESTRUCTIBLE               = 0x8,
            HURTS_BASE                   = 0x10,
            NO_SPACE_NEEDED_WHEN_BUILT   = 0x20,
        };
        //friend classes
        friend class Cannon;
        friend class Ship;
        friend class MultiShip;
        friend class MCShip; //!TEMP
        //static variables
        static vector<Thing*> things;
        static int maxThing;
        static int maxSpawnedThing;
        static int frameTime;
        static const int memExpand;
        //constructors
        Thing();
        Thing(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam);
        //constructors dedicated to inheritance
        Thing(Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam);
        //getters
        int getXpos(){return xPos;}
        int getYpos(){return yPos;}
        int getID(){return id;}
        int getSpawnID(){return spawnId;}
        int getHP(){return hp;}
        COLLISION getCollType(){return collType;}
        COLLFACTION getCollFaction(){return collFaction;}
        //pseudo-getters
        bool hasFlag(int flgs){return flags & flgs;}
        //pseudo-setters
        virtual void repos(int x,int y){xPos=x;yPos=y;cBox.x=x;cBox.y=y;}
        void reposx(int x){repos(x,yPos);}
        void reposy(int y){repos(xPos,y);}
        void addpos(int x,int y){xPos+=x;yPos+=y;cBox.x+=x;cBox.y+=y;}
        void setTexture(Texture *t);
        //setters
        void setFlags(int flags){this->flags=flags;}
        void addFlag(FLAG flag){flags|=flag;}
        //static void setFrameTime(int ft){frameTime=ft;}
        //other functions
        void start(int x,int y,COLLISION ct,COLLFACTION cf,int hp,int cdam);
        void spawn();
        void spawn(int x,int y);
        void clone();
        void clone(int x,int y);
        void show();
        void showRelativeToField(int fx,int fy);
        void showRelativeToField(int fx,int fy,int fw,int fh);
        void thkact();
        void live();
        //static functions
        template<class T> static void create(T thing);
        static int nextSpawnId();
        //friend functions
        friend bool checkCollision(Thing* thing1,Thing* thing2);
        //virtual functions
        virtual void despawn();
        void check();
        //pure virtual functions
        virtual void act()=0;
        virtual void think()=0;
        virtual Thing *retClone()=0;
    protected:
        //position on screen
        float xPos,yPos;
        //collision box
        SDL_Rect cBox;
        //collision type and faction
        COLLISION collType;
        COLLFACTION collFaction;
        //various stats
        int hp;
        int crashDamage;
        //flags
        int flags;
        //identification number
        int id;
        //index in the things vector
        int spawnId;
        //optional functions
        virtual void ccheck(){;}
};

///template functions
//add copy of thing to things
template<class T> void Thing::create(T model){
    T clone=model;
    clone->spawn();
}

#endif
