#include <cstdlib>
#include <ctime>
#include <iostream>

#include <SDL.h>

#include "grappa/Timer.h"

#include "globals.h"
#include "Thing.h"
#include "Collector.h"
#include "Food.h"

#define FPS 300

int main(int argc,char *args[])
{
    start_SDL();
    //request seed
    int seed;
    std::cout << "input positive integral seed (-1 for random): ";
    std::cin >> seed;
    if(seed<0) seed=time(NULL);
    srand(seed);
    std::cout << "seed: " << seed << std::endl;
    //request simulation type
    char c;
    bool useNNs=false;
    std::cout << "use neural nets? (y/n/b): ";
    std::cin >> c;
    if(c=='y') useNNs=true;
    //start sim
    std::cout << "press any key to begin the simulation" << std::endl;
    std::cin.get();
    //initial spawn
    if(c=='b'){
        for(int j=0;j<20;j++) Thing::create(new Collector(true));
        for(int j=0;j<20;j++) Thing::create(new Collector(false));
    }
    else for(int j=0;j<40;j++) Thing::create(new Collector(useNNs));
    for(int j=0;j<200;j++) Thing::create(new Food());
    //main loop
    SDL_Event e;
    while(!gWindow->checkQuit()){
        //
        double startTime=SDL_GetTicks();
        Texture::clearRenderer();
        //manage user input (only window events)
        while(SDL_PollEvent(&e)>0){
            gWindow->handleEvent(&e);
        }
        //manage things
        for(int j=0;j<Thing::maxSpawnedThing;j++){
            if(Thing::things[j]!=NULL){
                Thing::things[j]->act();
                if(Thing::things[j]!=NULL){
                    Thing::things[j]->show();
                }
            }
        }
        //spawn food
        if(rand()%50==0) Thing::create(new Food());
        //
        Texture::updateRenderer();
        while(SDL_GetTicks()-startTime<1000.0/(double)FPS);
    }
    /*
    srand(time(NULL));
    int i=4;
    int o=2;
    NeuralNetwork nn(i,3,3,o,1.0);
    while(true){
        std::vector<double> inputs;
        for(int j=0;j<i;j++){
            inputs.push_back((double)rand()/RAND_MAX*2-1.0);
            std::cout << "input: " << inputs.at(j) << std::endl;
        }
        std::vector<double> outputs=nn.update(inputs);
        for(int j=0;j<o;j++){
            std::cout << "output: " << outputs[j] << std::endl;
        }
        std::cout << std::endl;
        std::cin.get();
    }
    */
    return 0;
}
