#include "mathutils.h"

///math utility functions
//distance between the center of two rectangles
double calcDistance(SDL_Rect rect1,SDL_Rect rect2){
    std::pair<double,double> center1={(double)(rect1.x+rect1.w/2.0),(double)(rect1.y+rect1.h/2.0)};
    std::pair<double,double> center2={(double)(rect2.x+rect2.w/2.0),(double)(rect2.y+rect2.h/2.0)};
    return calcDistance(center1,center2);
}
//distance between two points
double calcDistance(std::pair<double,double> point1,std::pair<double,double> point2){
    return sqrt(pow(point1.first-point2.first,2)+pow(point1.second-point2.second,2));
}
//return random number in between range
int rrange(int min,int max){
    if(min>=max) return 0;
    return rand()%(max-min)+min;
}
int rrange(int max){
    return rand()%max;
}
double drrange(double min,double max){
    if(min>=max) return 0.0;
    double dr=(double)rand()/RAND_MAX;
    return min+dr*(max-min);
}
double drrange(double max){
    drrange(0.0,max);
}
int rsign(){
    return rand()%100<50 ? -1 : 1;
}

///other utilities
//evaluate error
bool containsError(int errCont,int err){
    return errCont & err;
}
