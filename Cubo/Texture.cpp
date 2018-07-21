#include <iostream>
#include <string>
#include <SDL.h>

#include "globals.h"

#include "Texture.h"

///texture functions
//constructor and deconstructor
Texture::Texture(){
    reset();
}
Texture::~Texture(){
    reset();
    SDL_DestroyTexture(t);
}
//reset the texture
void Texture::reset(){
    width=0;
    height=0;
    t=NULL;
}
//load the texture form a file
void Texture::load(std::string path){
    //reset texture if needed
    if(t!=NULL) reset();
    //load surface
    path=loadPath+path;
    SDL_Surface* s=IMG_Load(path.c_str());
    if(s==NULL){
        std::cout<<"Could not load the image: "<<path<<"\n";
        return;
    }
    //get width and height
    width=s->w;
    height=s->h;
    //convert surface to texture
    t=SDL_CreateTextureFromSurface(gRenderer,s);
    SDL_FreeSurface(s);
}
//show the texture as background
void Texture::show(){
    show(0,0,SCREEN_W,SCREEN_H);
}
//show the texture on the screen
void Texture::show(int x,int y){
    show(x,y,width,height);
}
//stretch and show the texture on the screen
void Texture::show(int x,int y,int w,int h){
    SDL_Rect r={x,y,w,h};
    SDL_RenderCopy(gRenderer,t,NULL,&r);
}
