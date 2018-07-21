#ifndef ADV_H
#define ADV_H

#include <vector>
#include <SDL.h>

#include "Entity.h"

///adv class
class Adv : public Entity{
    public:
        //constructors and deconstructors
        Adv();
        ~Adv();
        //other functions
        void handleEvent(SDL_Event* e);
        void move(std::vector<SDL_Rect>* walls=NULL);
};

#endif // ADV_H
