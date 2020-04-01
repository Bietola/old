#ifndef MATERIALPRODUCTMULTIBUILDER_H
#define MATERIALPRODUCTMULTIBUILDER_H

#include "MultiBuilder.h"
#include "Users.h"
#include "MaterialProduct.h"

///material-product multi-builder class
//!WIP does not inherit from a more general "ProductMultiBuilder" for lazyness
class MaterialProductMultiBuilder:public virtual MultiBuilder,public virtual MaterialUser{
    public:
    protected:
        //virtual inheritance constructors
        MaterialProductMultiBuilder();
        //virtual functions replacement
        void build();
};

#endif
