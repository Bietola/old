#ifndef NEURALNET_H
#define NEURALNET_H

#include <cassert>
#include <cmath>
#include <vector>

#include "../mathutils.h"
#include "NeuralLayer.h"

#define NN_DEBUG false

///neural network class
class NeuralNetwork{
    public:
        //constructor
        NeuralNetwork(int maxInputs,int maxHiddenLayers,int maxNeuronsPerLayer,int maxOutputs,double sigmoidResponse);
        //return output
        std::vector<double> update(std::vector<double> inputs);
        //mutation function
        void mutate(double mRate,double mStrength);
    private:
        //response constant of the sigmoid function used to adjust neuron output
        double sigmoidResponse;
        //neural layers (including input and output layer)
        std::vector<NeuralLayer> layers;
        //sigmoid function
        double sigmoid(double x){return 1.0/(1.0+exp(-x/sigmoidResponse));}
};

#endif // NEURALNET_H
