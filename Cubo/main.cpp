#include <iostream>
#include <SDL.h>

#include "Texture.h"
#include "Adv.h"
#include "globals.h"

int main(int argc,char* args[]){
    bool quit=false;
    //start game
    start_game();
    //game loop
    SDL_Event e;
    while(!quit){
        //handle input
        while(SDL_PollEvent(&e)>0){
            //quit if requested
            if(e.type==SDL_QUIT){
                quit=true;
            }
            //TEST: modify adv speed if requested
            else if(e.key.keysym.sym=='d') you.speed++;
            else if(e.key.keysym.sym=='a') you.speed--;
            //handle adv events
            else you.handleEvent(&e);
        }
        //move adv
        you.move(&gWalls);
        //clear screen
        SDL_SetRenderDrawColor(gRenderer,0xFF,0xFF,0xFF,0xFF);
        SDL_RenderClear(gRenderer);
        //draw walls
        SDL_SetRenderDrawColor(gRenderer,0xFF,0x00,0xFF,0xFF);
        SDL_RenderDrawRect(gRenderer,&gWalls[0]);
        SDL_RenderFillRect(gRenderer,&gWalls[1]);
        //show adv on the screen
        you.show();
        //update screen
        SDL_RenderPresent(gRenderer);
    }
    return 0;
}
