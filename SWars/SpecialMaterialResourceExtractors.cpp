#include "SpecialMaterialResourceExtractors.h"

///shitty rocky functions
//constructor
ShittyRocky::ShittyRocky():Thing(0,0,gTextures[TXTR_SHITTYROCKY],COLL_SHIP,CFACT_FRIEND,1,1),MaterialUser(0.0,100.0),MaterialResourceExtractor(50.0,ResourceExtractor::ROCK),MaterialSharer(),MaterialProduct(50.0){}
ShittyRocky::ShittyRocky(int x,int y):Thing(x,y,gTextures[TXTR_SHITTYROCKY],COLL_SHIP,CFACT_FRIEND,1,1),MaterialUser(0.0,100.0),MaterialResourceExtractor(50.0,ResourceExtractor::ROCK),MaterialSharer(),MaterialProduct(50.0){}
//think
void ShittyRocky::think(){}
void ShittyRocky::act(){
    MaterialResourceExtractor::act();
    share();
}

///rocky functions
//constructor
Rocky::Rocky():Thing(0,0,gTextures[TXTR_ROCKY],COLL_SHIP,CFACT_FRIEND,1,1),MaterialUser(0.0,100.0),MaterialResourceExtractor(120.0,ResourceExtractor::ROCK),MaterialSharer(),MaterialProduct(200.0){}
Rocky::Rocky(int x,int y):Thing(x,y,gTextures[TXTR_ROCKY],COLL_SHIP,CFACT_FRIEND,1,1),MaterialUser(0.0,100.0),MaterialResourceExtractor(120.0,ResourceExtractor::ROCK),MaterialSharer(),MaterialProduct(200.0){}
//think
void Rocky::think(){}
void Rocky::act(){
    MaterialResourceExtractor::act();
    share();
}
