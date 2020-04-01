#ifndef RESOURCEEXTRACTOR_H
#define RESOURCEEXTRACTOR_H

#include "Thing.h"
#include "ResourceContainer.h"
#include "Users.h"
#include "Timer.h"

///resource extractor class
class ResourceExtractor:public virtual Thing{
    public:
        //friend classes
        friend class BGResourceContainer;
        //enumerations
        enum CAN_EXTRACT{
            ROCK = 0x1,
        };
        //constructors
        ResourceExtractor(double es,int extractFlags);
        //virtual functions fulfillment
        void act();
        void ccheck();
    protected:
        //available extractable rcs
        std::vector<ResourceContainer*> availableResourceContainers;
        //timer for keeping track of extraction speed
        Timer extractionTimer;
        //extraction speed (measured in extraction units per 1000 cycles)
        double extractionSpeed;
        //extractable resources
        bool extractableResources[Resource::MAX];
        //initialization function
        void start(int extracFlags);
        //extract from an extractable resource container every cycle
        bool extract();
        //decides what to do with extracted units
        virtual bool setExtractionUnits(double eu)=0;
};

///material resource extractor class
class MaterialResourceExtractor:public ResourceExtractor,public virtual MaterialUser{
    public:
        //constructor
        MaterialResourceExtractor(double es,int extractFlags);
        //virtual functions fulfillment
        bool setExtractionUnits(double eu);
};

///energy resource extractor class
class EnergyResourceExtractor:public ResourceExtractor,public virtual EnergyUser{
    public:
        //constructor
        EnergyResourceExtractor(double es,int extractFlags);
        //virtual functions fulfillment
        bool setExtractionUnits(double eu);
};

#endif
