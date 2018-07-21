#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <curses.h>

#include "comments.h"

#include "names.h"
#include "utils.h"
#include "Creature.h"

///BEARD class (needed for PERSON class)
class Beard{
    public:
        //enumerations
        enum TYPE{
            NONE,
            NORMAL,
            ABNORMAL,
            MAX,
        };
        //getters
        int getLength(){return length;}
        std::string getName(){return names[type];}
        //other functions
        void setRand(int level);
    private:
        static std::string names[MAX];
        int length;
        TYPE type;
};
///MOUSTACHE class (needed for PERSON class)
class Moustache{
    public:
        //enumerations
        enum TYPE{
            NONE,
            CHEVRON,
            DALI,
            ENGLISH,
            PENCIL,
            HANDLEBAR,
            PETITHANDLEBAR,
            BRUSH,
            PYRAMIDAL,
            HORSESHOE,
            TOOTHBRUSH,
            LAMPSHADE,
            FUMANCHU,
            IMPERIAL,
            WALRUS,
            MAX,
        };
        //getters
        int getLength(){return length;}
        std::string getName(){return names[type];}
        //functions
        void setRand(int level);
    private:
        static std::string names[MAX];
        int length;
        TYPE type;
};

///PERSON (crew member) base class
class Person:public Creature{
    public:
        //variables
        int money;
        ShipBuilding *mannedBuilding;
        //constructors
        Person(int level);
        Person(std::string name,int level);
        //getters
        bool isFree(){if(mannedBuilding==NULL) return true;return false;}
        //virtual functions
        virtual void describe();
        //virtual functions fulfillment
        const char *getMerchantString();
        void invPrint(int x,int y);
        Thing::THINGTYPE getThingType(){return Thing::PERSON;}
        Thing *clone(){return new Person(*this);}
        //functions
        void free();
    protected:
        //static variables
        static int nextId;
        //variables
        Moustache moustache;
        Beard beard;
        int id;
        //initialization functions
        void generate(int level);
        void generate(std::string name,int level);
};

///CAPTAIN base class
class Captain: public Person{
    public:
        Captain(std::string name);
        void say(const char *phrase);
};

#endif
