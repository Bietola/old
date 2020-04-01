#include "MaterialProductMultiBuilder.h""

///material-product multi-builder functions
//virtual inheritance constructors
MaterialProductMultiBuilder::MaterialProductMultiBuilder(){}
//build - check if model is a product and pay the price in materials
void MaterialProductMultiBuilder::build(){
    if(dynamic_cast<MaterialProduct*>(model)){
        if(addMaterials(-dynamic_cast<MaterialProduct*>(model)->getCost())){
            MultiBuilder::build();
        }
    }
    else MultiBuilder::build();
}
