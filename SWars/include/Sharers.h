#ifndef SHARERS_H
#define SHARERS_H

#include "Users.h"
#include "Sharer.h"
#include "Network.h"

///material sharer class
class MaterialSharer:public Sharer,public MaterialUser{
    public:
        //constructor
        MaterialSharer(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam);
        //network dedicated functions
        void shareMaterials(int mat){sharedMaterials+=mat;}
    private:
        //network dedicated variables
        int sharedMaterials;
};

///energy sharer class
//!WIP
/*
class EnergySharer:public Sharer,public EnergyUser{
    private:
        ;
};
*/

#endif
