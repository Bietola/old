#ifndef TIMER_H
#define TIMER_H

///timer class
class Timer{
    public:
        //constructors
        Timer();
        //getters and setters
        int getCycles();
        static int getCurrentCycle(){return currentCycle;}
        void addCycles(int cycles);
        static void increment(){currentCycle++;}
        //other functions
        void start();
        void start(int i);
        void pause();
        void restart();
        void reset();
        void reset(int i);
        bool isStarted();
    private:
        //static vars
        static int currentCycle;
        //ticks passed from the start of the program to the creation of the timer
        int creationCycle;
        //ticks passed from the last timer restart
        int startCycle;
        //ticks that have passed in all of the timer's starts (not counted when the timer is paused)
        int totCycles;
};

#endif
