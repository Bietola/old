#include "Collector.h"

///static constant initializations
double Collector::VISION_RANGE=200.0;
double Collector::MAX_VEL=2.0;
//must be in between 0 and 1
double Collector::MUTATION_RATE=0.2;
double Collector::MUTATION_STRENGTH=0.2;

///static member initializations
int Collector::NUM=0;

///collector functions
//constructor
Collector::Collector():Ship(NULL,rand()%(BOARD_W-getWidth()),rand()%(BOARD_H-getHeight()),CT_COLLECTOR){
    start(true);
    NUM++;
}
Collector::Collector(bool useNN):Ship(NULL,rand()%(BOARD_W-getWidth()),rand()%(BOARD_H-getHeight()),CT_COLLECTOR){
    start(useNN);
    NUM++;
}
//copy constructor
Collector::Collector(Collector& collector):Ship(texture,rand()%(BOARD_W-getWidth()),rand()%(BOARD_H-getHeight()),CT_COLLECTOR){
    *this=collector;
    neuralNetwork=new NeuralNetwork(*collector.neuralNetwork);
    NUM++;
}
//initialization function
void Collector::start(bool useNn){
    //check constants
    assert(MUTATION_RATE<=1.0);
    assert(MUTATION_STRENGTH<=1.0);
    //set variables
    useNN=useNn;
    if(useNN){
        stomach=200;
        reproductionThreshold=rrange(300,500);
    }
    else{
        stomach=1000;
        reproductionThreshold=1500;
    }
    startTime=SDL_GetTicks();
    cycleTime=rrange(100,120000);
    neuralNetwork=new NeuralNetwork(5,1,6,2,1.0);
    //set texture
    if(useNN) setTexture(gTextures[GTXTR_NNCOLLECTOR]);
    else{
        while(xVel==0) xVel=(double)rand()/RAND_MAX*MAX_VEL;
        while(yVel==0) yVel=(double)rand()/RAND_MAX*MAX_VEL;
        setTexture(gTextures[GTXTR_RANDCOLLECTOR]);
    }
}
//mutate
void Collector::mutate(){
    //mutate parameters
    if(rrange(1000)<MUTATION_RATE*1000) reproductionThreshold=std::max(reproductionThreshold+(MUTATION_STRENGTH*rrange(10,100)*rsign()),100.0);
    if(rrange(1000)<MUTATION_RATE*1000) cycleTime=std::max(cycleTime+(int)(MUTATION_STRENGTH*rrange(10,60000)*rsign()),100);
    //mutate neural network
    neuralNetwork->mutate(MUTATION_RATE,MUTATION_STRENGTH);
}
//make a mutated copy with less stomach
void Collector::reproduce(){
    //halve stomach
    stomach/=2;
    //create identical offspring to mutate
    Collector* offspring=new Collector(*this);
    //mutate offspring
    if(useNN) offspring->mutate();
    //spawn offspring
    offspring->spawn();
}
//act
void Collector::act(){
    //diminish stomach based on velocity
    double xxVel=xVel/MAX_VEL;
    double yyVel=yVel/MAX_VEL;
    double hungerChance=sqrt(xxVel*xxVel+yyVel*yyVel)*100;
    if(rand()%200<hungerChance) stomach--;
    //check if collector is dead
    if(stomach<=0){
        if(COLLECTOR_DB) std::cout << "D" << std::endl;
        despawn();
        return;
    }
    //manage reproduction
    if(stomach>=reproductionThreshold){
        if(COLLECTOR_DB) std::cout << "R" << std::endl;
        reproduce();
    }
    //use NN
    if(useNN){
        //adjust inner timer
        if(SDL_GetTicks()-startTime>=cycleTime) cycleTime=SDL_GetTicks();
        //find nearest food
        double minDistance=VISION_RANGE+1.0;
        int nearestFoodId=-1;
        for(int j=0;j<maxSpawnedThing;j++){
            if(things[j]!=NULL){
                if(things[j]->getCollisionType()==CT_FOOD){
                    double distance=calcDistance(cBox,things[j]->getCBox());
                    if(distance<minDistance){
                        minDistance=distance;
                        nearestFoodId=j;
                    }
                }
            }
        }
        //if nearest food is found, feed information about its position to the neural network. Otherwise use -1 -1.
        std::pair<double,double> distanceVector;
        if(nearestFoodId!=-1){
            if(minDistance==0) distanceVector={0.0,0.0};
            else{
                //find position
                int nfxPos,nfyPos;
                nfxPos=things[nearestFoodId]->getXPos();
                nfyPos=things[nearestFoodId]->getYPos();
                //calculate vector between food and collector
                distanceVector={(double)(cBox.x-nfxPos),(double)(cBox.y-nfyPos)};
                //normalize vector
                distanceVector.first/=minDistance;
                distanceVector.second/=minDistance;
            }
        }
        std::vector<double> inputs={xVel/MAX_VEL,yVel/MAX_VEL,distanceVector.first,distanceVector.second,(double)(SDL_GetTicks()-startTime)/(double)cycleTime};
        std::vector<double> outputs=neuralNetwork->update(inputs);
        //debug
        if(COLLECTOR_DB){
            std::cout << "CDB (" << spawnId << "): i: " << inputs[0] << ";" << inputs[1] << ";" << inputs[2] << ";" << inputs[3] << ";" << inputs[4] << std::endl;
            std::cout << "CDB (" << spawnId << "): o: " << outputs[0] << ";" << outputs[1] << std::endl;
        }
        //set velocity based on the nn outputs
        xVel=(outputs[0]==0 ? 0 : ((MAX_VEL*2)*outputs[0])-MAX_VEL);
        yVel=(outputs[1]==0 ? 0 : ((MAX_VEL*2)*outputs[1])-MAX_VEL);
    }
    //modify velocity randomly
    else{
        if(!rrange(100)){
            xVel=(double)rand()/RAND_MAX*MAX_VEL;
            yVel=(double)rand()/RAND_MAX*MAX_VEL;
        }
    }
    //move collector
    Ship::act();
}
//handle collision - return if this has despawned in the process
bool Collector::handleCollision(Thing* collider){
    //collision with food
    if(collider->getCollisionType()==CT_FOOD){
        assert(dynamic_cast<Food*>(collider)!=NULL);
        stomach+=dynamic_cast<Food*>(collider)->getAmount();
        collider->despawn();
        return false;
    }
    else if(collider->getCollisionType()==CT_COLLECTOR){
        return false;
    }
    else{
        assert(false);
        return false;
    }
}
//despawn override
void Collector::despawn(){
    NUM--;
    Thing::despawn();
}
