#include "Resource.h"

///resource functions
//static variables
Texture *Resource::bgTextures[Resource::MAX][MAX_STATES];
//constructor
Resource::Resource():type(Resource::ROCK),amount(0.0){}
Resource::Resource(Resource::TYPE type,double amount):type(type),amount(amount){}
//extract given amount of resources - return extracted resources.
double Resource::extractFrom(double amount){
    double retVal;
    if(addAmount(-amount)) retVal=amount;
    else{
        retVal=this->amount;
        this->amount=0.0;
    }
    return retVal;
}
