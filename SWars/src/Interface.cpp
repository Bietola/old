#include "Interface.h"

///number box class
//constructors
NumBox::NumBox():Texture(){}
NumBox::NumBox(int r,int g,int b,int maxDigits):Texture(){
    SDL_Color color={r,g,b};
    start(gFont,&color,maxDigits,"NA");
}
NumBox::NumBox(TTF_Font *font,int r,int g,int b,int maxDigits):Texture(){
    SDL_Color color={r,g,b};
    start(font,&color,maxDigits,"NA");
}
NumBox::NumBox(std::string fontPath,int size,int r,int g,int b,int maxDigits){
    SDL_Color color={r,g,b};
    TTF_Font *font;
    font=TTF_OpenFont(fontPath.c_str(),size);
    start(font,&color,maxDigits,"NA");
}
NumBox::NumBox(int r,int g,int b,int maxDigits,std::string title):Texture(){
    SDL_Color color={r,g,b};
    start(gFont,&color,maxDigits,title.c_str());
}
NumBox::NumBox(TTF_Font *font,int r,int g,int b,int maxDigits,std::string title):Texture(){
    SDL_Color color={r,g,b};
    start(font,&color,maxDigits,title.c_str());
}
NumBox::NumBox(std::string fontPath,int size,int r,int g,int b,int maxDigits,std::string title){
    SDL_Color color={r,g,b};
    TTF_Font *font;
    font=TTF_OpenFont(fontPath.c_str(),size);
    start(font,&color,maxDigits,title.c_str());
}
//initialization function
void NumBox::start(TTF_Font *font,SDL_Color *color,int maxDigits,std::string title){
    this->font=font;
    this->color=color;
    this->maxDigits=maxDigits;
    this->title=title;
}
//show the num box on the screen
void NumBox::show(int x,int y,int n){
    std::stringstream numberString;
    numberString << n;
    /*!TROLL BUG: loadFromText(std::string,SDL_Color*) does not work but loadFromText(std::string,int,int,int) does
    int r=color->r;
    int g=color->g;
    int b=color->b;
    std::cout<<std::endl<<"from interface: "<<r<<" "<<g<<" "<<b<<std::endl;
    */ loadFromText(numberString.str(),255,255,255);
    Texture::show(x,y);
}

//texture box functions
//constructors
TextureBox::TextureBox(std::vector<Texture*> txtrs):curTextureI(0),textures(txtrs){}
TextureBox::TextureBox(std::vector<Thing*> thngs):curTextureI(0){
    for(int j=0;j<thngs.size();j++){
        textures.push_back(new Texture(thngs[j]->sdlT()));
    }
}
//destructor
TextureBox::~TextureBox(){
    for(int j=0;j<textures.size();j++){
        delete textures[j];
        textures.clear();
    }
}
//advance the index once
void TextureBox::next(){
    curTextureI++;
    if(curTextureI==textures.size()) curTextureI=0;
}
//go backward once
void TextureBox::prev(){
    curTextureI--;
    if(curTextureI==-1) curTextureI=textures.size()-1;
}
//add a texture
void TextureBox::add(Texture *txtr){
    textures.push_back(txtr);
}
void TextureBox::add(Thing *thng){
    add(new Texture(thng->sdlT()));
}
