#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

#include "menu.h"

#include "Ship.h"
#include "Person.h"

///forward declarations
class Place;
class PlaceMap;

///CAPTAIN base class
class Player{
    public:
        //constants
        static const char *USER;
        static const char *CPU;
        //constructors
        Player(std::string name);
        //pure virtual functions
        virtual void decide()=0;
        virtual std::string getContr()=0;
        //variables
        Captain *captain;
        PlaceMap *location;
        Ship *ship;
};

///USER CAPTAIN base class
class UserPlayer: public Player{
    public:
        //constructors
        UserPlayer(std::string name);
        //virtual function fulfillment
        void decide();
        std::string getContr(){std::string s(USER);return s;}
};

///CPU CAPTAIN base class
class CPUPlayer: public Player{
    public:
        //constructors
        CPUPlayer(std::string name);
        //virtual function fulfillment
        void decide();
        std::string getContr(){std::string s(CPU);return s;}
};

#endif
