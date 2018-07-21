#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SDL.h>

///window class
class Window{
    public:
        //constructors
        Window();
        Window(std::string name,int width,int height);
        Window(std::string name,int x,int y,int width,int height);
        Window(std::string name,int width,int height,Uint32 flags);
        Window(std::string name,int x,int y,int width,int height,Uint32 flags);
        //destructor(s)
        ~Window();
        //getters
        int getWidth(){return width;}
        int getHeight(){return height;}
        SDL_Window *sdlW(){return window;}
        SDL_Renderer *sdlR(){return renderer;}
        //other functions
        bool handleEvent(SDL_Event *e);
    protected:
        //dimensions
        int width,height;
        //windows focus
        bool fullScreen;
        //original SDL window
        SDL_Window *window;
        //original SDL renderer used by the window
        SDL_Renderer *renderer;
        //start function
        void start(std::string name,int x,int y,int width,int height,Uint32 flags);
};

#endif // WINDOW_H
