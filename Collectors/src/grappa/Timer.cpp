#include "Timer.h"

///timer functions
//constructor
Timer::Timer(){
    totTicks=0;
    creationTick=SDL_GetTicks();
}
//getters and setters
int Timer::getTicks(){
    if(isStarted()) return SDL_GetTicks()-startTick+totTicks;
    else return totTicks;
}
double Timer::getSeconds(){
    return (double)getTicks()/(double)1000;
}
//start the timer
void Timer::start(){
    startTick=SDL_GetTicks();
}
void Timer::setTicksStart(int t){
    startTick=SDL_GetTicks()-t;
}
void Timer::setSecondsStart(double s){
    startTick=SDL_GetTicks()-s*1000;
}
//pause the timer
void Timer::pause(){
   totTicks+=SDL_GetTicks()-startTick;
   startTick=-1;
}
//restart timer after a pause
void Timer::restart(){
    startTick=SDL_GetTicks();
}
//reset the timer
void Timer::reset(){
    totTicks=0;
    startTick=SDL_GetTicks();
}
void Timer::setTicksReset(int i){
    totTicks=0;
    startTick=SDL_GetTicks()-i;
}
void Timer::setSecondsReset(double s){
    totTicks=0;
    startTick=SDL_GetTicks()-s*1000;
}
//check if the timer is started or paused
bool Timer::isStarted(){
    if(startTick>=0) return true;
    else return false;
}
