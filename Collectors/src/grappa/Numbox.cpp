#include "Numbox.h"

///number box class
//constructors
NumBox::NumBox(){}
NumBox::NumBox(int r,int g,int b,int maxDigits){
    SDL_Color color={r,g,b};
    start(Texture::gFont,&color,maxDigits,"NA",NULL);
}
NumBox::NumBox(TTF_Font *font,int r,int g,int b,int maxDigits){
    SDL_Color color={r,g,b};
    start(font,&color,maxDigits,"NA",NULL);
}
NumBox::NumBox(std::string fontPath,int size,int r,int g,int b,int maxDigits){
    SDL_Color color={r,g,b};
    TTF_Font *font;
    font=TTF_OpenFont(fontPath.c_str(),size);
    start(font,&color,maxDigits,"NA",NULL);
}
NumBox::NumBox(int r,int g,int b,int maxDigits,std::string title){
    SDL_Color color={r,g,b};
    start(Texture::gFont,&color,maxDigits,title.c_str(),NULL);
}
NumBox::NumBox(TTF_Font *font,int r,int g,int b,int maxDigits,std::string title){
    SDL_Color color={r,g,b};
    start(font,&color,maxDigits,title.c_str(),NULL);
}
NumBox::NumBox(std::string fontPath,int size,int r,int g,int b,int maxDigits,std::string title){
    SDL_Color color={r,g,b};
    TTF_Font *font;
    font=TTF_OpenFont(fontPath.c_str(),size);
    start(font,&color,maxDigits,title.c_str(),NULL);
}
NumBox::NumBox(std::string fontPath,int size,int r,int g,int b,int maxDigits,std::string title,Texture *backGround){
    SDL_Color color={r,g,b};
    TTF_Font *font;
    font=TTF_OpenFont(fontPath.c_str(),size);
    start(font,&color,maxDigits,title.c_str(),backGround);
}
//initialization function
void NumBox::start(TTF_Font *font,SDL_Color *color,int maxDigits,std::string title,Texture *bg){
    this->font=font;
    this->color=color;
    this->maxDigits=maxDigits;
    this->title=title;
    this->backGround=bg;
}
//show the num box on the screen
void NumBox::show(int x,int y,int n){
    char buffer[10];
    int length;
    std::stringstream ss;
    ss << n;
    strcpy(buffer,ss.str().c_str());
    length=strlen(buffer);
    if(length<=maxDigits){
        int j;
        char number[10];
        strcpy(number,buffer);
        for(j=0;j<length;j++){
            buffer[j]=' ';
        }
        buffer[j]='\0';
        strcat(buffer,number);
    }
    else{
        int j;
        for(j=0;j<maxDigits;j++){
            buffer[j]='9';
        }
        buffer[j]='\0';
    }
    /*!TROLL BUG: loadFromText(std::string,SDL_Color*) does not work but loadFromText(std::string,int,int,int) does
    int r=color->r;
    int g=color->g;
    int b=color->b;
    std::cout<<std::endl<<"from interface: "<<r<<" "<<g<<" "<<b<<std::endl;
    */
    numTexture->loadFromText(buffer,255,255,255);
    numTexture->show(x,y);
}
