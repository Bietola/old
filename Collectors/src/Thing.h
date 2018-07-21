#ifndef THING_H
#define THING_H

#include <vector>
#include <SDL.h>

#include "globals.h"
#include "grappa/Texture.h"

///THING class
class Thing{
    public:
        //enumerations
        enum COLLTYPE{
            CT_COLLECTOR,
            CT_FOOD,
        };
        //static variables
        static std::vector<Thing*> things;
        static int maxThing;
        static int maxSpawnedThing;
        static int frameTime;
        static const int memExpand;
        //constructors
        Thing();
        Thing(Texture *texture,int xPos,int yPos,COLLTYPE collisionType);
        //destructor
        virtual ~Thing();
        //getters
        int getXPos(){return cBox.x;}
        int getYPos(){return cBox.y;}
        int getWidth(){return cBox.w;}
        int getHeight(){return cBox.h;}
        SDL_Rect getCBox(){return cBox;}
        COLLTYPE getCollisionType(){return collisionType;}
        int getID(){return id;}
        int getSpawnID(){return spawnId;}
        //setters
        void setTexture(Texture* txtr);
        //pseudo-setters
        virtual void repos(int x,int y){cBox.x=x;cBox.y=y;}
        void reposx(int x){repos(x,cBox.y);}
        void reposy(int y){repos(cBox.x,y);}
        void addpos(int x,int y){cBox.x+=x;cBox.y+=y;}
        //static void setFrameTime(int ft){frameTime=ft;}
        //other functions
        void spawn();
        void spawn(int x,int y);
        void clone();
        void clone(int x,int y);
        void show();
        //static functions
        template<class T> static void create(T thing);
        static int nextSpawnId();
        //friend functions
        friend bool checkCollision(Thing* thing1,Thing* thing2);
        //virtual functions
        virtual void despawn();
        //pure virtual functions
        virtual void act()=0;
        virtual Thing *retClone()=0;
    protected:
        //texture
        Texture *texture;
        //collision box
        SDL_Rect cBox;
        //identification number
        int id;
        //index in the things vector
        int spawnId;
        //collision type
        COLLTYPE collisionType;
        //initialization function
        void start(Texture* t,int x,int y,COLLTYPE ct);
};

///template functions
//add copy of thing to things
template<class T> void Thing::create(T model){
    T clone=model;
    clone->spawn();
}

#endif
