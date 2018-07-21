#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include <cassert>
#include <cstddef>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Window.h"

///false constants
//screen dimensions
#define SCREEN_W Texture::getScreenWidth()
#define SCREEN_H Texture::getScreenHeight()

///constants
extern const int BOARD_W;
extern const int BOARD_H;

///texture class
class Texture{
    public:
        //friend classes
        friend class Sprite;
        friend class Animation;
        //static variables and classes
        static Window *gWindow;
        static TTF_Font *gFont;
        //constructors
        Texture();
        Texture(SDL_Texture *t);
        Texture(std::string path);
        //copy constructor
        Texture(Texture &texture);
        //destructor(s)
        virtual ~Texture();
        //cloning function
        Texture *clone(){return new Texture(*this);}
        //getters
        int getWidth(){return width;}
        int getHeight(){return height;}
        SDL_Texture *sdlT(){return texture;}
        //static getters
        static SDL_Renderer *getRenderer(){return gWindow->sdlR();}
        static int getScreenWidth(){return gWindow->getWidth()/zoomRatio;}
        static int getScreenHeight(){return gWindow->getHeight()/zoomRatio;}
        //setters
        static void setDefaultLoadPath(std::string path){gPath=path;}
        //pseudo-setters
        bool load(std::string path);
        bool loadFromText(std::string text);
        bool loadFromText(std::string text,SDL_Color *color);
        bool loadFromText(TTF_Font *font,std::string text,SDL_Color *color);
        bool loadFromText(std::string text,int r,int g,int b);
        bool loadFromText(TTF_Font *font,std::string text,int r,int g,int b);
        void setColor(Uint8 r,Uint8 g,Uint8 b);
        //global initialization function
        static void gStart(Window *window,TTF_Font *font);
        //static functions
        static void zoom(double zr);
        static void updateRenderer();
        static void clearRenderer();
        //other functions
        void show(SDL_Rect *selectRect,SDL_Rect *showRect);
        void show(int x,int y);
    private:
        //static variables and classes
        static std::string gPath;
        static double zoomRatio;
        static SDL_Color keyColor;
        //global initialization dedicated variables
        static bool globalInit;
        //dimensions
        int width,height;
        //original SDL texture
        SDL_Texture *texture;
        //start function
        void start(SDL_Texture *t,int width,int height);
        //setters
        void set(SDL_Texture *t);
        void set(Texture *t);
};

#endif
