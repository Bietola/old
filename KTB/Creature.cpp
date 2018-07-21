#include "Creature.h"

#include "ShipBuilding.h"

///CREATURE base class
//static variable definitions
Comm *Creature::curWarnlog=NULL;
Comm *Creature::curSpeaklog=NULL;
Comm *Creature::curPainlog=NULL;
//return the level of magnitude of the wounds inflicted on the creature
int Creature::getWoundsLevel(){
    if(wounds<100) return 0;
    if(wounds<500) return 1;
    if(wounds<1000) return 2;
    if(wounds<2000) return 3;
    if(wounds<5000) return 4;
}
//return the level of magnitude of the wounds inflicted on the creature
int Creature::getPainLevel(){
    if(pain<consciousness/4) return 0;
    if(pain<consciousness/3) return 1;
    if(pain<consciousness/2) return 2;
    if(pain<3*consciousness/4) return 3;
    if(pain<7*consciousness/8) return 4;
    if(pain>consciousness) return 5;
}
//return (in percentage) the efficiency of the creature at completing jobs
int Creature::getEfficency(){
    int painlv=getPainLevel();
    if(painlv==0) return 100;
    if(painlv==1) return 75;
    if(painlv==2) return 50;
    if(painlv==3) return 25;
    if(painlv==4) return 10;
    if(painlv==5) return 0;
}
//make the creature take damage from a cannon shot
void Creature::takeDamage(Shot shot){
    int damage=shot.getDamage();
    //check type
    if(shot.type==Shot::METAL){
        //inflict a lot of pain
        pain+=damage*2-painResistance;
        //inflict some wounds
        wounds+=damage-toughness;
        //reduce max consciousness
        if(perc(5)){
            consciousness-=std::max(damage/10-painResistance-selfcontrol,0);
        }
    }
    else if(shot.type==Shot::EXPLOSIVE){
        //inflict a moderate amount of pain
        pain+=damage-painResistance;
        //inflict some wounds
        wounds+=damage*0.8-toughness;
    }
    else if(shot.type==Shot::COMPOSITE){
        ;
    }
    else if(shot.type==Shot::MONSTER){
        ;
    }
    else if(shot.type==Shot::TOXIC){
        ;
    }
    else if(shot.type==Shot::ACID){
        ;
    }
    else if(shot.type==Shot::HAIRY){
        ;
    }
    else if(shot.type==Shot::PUREPAIN){
        //inflict pain only
        pain+=damage-painResistance;
    }
    else if(shot.type==Shot::PUREWOUNDS){
        //inflict wounds only
        wounds+=damage-toughness;
    }
}
//update the creatures stats
void Creature::update(){
    //bleed
    if(wounds>0) blood=std::max(blood-bloodLossRate,0);
    //regenerate wounds
    wounds=std::max(wounds-regeneration,0);
    //regenerate pain
    pain=std::max(pain-painRecovery,0);
    //lose consciousness permanently
    if(getPainLevel()==5) consciousness=std::min(consciousness-std::max(1,crazyness-selfcontrol),10);
    //announce pain
    if(pert(std::max(getPainLevel()-2,0)*2)) say(curPainlog,"it hurts!!");
}
//make the creature say something during a battle
void Creature::say(const char *phrase){
    say(curSpeaklog,phrase);
}
void Creature::say(std::string phrase){
    say(phrase.c_str());
}
void Creature::say(Comm* log,const char *phrase){
    if(log!=NULL) add_comm(log,"%s: %s",getInitials().c_str(),phrase);
}
void Creature::say(Comm* log,std::string phrase){
    say(log,phrase.c_str());
}
//STATIC - set person's global current logs
void Creature::setLogs(Comm *warnlog,Comm *speaklog,Comm *painlog){
    curWarnlog=warnlog;
    curSpeaklog=speaklog;
    curPainlog=painlog;
}
