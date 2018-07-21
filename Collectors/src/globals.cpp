#include "globals.h"

///global variables
Texture* gTextures[GTXTR_MAX];
Window* gWindow;

///initialization functions
//set up SDL and graphics
void start_SDL(){
    //initialize sdl
    if(SDL_Init(SDL_INIT_VIDEO)<0) std::cout << "ERROR: could not initialize SDL" << std::endl;
    //initialize window and textures
    gWindow=new Window("Collectors",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1000,750,SDL_WINDOW_SHOWN);
    Texture::gStart(gWindow,NULL);
    //load global textures
    Texture::setDefaultLoadPath("assets/");
    gTextures[GTXTR_FOOD]=new Texture("food.bmp");
    gTextures[GTXTR_NNCOLLECTOR]=new Texture("nn collector.bmp");
    gTextures[GTXTR_RANDCOLLECTOR]=new Texture("rand collector.bmp");
}
//set up simulation
void start_sim(){
    ;
}
