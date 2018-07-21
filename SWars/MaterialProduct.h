#ifndef MATERIALPRODUCT_H
#define MATERIALPRODUCT_H

#include "Thing.h"

///material product class (doesn't take from general "Product" class for simplicity)
class MaterialProduct:public virtual Thing{
    public:
        //constructor
        MaterialProduct(double matCost);
        //getters
        double getMaterialCost(){return materialCost;}
        double getCost(){return  materialCost;}
    private:
        //cost in materials of the product
        double materialCost;
};

#endif
