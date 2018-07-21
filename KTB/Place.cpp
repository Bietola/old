#include "Place.h"

#include "Player.h"

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

///STARTING PLACE class
//constructor (make a random name)
StartingPlace::StartingPlace(){
    visited=false;
    name="Starting Place";
}
//returns a description of the place
const char *StartingPlace::getDescription(){
    if(!visited) return "A weird looking merchant is standing near a small island full of ships. You notice a creepy smile on his face...\0";
    else return "You don't know how or when, but the merchant's island is gone... in its place stands now a much bigger island with a flourishing town.\0";
}
//function called when someone enters the place
void StartingPlace::visit(Player *pl){
    clear();
    move(0,0);
    /*!if(!visited){
        int y=0;
        visited=true;
        StartingMerchant sm("As you enter the shop a merchant welcomes you: \"Welcome to me ship shop capt'n! I guess ye' need a ship hey?\"\0","me ships");
        sm.generateMerchandise();
        sm.inter(0,y,pl);
    }*/
    if(DB_MODE){
        if(pl->ship==NULL){
            mvprintw(0,0,"DB MODE SHIP added.");
            pl->ship=new UserShip("morningsun",pl->captain,50000,25000,1000,10,0);
            for(int j=0;j<3;j++) pl->ship->addBuilding(new BasicSail(1));
            for(int j=0;j<10;j++) pl->ship->addBuilding(new BasicCannon(1));
            for(int j=0;j<5;j++) pl->ship->addPerson(new Person(1));
        }
        else mvprintw(0,0,"DB MODE ship cannot be added...");
        refresh();
        getch();
        return;
    }
    if(!visited){
        int y=0;
        visited=true;
        shipMerchant.restock(pl);
        shipMerchant.sell(0,y,pl);
    }
    else{
        std::string townChoice;
        slaveMerchant.restock();
        cannonMerchant.restock();
        sailMerchant.restock();
        stuffMerchant.restock();
        do{
            int towny=0;
            clear();
            move(0,0);
            Menu townMenu;
            glstart_menu(&townMenu,0,towny,"the town","slavery","cannons shop","sails & co'","STUFFFORKILLING store","exit town","stop");
            townChoice=inter_menu(&townMenu);
            int y=0;
            clear();
            move(0,0);
            if(townChoice=="slavery"){
                slaveMerchant.sell(0,y,pl);
            }
            else if(townChoice=="cannons shop"){
                cannonMerchant.sell(0,y,pl);
            }
            else if(townChoice=="sails & co'"){
                sailMerchant.sell(0,y,pl);
            }
            else if(townChoice=="STUFFFORKILLING store"){
                stuffMerchant.sell(0,y,pl);
            }
        }while(townChoice!="exit town");
    }
}

///OPEN SEA class
//constructor (make a random name)
OpenSea::OpenSea(){
    if(perc(50)) name=names::getRand(names::ADJ,names::NAMES);
    else name=names::getRand(names::NAMES,names::NAMES);
    if(perc(25)) name+="'s";
    name+=(" "+names::getRand(names::OPENSEA));
    capitalize(name);
    danger=rrange(100);
    luck=rrange(25)+danger/4;
}
//returns a description of the place
const char *OpenSea::getDescription(){
    return "The only thing that you manage to see all around you is the immensity of the open sea... how could any man want more from this world?";
}
//function called when someone enters the place
void OpenSea::visit(Player *pl){
    ///DANGEROUS EVENTS
    if(perc(danger)){
        ///BATTLES
        if(perc(75)){
            Ship *ene;
            int r=rrange(100);
            if((r-=10)<0) ene=new ExplorerShip();
            else if((r-=20)<0) ene=new MercenaryShip();
            else if((r-=20)<0) ene=new PirateShip();
            else if((r-=20)<0) ene=new RoyalArmyShip();
            else if((r-=20)<0) ene=new PirateShip();
            else if((r-=5)<0) ene=new PirateMotherShip();
            else ene=new RoyalWarShip();
            Ship::battle(ene->intro,pl->ship,ene);
        }
        ///DANGEROUS EVENTS
        else{
            int r=rrange(100);
            int y=0;move(y,0);clear();
            ///SICKNESS
            if((r-=20)<0){
                y+=para("You're crew members start to cough as you enter a menacing cloud full of purple vapors. You figure that not all of them will live...\0")+2;
                getch();
                Person *rp=pl->ship->randPerson();
                mvprintw(y,0,"%s dies.",rp->getFullName().c_str());
                rp->die();
                getch();
            }
            ///EXPLOSION
            else if((r-=20)<0){
                y+=para("The sea turns bright red beneath your ship and the temperature starts to get warmer by the minute. Suddenly, flames start to spring out from the water and hit the ship's gunpowder supplies. You brace for impact...\0")+2;
                getch();
                ShipBuilding *rb=pl->ship->randBuilding();
                mvprintw(y,0,"BOOM! %s %s explodes!",rb->getTypeName(),rb->getSymId().c_str());
                rb->takeDamage({Shot::EXPLOSIVE,2000});
                getch();
            }
            ///INSECT INFESTATION (FOOD DEPLETION)
            else if((r-=20)<0){
                y+=para("In the distance, you see a cloud-like object approaching the ship at a steady pace. When you recognize the distinctive sound of hungry locusts, it's already too late to change course...");
                getch();
                y+=para("you lose half of your food supplies.");
                pl->ship->changeFood(-pl->ship->getFood()/2);
                getch();
            }
            ///PAIN SPIRITS
            else if((r-=10)<0){
                y+=para("Small floating, white orbs slowly surround your ship, one for each of your crew members, including you. Suddenly, gaining an enormous amount of speed, they sink into your crew member's skin, bruising it in multiple points. Your ears are pierced by screams of pain.");
                for(int j=0;j<pl->ship->getCrew().size();j++){
                    pl->ship->getCrew(j)->takeDamage({Shot::PUREPAIN,2000});
                }
                getch();
            }
            ///VAMPIRE
            else if((r-=10)<0){
                y+=para("It turns out one of your crew members was a vampire. He escaped now, but the consequences of his thirst did not fallow him.");
                for(int j=0;j<pl->ship->getCrew().size();j++){
                    pl->ship->getCrew(j)->takeDamage({Shot::PUREWOUNDS,2000});
                }
                getch();
            }
            ///GHOST MONSTER ATTACK
            else if((r-=10)<0){
                y+=para("Legends tell of a tremendous monster which comes out of the depths at night to feast on the souls of unlucky seamen. Sadly, a portion of your crew decided to spend the night on the ship's deck celebrating your latest victory. Turns out the legends were true...");
                for(int j=0;j<pl->ship->getCrew().size();j++){
                    if(perc(50)) pl->ship->getCrew(j)->selfcontrol-=20;
                }
                getch();
            }
            ///SEA TERMITES
            else if((r-=10)<0){
                y+=para("When you heard that the coming of men into the cursed seas caused the appearance of a rare kind of fish that could only eat ship-wood, you didn't believe it. Now thought, after seeing a portion of your ship's deck vanish before your eyes, you give it a second thought...");
                pl->ship->getDeck()->takeDamage({Shot::PUREBUILDINGDAMAGE,2000});
                getch();
            }
        }
    }
    ///SEMI-DANGEROUS EVENTS
    else if(perc(danger)){

    }
    ///NEUTRAL (USELESS) EVENTS
    else if(!perc(luck)){

    }
    ///GOOD (REWARDING) EVENTS
    else{

    }
}

///ISLAND class
//constructor (make a random name)
Island::Island(){
    if(perc(50)) name=names::getRand(names::ADJ,names::NAMES);
    else name=names::getRand(names::NAMES,names::NAMES);
    if(perc(25)) name+="'s";
    name+=(" "+names::getRand(names::ISLAND));
    capitalize(name);
}
//returns a description of the place
const char *Island::getDescription(){
    return "Far in the distance you manage to see a small patch of land emerging from the water, which you recognize immediately as an island.";
}
//function called when someone enters the place
void Island::visit(Player *pl){
    ;
}
