#ifndef INTERFACE_H
#define INTERFACE_H

#include <string>
#include <sstream>
#include <cstring>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Texture.h"
#include "Thing.h"

/// OUTPUT ///
//number box class
class NumBox:public Texture{
    public:
        //constructors
        NumBox();
        NumBox(int r,int g,int b,int maxDigits);
        NumBox(TTF_Font *font,int r,int g,int b,int maxDigits);
        NumBox(std::string fontPath,int size,int r,int g,int b,int maxDigits);
        NumBox(int r,int g,int b,int maxDigits,std::string title);
        NumBox(TTF_Font *font,int r,int g,int b,int maxDigits,std::string title);
        NumBox(std::string fontPath,int size,int r,int g,int b,int maxDigits,std::string title);
        //output functions
        void show(int x,int y,int n);
        void show(int x,int y,std::string s);
    private:
        //initialization functions
        void start(TTF_Font *font,SDL_Color *color,int maxDigits,std::string title);
        //used font
        TTF_Font *font;
        //used color
        SDL_Color *color;
        //max number of digits that the box can store before overflowing
        int maxDigits;
        //title (optional)
        std::string title;
};

//texture box class
class TextureBox{
    public:
        //constructors
        TextureBox(std::vector<Texture*> txtrs);
        TextureBox(std::vector<Thing*> thngs);
        //destructor
        ~TextureBox();
        //getters
        Texture *getCurTexture(){return textures[curTextureI];}
        //setters
        void setCurTextureI(int i){if(i>=0 && i<textures.size()) curTextureI=i;}
        //other functions
        void next();
        void prev();
        void add(Texture* txtr);
        void add(Thing* thng);
    private:
        //textures
        std::vector<Texture*> textures;
        //texture index
        int curTextureI;
};

#endif
