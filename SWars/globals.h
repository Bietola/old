#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <iostream>
#include <sstream>
#include <SDL.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

#include <conio.h>

#include "Window.h"
#include "Texture.h"
#include "Resource.h"

///toggle constants
#define DEBUG false

///general constants
#define PI atan(1)*4
#define SINP4 sin(PI/4)
#define COSP4 cos(PI/4)
#define SINP8 sin(PI/8)
#define COSP8 cos(PI/8)

///error constants
//out of bounds error
#define OOBERR_NULL 0x0
#define OOBERR_PX 0x1
#define OOBERR_NX 0x2
#define OOBERR_PY 0x4
#define OOBERR_NY 0x8

using namespace std;

///custom types
//game textures
enum TXTR{
    TXTR_MCSHIP,
    TXTR_BOB,
    TXTR_PAUL,
    TXTR_BILLY,
    TXTR_HENRY,
    TXTR_BABY_DESTROYER,
    TXTR_MONTY,
    TXTR_BOBCANNON,
    TXTR_FRANKCANNON,
    TXTR_HENRYCANNON,
    TXTR_ASTRACANNON,
    TXTR_VINCENT,
    TXTR_SHITTYROCKY,
    TXTR_ROCKY,
    TXTR_SPIKE_TRAP_1,
    TXTR_ROCK_TEMPLE_1,
    TXTR_UMBRELLA_1,
    TXTR_BASIC_PROJ,
    TXTR_RED_BASIC_PROJ,
    TXTR_ASTRA_PROJ,
    TXTR_END_SCREEN_BG_1,
    TXTR_MAX,
};

///utility classes
//coos structure
struct Coos{
    int x,y;
    Coos():x(0),y(0){}
    Coos(int xx,int yy):x(xx),y(yy){}
};
/*
//identity structure (used for type deduction)
template<class T> struct Identity{
    typedef T type;
};
*/

//structure used to determine if a variable is a pointer to a class or not
/*template<class T> struct is_pointer{static const bool value=false;};
template<class T> struct is_pointer<T*>{static const bool value=true;};*/

///global data
extern Texture *gTextures[TXTR_MAX];

///global functions
//utilities (errors)
bool containsError(int errCont,int err);
//utilities (random generators)
int rrange(int min,int max);
int rrange(int max);
//utilities (string converters)
string to_string(int var);
string to_string(float var);
string to_string(double var);
//game utilities
void number(int n);
//game dedicated
void start_game();

/*
//identity dedicated
template<class T> Identity<T> *deduce_identity(T data);
*/

/*
///template functions definitions
//deduct the identity of a given variable or data structure
template<class T> Identity<T> *deduce_identity(T data){
    return new Identity<T>;
}
*/

#endif
