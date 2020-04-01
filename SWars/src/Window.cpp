#include "Window.h"

//constructors
Window::Window(){
    start("Unnamed",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,SDL_WINDOW_SHOWN);
}
Window::Window(std::string name,int width,int height){
    start(name,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,SDL_WINDOW_SHOWN);
}
Window::Window(std::string name,int x,int y,int width,int height){
    start(name,x,y,width,height,SDL_WINDOW_SHOWN);
}
Window::Window(std::string name,int x,int y,int width,int height,Uint32 flags){
    start(name,x,y,width,height,flags);
}
Window::Window(std::string name,int width,int height,Uint32 flags){
    start(name,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,width,height,flags);
}
//destructor(s)
Window::~Window(){
    SDL_DestroyWindow(window);
}
//initialization functions
void Window::start(std::string name,int x,int y,int width,int height,Uint32 flags){
    this->width=width;
    this->height=height;
    window=SDL_CreateWindow(name.c_str(),x,y,width,height,flags);
    renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
}
//handle window events (return true if the window is closed)
bool Window::handleEvent(SDL_Event *e){
    if(e->type==SDL_QUIT) return true;
    if(e->type==SDL_WINDOWEVENT){
        if(e->window.event==SDL_WINDOWEVENT_SIZE_CHANGED){
            width=e->window.data1;
            height=e->window.data2;
            SDL_RenderPresent(renderer);
        }
        else if(e->window.event==SDL_WINDOWEVENT_EXPOSED){
            SDL_RenderPresent(renderer);
        }
    }
    return false;
}
