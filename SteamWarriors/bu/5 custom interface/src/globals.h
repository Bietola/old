#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include <cassert>

#include <SFML/Graphics.hpp>

#include "Handler.h"

///global window
extern sf::RenderWindow gWindow;
extern Handler* gGameState;

///tile width and height
const unsigned short TILE_WIDTH = 64;
const unsigned short TILE_HEIGHT = 64;

///prepare game
extern void start_game();

#endif // GLOBALS_H_INCLUDED
