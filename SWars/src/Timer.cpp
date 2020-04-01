#include "Timer.h"

///static variables initialization
int Timer::currentCycle=0;

///timer functions
//constructor
Timer::Timer(){
    totCycles=0;
    creationCycle=currentCycle;
}
// return cycles
int Timer::getCycles(){
    if(isStarted()) return currentCycle-startCycle+totCycles;
    else return totCycles;
}
//add cycles manually
void Timer::addCycles(int cycles){
    totCycles+=cycles;
}
//start the timer
void Timer::start(){
    startCycle=currentCycle;
}
void Timer::start(int i){
    startCycle=currentCycle+i;
}
//pause the timer
void Timer::pause(){
   totCycles+=currentCycle-startCycle;
   startCycle=-1;
}
//restart timer after a pause
void Timer::restart(){
    startCycle=currentCycle;
}
//reset the timer
void Timer::reset(){
    totCycles=0;
    startCycle=currentCycle;
}
void Timer::reset(int i){
    totCycles=0;
    startCycle=currentCycle+i;
}
//check if the timer is started or paused
bool Timer::isStarted(){
    if(startCycle>=0) return true;
    else return false;
}
