#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <SDL.h>

///texture class
class Texture{
    public:
        //constructors and deconstructors
        Texture();
        ~Texture();
        //getter and setters
        int getWidth(){return width;}
        int getHeight(){return height;}
        //other functions
        void reset();
        void load(std::string path);
        void show();
        void show(int x,int y);
        void show(int x,int y,int w,int h);
    private:
        //width and height
        int width;
        int height;
        //SDL texture
        SDL_Texture* t;
};

#endif // TEXTURE_H
