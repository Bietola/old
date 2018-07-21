#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

///Sprite class
class Sprite{
    public:
        //constructor
        Sprite(SDL_Rect showBox);
        //other functions
        virtual void show()=0;
    protected:
        //show box
        SDL_Rect showBox;
        //initialization function
        void start(SDL_Rect sBox);
};

#endif
