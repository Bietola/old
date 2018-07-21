#ifndef NEURON_H
#define NEURON_H

#include <cstdlib>
#include <vector>

///neuron class
class Neuron{
    public:
        //friend classes
        friend class NeuralLayer;
        friend class NeuralNetwork;
        //constructor
        Neuron(int maxWeigths);
        //getters
        int getMaxWeigths(){return weigths.size()-1;}
    private:
        //input weights
        std::vector<double> weigths;
};

#endif // NEURON_H
