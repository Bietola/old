#include "globals.h"

using namespace std;

///global data
//all of the game textures
Texture *gTextures[TXTR_MAX];
//default load path
string gLoadPath="";

///global functions
//utilities (errors)
bool containsError(int errCont,int err){
    return errCont & err;
}
//utilities (random generation)
int rrange(int min,int max){
    if(min==max) return max;
    if(min>max) return min;
    return (rand()%(max-min+1))+min;
}
int rrange(int max){
    return rrange(0,max);
}
//utilities (string converters)
string to_string(int var){
    stringstream ss;
    ss << var;
    return ss.str();
}
string to_string(float var){
    stringstream ss;
    ss << var;
    return ss.str();
}
string to_string(double var){
    stringstream ss;
    ss << var;
    return ss.str();
}
//start graphics and game
void start_game(){
    //start SDL
    if(SDL_Init(SDL_INIT_EVERYTHING)<0) cout<<"Could not start SDL\n";
    //set hints (set texture filtering to linear)
    //!if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1")) cout<<"Could not set rendering to linear\n";
    //start SDL_ttf
    if(TTF_Init()==-1) cout<<"Could not start SDL_ttf\n";
    //start global window and renderer dedicated to textures
    Window *window=new Window("SWars",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,1000,700,SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    TTF_Font *font=TTF_OpenFont("fonts/vtks.ttf",100);
    Texture::gStart(window,font);
    //load global media
    Texture::setDefaultLoadPath("textures/");
    gTextures[TXTR_MCSHIP]=new Texture("MCShip.bmp");
    gTextures[TXTR_BOB]=new Texture("bob.bmp");
    gTextures[TXTR_PAUL]=new Texture("paul.bmp");
    gTextures[TXTR_BILLY]=new Texture("billy.bmp");
    gTextures[TXTR_HENRY]=new Texture("henry.bmp");
    gTextures[TXTR_BABY_DESTROYER]=new Texture("baby destroyer.bmp");
    gTextures[TXTR_MONTY]=new Texture("monty.bmp");
    gTextures[TXTR_BOBCANNON]=new Texture("bobcannon.bmp");
    gTextures[TXTR_FRANKCANNON]=new Texture("frankcannon.bmp");
    gTextures[TXTR_HENRYCANNON]=new Texture("henrycannon.bmp");
    gTextures[TXTR_ASTRACANNON]=new Texture("astracannon.bmp");
    gTextures[TXTR_VINCENT]=new Texture("vincent.bmp");
    gTextures[TXTR_SHITTYROCKY]=new Texture("shitty rocky.bmp");
    gTextures[TXTR_ROCKY]=new Texture("rocky.bmp");
    gTextures[TXTR_SPIKE_TRAP_1]=new Texture("spike trap 1.bmp");
    gTextures[TXTR_ROCK_TEMPLE_1]=new Texture("rock temple 1.bmp");
    gTextures[TXTR_UMBRELLA_1]=new Texture("umbrella 1.bmp");
    gTextures[TXTR_BASIC_PROJ]=new Texture("basic projectile.bmp");
    gTextures[TXTR_ASTRA_PROJ]=new Texture("astra projectile.bmp");
    gTextures[TXTR_END_SCREEN_BG_1]=new Texture("backgrounds/end screen bg 1.bmp");
    //!TODO load other textures
    //load resource textures
    Texture::setDefaultLoadPath("textures/resources/");
    Resource::bgTextures[Resource::ROCK][0]=new Texture("rock_1.bmp");
    Resource::bgTextures[Resource::ROCK][1]=new Texture("rock_2.bmp");
    Resource::bgTextures[Resource::ROCK][2]=new Texture("rock_3.bmp");
    Resource::bgTextures[Resource::ROCK][3]=new Texture("rock_4.bmp");
    Resource::bgTextures[Resource::ROCK][4]=new Texture("rock_5.bmp");
    Resource::bgTextures[Resource::ROCK][5]=new Texture("rock_6.bmp");
    Resource::bgTextures[Resource::ROCK][6]=new Texture("rock_7.bmp");
    Resource::bgTextures[Resource::MAX][0]=new Texture("empty.bmp");
    //reset default load path
    Texture::setDefaultLoadPath("textures/");
    //start random seed
    srand(time(NULL));
}
