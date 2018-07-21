#ifndef TIMER_H
#define TIMER_H

#include <ctime>

///timer class
class Timer{
    private:
        //ticks passed from the start of the program to the creation of the timer
        int creationTick;
        //ticks passed from the last timer restart
        int startTick;
        //ticks that have passed in all of the timer's starts (not counted when the timer is paused)
        int totTicks;
    public:
        //constructors
        Timer();
        //getters and setters
        int getTicks();
        double getSeconds();
        //other functions
        void start();
        void start(int i);
        void pause();
        void restart();
        void reset();
        void reset(int i);
        bool isStarted();
        bool isReseted();
};

#endif
