#ifndef MENU_H
#define MENU_H

#include <stdarg.h>
#include <curses.h>
#include <string.h>
#include "palette.h"
#include "memutils.h"

#define MENU_MEMBER_MEM 10
#define MENU_BREAKKEY_MEM 10

#define MENU_ERR -1

///GLOBAL DATA
extern short menuEclr;
extern char menuSelKey;
extern char menuUpKey;
extern char menuDownKey;
extern char menuBackKey;
extern char menuRightKey;
extern char menuLeftKey;
extern short menuTitleFclr;
extern short menuTitleBclr;
extern short menuMemberFclr;
extern short menuMemberBclr;
extern short menuSelFclr;
extern short menuSelBclr;

///STRUCTURES
typedef struct{
    char* name;
    short fclr;
    short bclr;
}MenuMember;
typedef struct Menu_s{
    MenuMember title;
    int x;
    int y;
    int nodelayMode;
    int curPos;
    char selKey;
    char upKey;
    char downKey;
    char backKey;
    char lastBreakKey;
    short selFclr;
    short selBclr;
    int maxMembers;
    MenuMember* members;
    int maxBreakKeys;
    char* breakKeys;
    struct Menu_s* prev;
    struct Menu_s* next;
}Menu;

///INITIALIZATION
//start new menu
void start_menu(Menu* m,int x,int y,const char* title,short titFclr,short titBclr,char selKey,char upKey,char downKey,char backKey,char selFclr,char selBclr,...);
//start a new menu using global data
void glstart_menu(Menu* m,int x,int y,const char* title,...);
//add a new member to a menu
void add_menuMember(Menu* m,const char* name,short fclr,short bclr);
//add a new menu member following global values
void gladd_menuMember(Menu* m,const char* name);

///LINK
//links a menu to another one
void link_menu(Menu* first,Menu* second);

///OUTPUT
//print a menu and let the user interact with it
void print_menu(Menu m);
//erases a menu
void erase_menu(Menu m);

///INPUT
//make a menu interactive until some menu member is chosen and return choice as and integer
int idinter_menu(Menu* m);
//make a menu interactive until some menu member is chosen and return choice
char* inter_menu(Menu* m);
//make menu interactive and link it to another menu. If the back key is pressed, return to the linked menu. Return choice
/*!char* follow_menu(Menu* m,Menu* oldm);*/

///UTITLITY
//move the menu
void move_menu(Menu *m,int x,int y);
//return the number of lines used by a menu
int lines_menu(Menu m);
//return last break key pressed
char lastBreakKey_menu(Menu m);
//set menu to nodelay mode (WIP)
void nodelay_menu(Menu *m,int mode);

#endif
