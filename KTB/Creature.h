#ifndef CREATURE_H
#define CREATURE_H

#include <string>
#include <sstream>

#include "comments.h"

#include "Thing.h"

///forward declarations
class ShipBuilding;
struct Shot;

///CREATURE base class
class Creature: public Merchandise,public Storable{
    public:
        //static variables
        static Comm *curWarnlog;
        static Comm *curSpeaklog;
        static Comm *curPainlog;
        //constructor
        Creature(){}
        //destructor
        virtual ~Creature(){}
        //checkers
        bool isConscious(){return consciousness>0;}
        bool isAlive(){return blood>0;}
        bool canDo(){return isConscious() && isAlive();}
        //getters
        std::string getFirstName(){return firstName;}
        std::string getLastName(){return lastName;}
        //pseudo-getters
        std::string getFullName(){return firstName+" "+lastName;}
        std::string getAbbrName(){std::stringstream ss;ss << firstName[0] << firstName[1] << ". " << lastName;return ss.str();}
        std::string getInitials(){std::stringstream ss;ss << firstName[0] << firstName[1] << "." << lastName[0];return ss.str();}
        int getWoundsLevel();
        int getPainLevel();
        int getEfficency();
        void die(){blood=0;}
        //static functions
        static void setLogs(Comm *warnlog,Comm *speaklog,Comm *painlog);
        //virtual functions
        virtual void say(const char *phrase);
        //other functions
        void takeDamage(Shot shot);
        void update();
        void say(std::string phrase);
        void say(Comm *log,const char *phrase);
        void say(Comm *log,std::string phrase);
    /*protected:*/
        //variables
        std::string firstName;
        std::string lastName;
        int maxBlood;
        int blood;
        int consciousness;
        int strength;
        int toughness;
        int pain;
        int painResistance;
        int wounds;
        int regeneration;
        int painRecovery;
        int bloodLossRate;
        int agility;
        int crazyness;
        int selfcontrol;
};

#endif
