#include "Timer.h"

///timer functions
//constructor
Timer::Timer(){
    totTicks=0;
    creationTick=clock();
}
//getters and setters
int Timer::getTicks(){
    if(isStarted()) return clock()-startTick+totTicks;
    else return totTicks;
}
double Timer::getSeconds(){
    return (double)getTicks()/(double)1000;
}
//start the timer
void Timer::start(){
    startTick=clock();
}
void Timer::start(int i){
    startTick=clock()+i;
}
//pause the timer
void Timer::pause(){
   totTicks+=clock()-startTick;
   startTick=-1;
}
//restart timer after a pause ONLY (if the timer is reseted, don't do anything)
void Timer::restart(){
    if(!isReseted()) startTick=clock();
}
//reset the timer
void Timer::reset(){
    totTicks=0;
    startTick=clock();
}
void Timer::reset(int i){
    totTicks=0;
    startTick=clock()+i;
}
//check if the timer is started or paused
bool Timer::isStarted(){
    if(startTick>=0) return true;
    else return false;
}
//check if the timer is reseted
bool Timer::isReseted(){
    if(startTick>=0 || totTicks>0) return false;
    else return true;
}
