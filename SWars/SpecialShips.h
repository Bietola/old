#ifndef SPECIALSHIPS_H
#define SPECIALSHIPS_H

#include "Texture.h"
#include "Ship.h"
#include "Cannon.h"
#include "MaterialProduct.h"
#include "Resource.h"
#include "BGResourceContainer.h"
#include "Timer.h"

/// MODELS ///
///shooting ship (SShip) class
class SShip:public Ship,public Cannon{
    public:
        //constructors
        SShip(MOVE mt,SHOOT st,int cspeed,Ship *proj);
        SShip(MOVE mt,SHOOT st,int vx,int vy,int cspeed,Ship *proj);
        //destructor
        virtual ~SShip();
        //virtual functions fulfillment
        void move();
        void act();
        //pure virtual functions
        virtual Thing *retClone()=0;
        virtual void think()=0;
};

/// PROJECTILES ///
///basic projectile class
class BasicProjectile:public Ship{
    public:
        //constructors
        BasicProjectile();
        BasicProjectile(int vx,int vy);
        BasicProjectile(int vx,int vy,int cdam);
        BasicProjectile(COLLFACTION cf);
        BasicProjectile(COLLFACTION cf,int vx,int vy);
        BasicProjectile(COLLFACTION cf,int vx,int vy,int cdam);
        //virtual function fulfillment
        virtual Thing* retClone(){return new BasicProjectile(*this);}
};

///dum projectile class
class DumProjectile:public Ship{
    public:
        //constructors
        DumProjectile();
        DumProjectile(int xspeed,int yspeed,int cdam);
        DumProjectile(COLLFACTION cf);
        DumProjectile(COLLFACTION cf,int xspeed,int yspeed,int cdam);
        DumProjectile(int xspeed,int yspeed);
        //other functions
        virtual Thing *retClone(){return new DumProjectile(*this);}
        void think();
    protected:
        int xSpeed;
        int ySpeed;
};

///temporary projectile class
class TempProjectile:public DumProjectile{
    public:
        //constructors
        TempProjectile(COLLFACTION cf,int xspeed,int yspeed,int cdam,int dur);
        TempProjectile(Texture *t,COLLFACTION cf,int xspeed,int yspeed,int cdam,int dur);
        //copy constructor
        TempProjectile(TempProjectile &tempProjectile);
        //other functions
        virtual Thing *retClone(){return new TempProjectile(*this);}
        void ccheck();
    private:
        int duration;
        Timer lifeTimer;
};

/*///anti air projectile class
class AAProjectile:public Ship{
    public:
        //constructors
        AAProjectile();
        AAProjectile(int speed);
        AAProjectile(COLLFA)
};*/

/// ENEMIES ///
///bob ship class
class BobShip:public Ship{
    public:
        //constructors
        BobShip();
        BobShip(int x,int y);
        BobShip(int x,int y,int xspeed,int yspeed);
        //other functions
        virtual Ship *retClone(){return new BobShip(*this);}
        void think();
    private:
        int xSpeed;
        int ySpeed;
};

///slider ship class
class SliderShip:public Ship{
    public:
        //constructors
        SliderShip();
        SliderShip(int x,int y,Texture *t,int hp,int cdam,int speed);
        //other functions
        virtual Thing *retClone(){return new SliderShip(*this);}
        void think();
    protected:
        //various stats
        int speed;
        //additional info needed for movement
        int curDir;
};

/// SHOOTING ENEMIES ///
///paul ship class
class PaulShip:public SShip{
    public:
        //constants
        static const int MAX_PHASE_MOVE=200;
        static const int PHASE_STAND=25;
        static const int SPEED_SHIP=4;
        static const int SPEED_CHARGE=200;
        static const int SPEED_PROJ=20;
        static const int SHOOT_TIMES=4;
        //constructors
        PaulShip();
        PaulShip(int x,int y);
        PaulShip(int x,int y,int hp,int speed,int cspeed,int st);
        //virtual functions fulfillment
        virtual PaulShip *retClone(){return new PaulShip(*this);}
        void think();
    private:
        //initialization functions
        void start(int spd,int st);
        //additional infos needed for movement
        int phase;
        int speed;
        int shootTimes;
        int phaseMove;
};

///billy ship
class BillyShip:public SShip{
    public:
        //constants
        static const int CHARGE_SPEED=750;
        static const int PROJ_SPEED_X=7;
        static const int PROJ_SPEED_Y=5;
        static const int PROJ_DURATION=750;
        //constructors
        BillyShip();
        BillyShip(int x,int y);
        BillyShip(int x,int y,int hp,int cdam,int xVel,int yVelMult,int bombChargeTime,int bombNumber);
        BillyShip(int x,int y,int hp,int cdam,int xVel,int yVelMinMult,int yVelMaxMult,int bombChargeTime,int bombNumber);
        //destructor(s)
        ~BillyShip();
        //virtual function fulfillment
        virtual BillyShip *retClone(){return new BillyShip(*this);}
        void think();
    private:
        //initialization functions
        void start(int yVelMinMult,int yVelMaxMult,int bombChargeTime,int bombNumber);
        //movement
        Timer yVelTimer;
        float yVelCurMult;
        int yVelMinMult;
        int yVelMaxMult;
        int yPosStart;
        //bombing
        Timer bombTimer;
        int bombNumber;
        int bombChargeTime;
};

/// FRIENDS ///
///henry ship class
class HenryShip:public SShip{
    public:
        //constants
        static const int PHASE_MOVE=50;
        static const int PHASE_STAND=PHASE_MOVE+25;
        static const int SPEED_CHARGE=1000;
        static const int SPEED_PROJ=6;
        static const int LIMIT_MINALTITUDE=10;
        static const int LIMIT_MAXALTITUDE=10;
        static const int LIMIT_SIDES=10;
        //constructors
        HenryShip();
        HenryShip(int x,int y);
        //virtual functions fulfillment
        virtual Ship *retClone(){return new HenryShip(*this);}
        void think();
        void ccheck();
    private:
        //initialization functions
        void start();
        //additional info needed for movement
        int phase;
        int lifePhase;
};
///baby destroyer ship class
class BabyDestroyerShip:public SShip,public MaterialProduct{
    public:
        //constructors
        BabyDestroyerShip();
        //virtual functions fulfillment
        Thing *retClone(){BabyDestroyerShip tmp=*this;tmp.lifeTimer.start();return new BabyDestroyerShip(tmp);}
        void think();
        void ccheck();
        //timer for self destruct
        Timer lifeTimer;
    private:
        //initialization functions
        void start();
};

///NEUTRALS
///Monty ship class
class MontyShip:public SliderShip{
    public:
        //constructor
        MontyShip(int x,int y,int hp,int cdam,int speed,int res,int modTime);
        //virtual functions fulfillment
        Thing *retClone(){return new MontyShip(*this);}
        void think();
        void ccheck();
        //timer for self destruct
        Timer lifeTimer;
    private:
        //boolean to understand in which modality the Monty ship is
        bool mod;
        //amount of resources to generate once destroyed
        int res;
        //amount of time it takes for the ship to switch modes
        int modTime;
        //initialization function
        void start(int res,int modTime);
};

#endif
