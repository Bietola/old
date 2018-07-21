#ifndef COLLECTOR_H
#define COLLECTOR_H

#include <vector>
#include <utility>

#include "NeuralNetwork/NeuralNetwork.h"
#include "Ship.h"
#include "globals.h"
#include "Food.h"

#define COLLECTOR_DB false

///collector class
class Collector: public Ship{
    public:
        //static constants
        static /*!const*/ double VISION_RANGE;
        static /*!const*/ double MAX_VEL;
        static /*!const*/ double MUTATION_RATE;
        static /*!const*/ double MUTATION_STRENGTH;
        //static variables
        static int NUM;
        //constructor
        Collector();
        Collector(bool useNN);
        //copy constructor
        Collector(Collector& collector);
        //VFF
        Thing* retClone(){return new Collector(*this);}
        void act();
        bool handleCollision(Thing* collider);
        //OVFF
        void despawn();
    private:
        //true if NN is used - false if velocity is handled randomly
        bool useNN;
        //filled up by collecting food. Organism dies when empty.
        int stomach;
        //organism reproduces when stomach reaches this level
        int reproductionThreshold;
        //inner timer parameters
        int startTime;
        int cycleTime;
        //neural net
        NeuralNetwork* neuralNetwork;
        //initialization function
        void start(bool useNn);
        //move around the board
        void move();
        //mutate
        void mutate();
        //reproduce asexually
        void reproduce();
};

#endif // COLLECTOR_H
