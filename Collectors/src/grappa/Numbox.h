#ifndef NUMBOX_H
#define NUMBOX_H

#include <cstring>
#include <sstream>
#include <string>
#include <SDL.h>

#include "Texture.h"

///number box class
class NumBox{
    public:
        //constructors
        NumBox();
        NumBox(int r,int g,int b,int maxDigits);
        NumBox(TTF_Font *font,int r,int g,int b,int maxDigits);
        NumBox(std::string fontPath,int size,int r,int g,int b,int maxDigits);
        NumBox(int r,int g,int b,int maxDigits,std::string title);
        NumBox(TTF_Font *font,int r,int g,int b,int maxDigits,std::string title);
        NumBox(std::string fontPath,int size,int r,int g,int b,int maxDigits,std::string title);
        NumBox(std::string fontPath,int size,int r,int g,int b,int maxDigits,std::string title,Texture *backGround);
        //output functions
        void show(int x,int y,int n);
        void show(int x,int y,std::string s);
    private:
        //initialization functions
        void start(TTF_Font *font,SDL_Color *color,int maxDigits,std::string title,Texture *bg);
        //used font
        TTF_Font *font;
        //used color
        SDL_Color *color;
        //number texture
        Texture *numTexture;
        //optional background
        Texture *backGround;
        //max number of digits that the box can store before overflowing
        int maxDigits;
        //title (optional)
        std::string title;
};

#endif // NUMBOX_H
