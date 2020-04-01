 #ifndef SPECIALMATERIALRESOURCEEXTRACTORS_H
 #define SPECIALMATERIALRESOURCEEXTRACTORS_H

 #include "ResourceExtractor.h"
 #include "MaterialProduct.h"
 #include "Sharer.h"
 #include "globals.h"

  ///shitty rocky class
 class ShittyRocky:public MaterialResourceExtractor,public MaterialSharer,public MaterialProduct{
    public:
        //constructors
        ShittyRocky();
        ShittyRocky(int x,int y);
        //virtual functions fulfillment
        Thing *retClone(){return new ShittyRocky(*this);}
        void think();
        void act();
 };

 ///rocky class
 class Rocky:public MaterialResourceExtractor,public MaterialSharer,public MaterialProduct{
    public:
        //constructors
        Rocky();
        Rocky(int x,int y);
        //virtual functions fulfillment
        Thing *retClone(){return new Rocky(*this);}
        void think();
        void act();
 };

 #endif
