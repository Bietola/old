#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <cmath>

#include <SDL.h>

#include "mathutils.h"

#include "grappa/Texture.h"
#include "grappa/Window.h"

///global enumerations
//global texture
enum GTXTR{
    GTXTR_FOOD,
    GTXTR_NNCOLLECTOR,
    GTXTR_RANDCOLLECTOR,
    GTXTR_MAX
};

///global variables
//global window
extern Window* gWindow;
//global textures
extern Texture* gTextures[GTXTR_MAX];

///initialization functions
//start SDL
void start_SDL();
//start simulation parameters
void start_sim();

#endif // GLOBALS_H
