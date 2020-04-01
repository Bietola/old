#include "Networks.h"

///material network functions
//act - manage network
void BossNetwork<T,var,shared>::act(){
    T totToGive;
    for(int j=0;j<givers.size();j++){
        totToGive+=givers[j]->shared.getToGive();
    }
    boss->shared.store(totToGive);
}
