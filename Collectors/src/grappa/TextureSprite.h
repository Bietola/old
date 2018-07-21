#ifndef TEXTURESPRITE_H
#define TEXTURESPRITE_H

#include "Texture.h"
#include "Sprite.h"

///texture sprite class
class TextureSprite: public Sprite{
    public:
        //constructor
        TextureSprite(Texture *texture,SDL_Rect showBox);
        //virtual functions fulfillment
        void show();
    private:
        //texture
        Texture *texture;
        //initialization functions
        void start(Texture *txtr);
};


#endif // TEXTURESPRITE_H
