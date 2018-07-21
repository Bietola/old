#ifndef PRODUCER_H
#define PRODUCER_H

#include "Thing.h"
#include "Users.h"
#include "Timer.h"

///producer class
class Producer:public virtual Thing{
    public:
        //constructor
        Producer(int cs);
        //checkers
        bool isProducing(){return canProduce;}
        //pseudo-setters
        void stopProducing(){canProduce=false;}
        void startProducing(){canProduce=true;}
        //virtual functions fulfillment (Thing class)
        void act();
    protected:
        //produce pure virtual function. Called int act() every time build timer is reached.
        virtual bool produce()=0;
        //charge speed [1 material / 1000 cycles] and timer. produce function is called every time the time is reached.
        int chargeSpeed;
        Timer chargeTimer;
        //variable that indicates if producer is producing
        bool canProduce;
};

///material producer class
class MaterialProducer:public Producer,public virtual MaterialUser{
    public:
        //constructor
        MaterialProducer(int cs):Producer(cs){}
    private:
        //virtual function fulfillment (Producer)
        bool produce();
};

#endif
