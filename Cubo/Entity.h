#ifndef ENTITY_H
#define ENTITY_H

#include <SDL.h>

#include "Texture.h"

///entity class
class Entity{
    public:
        //public variables
        int speed;
        //constructors and deconstructors
        Entity();
        ~Entity();
        //getters and setters
        int getXpos(){return xPos;}
        int getYpos(){return yPos;}
        int getXvel(){return xVel;}
        int getYvel(){return yVel;}
        //other functions
        void reset();
        void setTexture(Texture t);
        void show();
    protected:
        //position
        int xPos;
        int yPos;
        //movement vector (velocity)
        int xVel;
        int yVel;
        //collision box
        SDL_Rect cBox;
        //texture
        Texture t;
};

#endif // ENTITY_H
