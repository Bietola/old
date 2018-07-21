#include "NeuralNetwork.h"

#include <iostream>

///neural network functions
//constructor
NeuralNetwork::NeuralNetwork(int maxInputs,int maxHiddenLayers,int maxNeuronsPerLayer,int maxOutputs,double sigmoidResponse){
    //set constants
    this->sigmoidResponse=sigmoidResponse;
    //initialize hidden layers
    for(int j=0;j<maxHiddenLayers;j++){
        int maxWeigths=(j==0 ? maxInputs : maxNeuronsPerLayer);
        layers.push_back(NeuralLayer(maxWeigths,maxNeuronsPerLayer));
    }
    //initialize output layer
    layers.push_back(NeuralLayer(maxNeuronsPerLayer,maxOutputs));
}
//update - return outputs of neural network
std::vector<double> NeuralNetwork::update(std::vector<double> inputs){
    std::vector<double> outputs;
    //process information through neural layers
    for(int j=0;j<layers.size();j++){
        outputs.clear();
        for(int k=0;k<layers[j].neurons.size();k++){
            double output=0.0;
            assert(inputs.size()==layers[j].neurons[k].weigths.size()-1);
            for(int i=0;i<inputs.size();i++){
                output+=layers[j].neurons[k].weigths[i]*inputs[i];
            }
            output-=layers[j].neurons[k].weigths[inputs.size()];
            //!START DB
            if(NN_DEBUG) std::cout << "iOutput (" << j << ";" << k  << "): " << output << "; " << sigmoid(output) << std::endl;
            //!END DB
            outputs.push_back(sigmoid(output));
        }
        inputs.clear();
        inputs=outputs;
    }
    return outputs;
}
//mutate - randomly modify weights around the net
void NeuralNetwork::mutate(double mRate,double mStrength){
    //check that mutations stats are standardized
    assert(mRate<=1.0 && mStrength<=1.0);
    //mutate net
    for(int j=0;j<layers.size();j++){
        for(int k=0;k<layers[j].neurons.size();k++){
            for(int i=0;i<layers[j].neurons[k].weigths.size();i++){
                if(rrange(1000)<mRate*1000) layers[j].neurons[k].weigths[i]*=(drrange(0.5,1.0)*(1.0-mStrength));
            }
        }
    }
}
