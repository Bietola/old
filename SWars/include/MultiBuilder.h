#ifndef MULTIBUILDER_H
#define MULTIBUILDER_H

#include <vector>

#include "Builder.h"
#include "globals.h"

///MULTIBUILDER class
class MultiBuilder:public Builder{
    public:
        //constructors
        MultiBuilder();
        MultiBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,vector<Thing*> ms,double cs);
        MultiBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,double cs);
        //copy constructor
        /*MultiBuilder(MultiBuilder &multiBuilder);*/
        //destructor(s)
        virtual ~MultiBuilder();
        //other functions
        void setModel(unsigned int i);
        void nextModel();
        void prevModel();
        //virtual functions fulfillment
        Thing *retClone(){return new MultiBuilder(*this);}
        void think();
        void act();
    protected:
        //normal inheritance dedicated constructors
        MultiBuilder(double cs,vector<Thing*> ms);
        MultiBuilder(double cs);
        //virtual inheritance dedicated constructors
        MultiBuilder(vector<Thing*> ms);
        //vector of models
        vector<Thing*> models;
        //current model index
        int curModelI;
        //initialization functions
        void start();
        void start(vector<Thing*> ms);
};

#endif
