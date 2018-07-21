#ifndef NEURALLAYER_H
#define NEURALLAYER_H

#include <vector>

#include "Neuron.h"

///neural layer class
class NeuralLayer{
    public:
        //friend classes
        friend class NeuralNetwork;
        //constructor
        NeuralLayer(int maxWeigths,int maxNeurons);
    private:
        //neurons in neural layers
        std::vector<Neuron> neurons;
};

#endif // NEURALLAYER_H
