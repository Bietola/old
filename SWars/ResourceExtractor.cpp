#include "ResourceExtractor.h"

///resource extractor functions
//constructors
ResourceExtractor::ResourceExtractor(double es,int extractFlags):extractionSpeed(es),extractionTimer(){start(extractFlags);}
//initialization functions
void ResourceExtractor::start(int extractFlags){
    extractionTimer.start();
    for(int j=0;j<Resource::MAX;j++){
        extractableResources[j]=extractFlags & (int)pow(2,j);
    }
}
//extract from an extractable resource container - return true if extraction speed requirements have been reached; false if otherwise
bool ResourceExtractor::extract(){
    double extractedUnits=0;
    extractionTimer.reset();
    //extract possible extractable resources depending on extractor's capabilities - return false if nothing is extracted
    for(std::vector<ResourceContainer*>::iterator q=availableResourceContainers.begin();q!=availableResourceContainers.end();q++){
        for(int j=0;j<Resource::MAX;j++){
            if((*q)->resources[j].getAmount()>0 && extractableResources[j]){
                extractedUnits+=(*q)->resources[j].extractFrom(1.0);
            }
        }
    }
    //convert extracted units into whatever the extractor's been programmed to produce
    if(extractedUnits!=0) setExtractionUnits(extractedUnits);
    //return failure
    if(extractedUnits<1.0) return false;
    //return success
    return true;
}
//act function
void ResourceExtractor::act(){
    //extract at extraction speed
    if(extractionTimer.getCycles()>=1000/extractionSpeed){
        extract();
    }
}
//do some clean up specific to extractors
void ResourceExtractor::ccheck(){
    //clean up available resource containers
    availableResourceContainers.clear();
}

///material resource extractor functions
//constructor
MaterialResourceExtractor::MaterialResourceExtractor(double es,int extractFlags):ResourceExtractor(es,extractFlags){}
//set extraction units
bool MaterialResourceExtractor::setExtractionUnits(double eu){
    return setMaterials(eu);
}

///energy resource extractor functions
//constructor
EnergyResourceExtractor::EnergyResourceExtractor(double es,int extractFlags):ResourceExtractor(es,extractFlags){}
//set extraction units
bool EnergyResourceExtractor::setExtractionUnits(double eu){
    return setEnergy(eu);
}
