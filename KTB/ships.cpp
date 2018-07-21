#include "ships.h"

///EXPLORER SHIP class
//constructor
ExplorerShip::ExplorerShip():DummySFC(names::getRand(names::ADJ,names::FIRST),new Captain(names::getRand(names::FIRST)),2500,1000,13+2,15,0){
    intro="An explorer ship wants to kill you for no reason...\0";
    for(int j=0;j<5;j++){
        addPerson(new Person(1));
    }
    for(int j=0;j<3;j++){
        addBuilding(new BasicCannon(1));
    }
    addBuilding(new BasicSail(1));
}

///PIRATE SHIP class
//constructor
PirateShip::PirateShip():DummySFC(names::getRand(names::NAMES,names::NAMES),new Captain(names::getRand(names::ADJ,names::FIRST)),7500,2500,18+4,10,0){
    intro="A pirate ship wants to kill you for no reason...\0";
    for(int j=0;j<5;j++){
        addPerson(new Person(1));
    }
    for(int j=0;j<8;j++){
        addBuilding(new BasicCannon(1));
    }
    addBuilding(new BasicSail(1));
}

///ROYAL ARMY SHIP class
//constructor
RoyalArmyShip::RoyalArmyShip():DummySFC(names::getRand(names::ADJ)+"light",new Captain(names::getRand(names::FIRST)+"son"),10000,4000,34+4,5,0){
    intro="A royal army ship wants to kill you for no reason...\0";
    for(int j=0;j<5;j++){
        addPerson(new Person(1));
    }
    for(int j=0;j<3;j++){
        addBuilding(new BasicSail(1));
    }
    for(int j=0;j<4;j++){
        addBuilding(new BasicCannon(1));
    }
}

///MERCENARY SHIP class
//constructor
MercenaryShip::MercenaryShip():DummySFC(names::getRand(names::ADJ,names::NAMES),new Captain(names::getRand(names::NAMES,names::FIRST)),8000,3000,34+4,5,0){
    intro="a mercenary ship wants to kill you for no reason...\0";
    for(int j=0;j<5;j++){
        addPerson(new Person(1));
    }
    for(int j=0;j<2;j++){
        addBuilding(new BasicSail(1));
    }
    for(int j=0;j<6;j++){
        addBuilding(new BasicCannon(1));
    }
}

///ROYAL WARSHIP class
//constructor
RoyalWarShip::RoyalWarShip():DummySFC(names::getRand(names::ADJ)+"sun",new Captain(names::getRand(names::FIRST)+"son"),25000,10000,50+10,4,0){
    intro="A royal warship ship wants to kill you for no reason...\0";
    for(int j=0;j<5;j++){
        addPerson(new Person(1));
    }
    for(int j=0;j<4;j++){
        addBuilding(new BasicSail(1));
    }
    for(int j=0;j<10;j++){
        addBuilding(new BasicCannon(1));
    }
}

///PIRATE MOTHERSHIP class
//constructor
PirateMotherShip::PirateMotherShip():DummySFC(names::getRand(names::NAMES,names::NAMES),new Captain(names::getRand(names::ADJ,names::FIRST)),20000,9000,44+10,4,0){
    intro="A pirate mother ship wants to kill you for no reason...\0";
    for(int j=0;j<5;j++){
        addPerson(new Person(1));
    }
    for(int j=0;j<3;j++){
        addBuilding(new BasicSail(1));
    }
    for(int j=0;j<14;j++){
        addBuilding(new BasicCannon(1));
    }
}
