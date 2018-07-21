#include "Users.h"

///material user functions
//constructor
MaterialUser::MaterialUser(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,double mat,double maxmat):Thing(x,y,t,ct,cf,hp,cdam){start(mat,maxmat);}
//constructor dedicated to virtual inheritance
MaterialUser::MaterialUser(){start(0,0);}
MaterialUser::MaterialUser(double mat,double maxmat){start(mat,maxmat);}
//initialization function
void MaterialUser::start(double mat,double maxmat){
    maxMaterials=maxmat;
    materials=(mat>maxmat ? maxmat : mat);
}

///energy user functions
//constructor
EnergyUser::EnergyUser(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,double ene,double maxene):Thing(x,y,t,ct,cf,hp,cdam){start(ene,maxene);}
//constructor dedicated to virtual inheritance
EnergyUser::EnergyUser(){start(0,0);}
EnergyUser::EnergyUser(double ene,double maxene){start(ene,maxene);}
//initialization function
void EnergyUser::start(double ene,double maxene){
    maxEnergy=maxene;
    energy=(ene>maxene ? maxene : ene);
}
