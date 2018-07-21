#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include <cstddef>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Window.h"

///false constants
//screen dimensions
#define SCREEN_W Texture::getScreenWidth()
#define SCREEN_H Texture::getScreenHeight()

/*DEBUG*/#include <conio.h>
/*DEBUG*/using namespace std;

///constants
extern const int BOARD_W;
extern const int BOARD_H;

///texture class
class Texture{
    public:
        //static variables and classes
        static Window *gWindow;
        static TTF_Font *gFont;
        //constructors
        Texture();
        Texture(SDL_Texture *t);
        Texture(std::string path);
        //destructor(s)
        virtual ~Texture();
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
        //global initialization function
        static void gStart(Window *window,TTF_Font *font);
        //static functions
        static void zoom(double zr);
        static void update();
        //other functions
        void show();
        void show(int x,int y);
        void show(int x,int y,int w,int h);
        void show(SDL_Rect *r);
        void showRelativeToField(int x,int y,int fx,int fy);
        void showRelativeToField(int x,int y,int fx,int fy,int fw,int fh);
        void showRelativeToViewer(int x,int y,int vx,int vy);
    protected:
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
