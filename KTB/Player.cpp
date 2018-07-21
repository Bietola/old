#include "Player.h"

#include "Place.h"

///PLAYER base class
//static variables definitions
const char *Player::USER="USER";
const char *Player::CPU="CPU";
//constructor
Player::Player(std::string name){
    captain=new Captain(name);
    ship=NULL;
}

///USER PLAYER class
//constructor
UserPlayer::UserPlayer(std::string name):Player(name){}
//make a decision on your turn
void UserPlayer::decide(){
    std::string choice;
    do{
        int y=0;
        clear();
        //print current location
        mvaddstr(y,0,captain->getFullName().c_str());
        mvprintw(y+=2,0,"location: %s (%s)\n",location->place->getName().c_str(),location->place->getTypeName());
        y+=para(location->place->getDescription())+2;
        //decide where to go
        Menu m;
        glstart_menu(&m,0,y,"what now?","check ship","stay here","stop");
        for(int j=0;j<location->paths.size();j++){
            gladd_menuMember(&m,location->paths[j]->place->getName().c_str());
        }
        choice=inter_menu(&m);
        if(choice=="check ship"){
            if(ship==NULL){
                mvprintw(0,0,"you have no ship...");
                refresh();
                getch();
            }
            else ship->printInventory();
        }
        else if(choice!="stay here"){
            for(int j=0;j<location->paths.size();j++){
                if(location->paths[j]->place->getName()==choice){
                    location=location->paths[j];
                    break;
                }
            }
        }
    }while(choice=="check ship");
    //visit the current place
    location->place->visit(this);
}

///CPU PLAYER class
//constructor
CPUPlayer::CPUPlayer(std::string name):Player(name){}
//make a decision on your turn
void CPUPlayer::decide(){
    db::stop("cpu");
}
