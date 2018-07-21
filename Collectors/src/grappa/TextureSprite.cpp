#include "TextureSprite.h"

///texture sprite functions
//constructor
TextureSprite::TextureSprite(Texture *texture,SDL_Rect showBox):Sprite(showBox){
    start(texture);
}
//initialization functions
void TextureSprite::start(Texture *txtr){
    texture=txtr;
}
//show the sprite's texture (applying the show box)
void TextureSprite::show(){
    texture->show(NULL,&showBox);
}

