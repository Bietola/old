#include "Neuron.h"

///neuron functions
//constructor
Neuron::Neuron(int maxWeigths){
    for(int j=0;j<maxWeigths+1;j++){
        weigths.push_back(((double)rand()/RAND_MAX)*(rand()%100<50 ? -1 : 1));
    }
}
