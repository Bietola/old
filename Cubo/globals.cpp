#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_image.h>

#include "Texture.h"
#include "Adv.h"

#include "globals.h"

///global data
//game window and renderer
SDL_Window* gWindow;
SDL_Renderer* gRenderer;
//default loading path
std::string loadPath="";
//game obstacles (walls)
std::vector<SDL_Rect> gWalls;

///texture objects
//game tiles
Texture tiles[TILE_MAX];

///adv objects
//main controllable entity
Adv you;

///global functions
//set up SDL and start the game
void start_game(){
    SDL_Event e;
    //set random seed
    srand(time(NULL));
    //set up SDL
    if(SDL_Init(SDL_INIT_VIDEO)<0){
        std::cout<<"Could not start SDL\n";
        return;
    }
    //!TODO SDL_SetHint(...);
    //set up SDL_image
    if(!IMG_Init(IMG_INIT_PNG)){
        std::cout<<"Could not start SDL_image\n";
        return;
    }
    //set up window and renderer
    gWindow=SDL_CreateWindow("GTB",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SCREEN_W,SCREEN_H,SDL_WINDOW_SHOWN);
    gRenderer=SDL_CreateRenderer(gWindow,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    //load tiles
    loadPath="tiles/";
    tiles[TILE_INTRO].load("intro.png");
    tiles[TILE_ADV].load("adv.png");
    //!TODO load other tiles
    //set up adv
    you.speed=3;
    you.setTexture(tiles[TILE_ADV]);
    //set up walls
    gWalls.resize(2);
    gWalls[0]={100,100,300,300};
    gWalls[1]={500,100,200,300};
    //show game intro
    tiles[TILE_INTRO].show();
    SDL_RenderPresent(gRenderer);
    //wait for user input
    while(e.type!=SDL_KEYDOWN) SDL_PollEvent(&e);
}
//check a collision between two rectangles (collision boxes)
bool check_boxCollision(SDL_Rect& r1,SDL_Rect& r2){
    //y collision
    if((r2.y>r1.y && r2.y<r1.y+r1.h) || (r1.y>r2.y && r1.y<r2.y+r2.h)){
        //x collision
        if((r2.x>r1.x && r2.x<r1.x+r1.w) || (r1.x>r2.x && r1.x<r2.x+r2.w)) return true;
    }
    return false;
}
