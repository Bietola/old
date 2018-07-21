#include "PlaceMap.h"

///PLACE MAP class
//static variables initialization
int PlaceMap::maxID=0;
//constructor - generate random map
PlaceMap::PlaceMap(int seed,int size){
    srand(seed);
    place=new StartingPlace();
    id=maxID;maxID++;
    for(int j=rrange(1,5);j>0;j--){
        paths.push_back(new PlaceMap(this,rrange(1,size-1)));
    }
}
//recursive constructors (used in generation only)
PlaceMap::PlaceMap(PlaceMap *back,int size){
    int r=rrange(100);
    if(r<50) place=new OpenSea();
    else place=new Island();
    id=maxID;maxID++;
    paths.push_back(back);
    for(int j=rrange(1,5);j>0;j--){
        if(size<1) break;
        paths.push_back(new PlaceMap(this,rrange(0,size-1)));
    }
}

///WORLD class
//constructor - generate random world
World::World(int seed,int size){
    head=new PlaceMap(seed,size);
}
/*!print world map
void World::dbPrint(){
    clear();
    move(0,0);
    print(head);
}
void World::dbPrint(PlaceMap *pm){
    pritnw("%d: ",pm->id);
    for(int j=0;j<pm->paths.size();j++){
        printw("%d%d "pm->paths[j]->id,(int)pm->paths[j]->getType());
    }
    place->print
}*/
