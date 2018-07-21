#ifndef FOOD_H
#define FOOD_H

#include "Thing.h"
#include "globals.h"

///food class
class Food: public Thing{
    public:
        //constructor
        Food();
        //getters
        int getAmount(){return amount;}
        //VFF
        Thing* retClone(){return new Food(*this);}
        void act();
    private:
        //amount of food acquired by the collector
        int amount;
        //initialization functions
        void start(int amnt);
};

#endif // FOOD_H
