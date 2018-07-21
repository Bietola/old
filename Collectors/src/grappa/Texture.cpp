#include "Texture.h"

///constants
//board dimensions
const int BOARD_W=1000;
const int BOARD_H=750;

///texture class static variables initialization
std::string Texture::gPath="";
double Texture::zoomRatio=1;
bool Texture::globalInit=false;
SDL_Color Texture::keyColor={0,0xFF,0xFF};
Window *Texture::gWindow;
TTF_Font *Texture::gFont=NULL;

///texture class functions
//constructors (!start required for global initialization)
Texture::Texture(){
    start(NULL,0,0);
}
Texture::Texture(SDL_Texture *t){
    set(t);
}
Texture::Texture(std::string path){
    if(!globalInit){
        std::cout << "WARNING: Texture not globally initialized... automatic initialization used." << std::endl;
        gStart(new Window(),NULL);
    }
    load(path);
}
//copy constructor
Texture::Texture(Texture &texture){
    *this=texture;
}
//destructor(s)
Texture::~Texture(){
    //!SDL_DestroyTexture(texture);
    std::cout << "destroying texture" << std::endl;
}
//start function
void Texture::start(SDL_Texture *t,int w,int h){
    //default global initiation
    if(!globalInit){
        std::cout << "WARNING: Texture not globally initialized... automatic initialization used." << std::endl;
        gStart(new Window(),NULL);
    }
    //set values
    texture=t;
    width=w;
    height=h;
}
//global initialization function
void Texture::gStart(Window *window,TTF_Font *font){
    if(!globalInit){
        //start global window and renderer
        gWindow=window;
        gFont=font;
        globalInit=true;
    }
}
//setters
void Texture::set(SDL_Texture *t){
    int w,h;
    SDL_QueryTexture(t,NULL,NULL,&w,&h);
    start(t,w,h);
}
void Texture::set(Texture *t){
    start(t->sdlT(),t->getWidth(),t->getHeight());
}
//load a texture from a file
bool Texture::load(std::string path){
    //make path
    path=gPath+path;
    //load BMP image as SDL surface
    SDL_Surface *s=SDL_LoadBMP(path.c_str());
    //check if loaded successfully
    if(s==NULL) std::cout<<"Could not load BMP file:"<<path<<" as game texture\n";
    //color key surface
    SDL_SetColorKey(s,SDL_TRUE,SDL_MapRGB(s->format,keyColor.r,keyColor.g,keyColor.b));
    //convert surface to texture and apply it as the class texture
    set(SDL_CreateTextureFromSurface(getRenderer(),s));
    //free surface
    SDL_FreeSurface(s);
    //return success
    return true;
}
//load from rendered text
bool Texture::loadFromText(std::string text){
    return loadFromText(text,0xFF,0xFF,0xFF);
}
bool Texture::loadFromText(std::string text,SDL_Color *color){
    return loadFromText(gFont,text,color);
}
bool Texture::loadFromText(TTF_Font *font,std::string text,SDL_Color *color){
    /*! TROLL BUG: similar to the troll bug in Interface.cpp
    int r=color->r;
    int g=color->g;
    int b=color->b;
    std::cout<<std::endl<<r<<" "<<g<<" "<<b<<std::endl;
    getch();
    */
    SDL_Surface *s=TTF_RenderText_Solid(gFont,text.c_str(),*color);
    if(s==NULL) std::cout<<"Could not load TTF rendered text texture:"<<text<<"\n";
    set(SDL_CreateTextureFromSurface(getRenderer(),s));
    SDL_FreeSurface(s);
    return true;
}
bool Texture::loadFromText(std::string text,int r,int g,int b){
    return loadFromText(gFont,text,r,g,b);
}
bool Texture::loadFromText(TTF_Font *font,std::string text,int r,int g,int b){
    SDL_Color color={r,g,b};
    return loadFromText(font,text,&color);
}
//apply a RGB color modulation to the texture
void Texture::setColor(Uint8 r,Uint8 g,Uint8 b){
    SDL_SetTextureColorMod(texture,r,g,b);
}
//update the screen
void Texture::updateRenderer(){
    SDL_RenderPresent(getRenderer());
}
//clear the screen
void Texture::clearRenderer(){
    SDL_RenderClear(getRenderer());
}
//zoom (set new zoom ratio)
void Texture::zoom(double zr){
    zoomRatio=zr;
}
//select only part of the texture and show it. Stretch it too.
void Texture::show(SDL_Rect *selectionRect,SDL_Rect *showRect){
    SDL_Rect zShowRect;
    zShowRect.x=showRect->x*zoomRatio;
    zShowRect.y=showRect->y*zoomRatio;
    zShowRect.w=showRect->w*zoomRatio;
    zShowRect.h=showRect->h*zoomRatio;;
    SDL_RenderCopy(getRenderer(),texture,selectionRect,&zShowRect);
}
//show the entire texture at given position
void Texture::show(int x,int y){
    show(NULL,new SDL_Rect{x,y,width,height});
}
