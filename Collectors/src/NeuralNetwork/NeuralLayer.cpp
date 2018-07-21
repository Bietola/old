#include "NeuralLayer.h"

///neural layer functions
NeuralLayer::NeuralLayer(int maxWeigths,int maxNeurons){
    for(int j=0;j<maxNeurons;j++){
        neurons.push_back(Neuron(maxWeigths));
    }
}
