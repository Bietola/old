#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <utility>
#include <cmath>
#include <cstdlib>
#include <ctime>

#define SDLUTILS
#ifdef SDLUTILS
#include <SDL.h>
#endif

///math utility functions
//return distance between the center of two rectangles
#ifdef SDLUTILS
double calcDistance(SDL_Rect rect1,SDL_Rect rect2);
#endif
double calcDistance(std::pair<double,double> point1,std::pair<double,double> point2);
//return random number
int rrange(int min,int max);
int rrange(int max);
double drrange(double min,double max);
double drrange(double max);
int rsign();

///other utilities
//evaluate if error is in int
inline bool containsError(int errCont,int err);

#endif // MATHUTILS_H
