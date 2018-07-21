#include "Texture.h"

///constants
//board dimensions
const int BOARD_W=2400;
const int BOARD_H=2000;

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
    load(path);
}
//destructor(s)
Texture::~Texture(){
    //!SDL_DestroyTexture(texture);
    cout<<"destroying texture"<<endl;
}
//start function
void Texture::start(SDL_Texture *t,int w,int h){
    //default global initiation
    if(!globalInit){
        Window *window=new Window();
        gStart(window,NULL);
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
//update the screen
void Texture::update(){
    SDL_RenderPresent(getRenderer());
}
//zoom (set new zoom ratio)
void Texture::zoom(double zr){
    zoomRatio=zr;
}
//show a texture at 0 0
void Texture::show(){
    show(0,0);
}
//show a texture at a given position
void Texture::show(int x,int y){
    show(x,y,width,height);
}
//stretch a texture before showing it
void Texture::show(int x,int y,int w,int h){
    SDL_Rect r={x*zoomRatio,y*zoomRatio,w*zoomRatio,h*zoomRatio};
    SDL_RenderCopy(getRenderer(),texture,NULL,&r);
}
//show relative to a field of view as big as the screen
void Texture::showRelativeToField(int x,int y,int fx,int fy){
    showRelativeToField(x,y,fx,fy,SCREEN_W,SCREEN_H);
}
//show relative to a field of view
void Texture::showRelativeToField(int x,int y,int fx,int fy,int fw,int fh){
    x*=zoomRatio;
    y*=zoomRatio;
    fx*=zoomRatio;
    fy*=zoomRatio;
    fw*=zoomRatio;
    fh*=zoomRatio;
    if(x>=fx-fw && x<=fx+fw && y>=fy-fh && y<=fy+fh){
        SDL_Rect r={x-fx,y-fy,width*zoomRatio,height*zoomRatio};
        SDL_RenderCopy(getRenderer(),texture,NULL,&r);
    }
}
//show relative to a viewer's field of view
void Texture::showRelativeToViewer(int x,int y,int vx,int vy){
    if(vx<SCREEN_W/2) showRelativeToField(x,y,0,BOARD_H-SCREEN_H);
    else if(vx>BOARD_W-SCREEN_W/2) showRelativeToField(x,y,BOARD_W-SCREEN_W,BOARD_H-SCREEN_H);
    else showRelativeToField(x,y,vx-SCREEN_W/2,BOARD_H-SCREEN_H);
}
