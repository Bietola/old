#include <string>
#include <vector>

#include <curses.h>
#include <cstdlib>
#include <ctime>

#include "palette.h"
#include "menu.h"

#include "Player.h"
#include "Place.h"

using namespace std;

///global variables
vector<Player*> players;
vector<World*> seas;

///global menu-functions
namespace menu{
    void turnBlock(std::string name);
    void welcome_screen();
    void main_menu();
    void start_game();
    void options();
    void credits();
    void game_loop();
    void wip();
    //turn block
    void turnBlock(std::string name){
        fill_screen(COLOR_RED);
        mvprintw(TERMINAL_MAX_Y/2,1,"%s",name.c_str());
        mvprintw(TERMINAL_MAX_Y/2,name.size()+1,"'s turn");
        refresh();
        getch();
    }
    //welcome screen
    void welcome_screen(){
        draw_rectangle(0,0,TERMINAL_MAX_X/2,TERMINAL_MAX_Y,COLOR_BLACK,COLOR_YELLOW,'.');
        draw_rectangle(TERMINAL_MAX_X/2,0,TERMINAL_MAX_X,TERMINAL_MAX_Y,COLOR_WHITE,COLOR_BLUE,'~');
        set_pair(COLOR_WHITE,COLOR_GREEN);
        mvprintw(TERMINAL_MAX_Y/2-1,TERMINAL_MAX_X/2-10,"WELCOME TO KTB!");
        mvprintw(TERMINAL_MAX_Y/2,TERMINAL_MAX_X/2-15,"press any key to play...");
        refresh();
        getch();
        main_menu();
    }
    //main menu
    void main_menu(){
        clear();
        Menu m;
        glstart_menu(&m,1,1,"MAIN MENU","start game","options","credits","stop");
        string s(inter_menu(&m));
        if(s=="start game") start_game();
        else if(s=="options") options();
        else if(s=="credits") credits();
    }
    //start a new game
    void start_game(){
        clear();
        set_pair(COLOR_WHITE,COLOR_BLACK);
        //load users and cpus from config file
        move(0,0);
        printw("loading players...");
        ifstream f("config.txt");
        while(f.get()!=':');
        while(!f.eof()){
            Player *p;
            string name,contr;
            f >> name >> contr;
            if(contr=="USER"){
                p=new UserPlayer(name);
            }
            else if(contr=="CPU") p=new CPUPlayer(name);
            players.push_back(p);
        }
        f.close();
        //print players
        printw("\nplayers loaded:");
        for(int j=0;j<players.size();j++){
            printw("\n%s (%s)",players[j]->captain->getFirstName().c_str(),players[j]->getContr().c_str());
        }
        getch();
        //show users their appearances
        for(int j=0;j<players.size();j++){
            if(players[j]->getContr()==Player::USER){
                turnBlock(players[j]->captain->getFullName());
                players[j]->captain->describe();
                getch();
            }
        }
        //generate world
        clear();
        mvprintw(0,0,"generating world...");
        refresh();
        //generate a sea for each player
        for(int j=0;j<players.size();j++){
            seas.push_back(new World(time(NULL)+j,50));
            players[j]->location=seas[j]->getHead();
        }
        printw("\nworld generated!");
        getch();
        //start the game loop
        game_loop();
    }
    //game options
    void options(){
        wip();
    }
    //show credits
    void credits(){
        wip();
    }
    //main game loop
    void game_loop(){
        //cycle through players
        while(true){
            for(int j=0;j<players.size();j++){
                turnBlock(players[j]->captain->getFullName());
                players[j]->decide();
            }
        }
    }
    //show WIP symbol for a work in progress menu
    void wip(){
        fill_screen(COLOR_YELLOW);
        mvprintw(TERMINAL_MAX_Y/2,TERMINAL_MAX_X/2-4,"WIP...");
        refresh();
        getch();
        main_menu();
    }
}

///global functions
//start the game
void start(int seed){
    //initiate random seed
    srand(seed);
    //start curses
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr,TRUE);
    start_color();
    //bring up welcome screen
    menu::welcome_screen();
}
void start(){
    start(time(NULL));
}

int main()
{
    start();
    return 0;
}
