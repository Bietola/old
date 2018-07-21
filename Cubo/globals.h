#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>

#include "Texture.h"
#include "Adv.h"

///constants
#define SCREEN_W 750
#define SCREEN_H 500

///custom types
//types of tiles
enum TILE{
    TILE_INTRO,
    TILE_ADV,
    TILE_MAX,
};

///global data
//game window and renderer
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;
//default loading path
extern std::string loadPath;
//game obstacles (walls)
extern std::vector<SDL_Rect> gWalls;

///texture objects
//game tiles
extern Texture tiles[TILE_MAX];

///adv objects
//main controllable entity
extern Adv you;

///global functions
void start_game();
bool check_boxCollision(SDL_Rect&,SDL_Rect&);

#endif // GLOBALS_H
