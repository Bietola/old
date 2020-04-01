#include "Producer.h"

///producer functions
//constructor
Producer::Producer(int cs):chargeSpeed(cs){
    chargeTimer.start();
    canProduce=true;
}
//act (from Thing)
void Producer::act(){
    if(chargeTimer.getCycles()>=1000/chargeSpeed && canProduce){
        produce();
        chargeTimer.reset();
    }
}

///material producer functions
//produce PVFF
bool MaterialProducer::produce(){
    return addMaterials(1.0);
}
