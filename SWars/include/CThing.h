#ifndef CTHING_H
#define CTHING_H

#include <SDL.h>

#include "Texture.h"
#include "Thing.h"

///Controllable THING class
class CThing:public virtual Thing{
    public:
        //pure virtual functions
        virtual void handleEvent(SDL_Event *e)=0;
        virtual void showInterface()=0;
        virtual void showRelativeToThis(Thing *thing);
};

#endif
