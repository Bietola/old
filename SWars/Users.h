#ifndef USERS_H
#define USERS_H

#include "Thing.h"

///material user class
class MaterialUser:public virtual Thing{
    public:
        //constructor
        MaterialUser(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,double mat,double maxmat);
        //other functions
        double getMaterials(){return materials;}
        bool setMaterials(double mat){if(mat>maxMaterials){materials=maxMaterials;return false;} if(mat<0){return false;} materials=mat;return true;}
        bool addMaterials(double mat){return setMaterials(materials+mat);}
    protected:
        //variables
        double materials;
        double maxMaterials;
        //constructor dedicated to virtual inheritance
        MaterialUser();
        MaterialUser(double mat,double maxmat);
        //initialization function
        void start(double mat,double maxmat);
};

///energy user
class EnergyUser:public virtual Thing{
    public:
        //constructor
        EnergyUser(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,double ene,double maxene);
        //other functions
        double getEnergy(){return energy;}
        bool setEnergy(double ene){if(ene>maxEnergy){energy=maxEnergy;return false;} if(ene<0){return false;} energy=ene;return true;}
        bool addEnergy(double ene){return setEnergy(energy+ene);}
    protected:
        //variables
        double energy;
        double maxEnergy;
        //constructor dedicated to virtual inheritance
        EnergyUser();
        EnergyUser(double ene,double maxene);
        //initialization function
        void start(double ene,double maxene);
};

#endif
