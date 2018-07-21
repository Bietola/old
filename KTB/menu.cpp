#include "menu.h"

///GLOBAL DATA
short menuEclr=0;
char menuSelKey='s';
char menuUpKey='w';
char menuDownKey='x';
char menuBackKey='-';
char menuRightKey='d';
char menuLeftKey='a';
short menuTitleFclr=COLOR_RED;
short menuTitleBclr=COLOR_BLACK;
short menuMemberFclr=COLOR_WHITE;
short menuMemberBclr=COLOR_BLACK;
short menuSelFclr=COLOR_BLACK;
short menuSelBclr=COLOR_WHITE;

///INITIALIZATION
//start new menu
void start_menu(Menu* m,int x,int y,const char* title,short titFclr,short titBclr,char selKey,char upKey,char downKey,char backKey,char selFclr,char selBclr,...){
    //temp vars
    const char* s;
    char c;
    int j;
    //init arg list
    va_list args;
    va_start(args,selBclr);
    //set constant properties (WIP)
    m->nodelayMode=0;
    m->curPos=0;
    //set standard properties
    m->x=x;
    m->y=y;
    m->title.name=(char*)malloc(sizeof(char)*(strlen(title)+1));
    strcpy(m->title.name,title);
    m->title.fclr=titFclr;
    m->title.bclr=titBclr;
    m->selKey=selKey;
    m->upKey=upKey;
    m->downKey=downKey;
    m->backKey=backKey;
    m->lastBreakKey='\0';
    m->selFclr=selFclr;
    m->selBclr=selBclr;
    m->maxMembers=0;
    m->maxBreakKeys=0;
    m->prev=NULL;
    m->next=NULL;
    //set members properties
    s=va_arg(args,const char*);
    for(j=0;strcmp(s,"stop")!=0 && strcmp(s,"break")!=0;j++){
        m->members=(MenuMember*)memexpand(m->members,m->maxMembers,sizeof(MenuMember),MENU_MEMBER_MEM);
        m->maxMembers++;
        m->members[j].name=(char*)malloc(sizeof(char)*(strlen(s)+1));
        strcpy(m->members[j].name,s);
        m->members[j].fclr=menuMemberFclr;
        m->members[j].bclr=menuMemberBclr;
        s=va_arg(args,char*);
    }
    if(strcmp(s,"break")==0){
        //set break keys
        c=(char)va_arg(args,int);
        for(j=0;c!='\0';j++){
            m->breakKeys=(char*)memexpand(m->breakKeys,m->maxBreakKeys,sizeof(char),MENU_BREAKKEY_MEM);
            m->maxBreakKeys++;
            m->breakKeys[j]=c;
            c=(char)va_arg(args,int);
        }
    }
    va_end(args);
}
//start a new menu using global data
void glstart_menu(Menu* m,int x,int y,const char* title,...){
    //temp vars
    char* s;
    char c;
    int j;
    //init arg list
    va_list args;
    va_start(args,title);
    //set constant properties (WIP)
    m->nodelayMode=0;
    m->curPos=0;
    //set standard properties
    m->x=x;
    m->y=y;
    m->title.name=(char*)malloc(sizeof(char)*(strlen(title)+1));
    strcpy(m->title.name,title);
    m->title.fclr=menuTitleFclr;
    m->title.bclr=menuTitleBclr;
    m->selKey=menuSelKey;
    m->upKey=menuUpKey;
    m->downKey=menuDownKey;
    m->backKey=menuBackKey;
    m->lastBreakKey='\0';
    m->selFclr=menuSelFclr;
    m->selBclr=menuSelBclr;
    m->maxMembers=0;
    m->maxBreakKeys=0;
    m->prev=NULL;
    m->next=NULL;
    //set members properties
    s=va_arg(args,char*);
    for(j=0;strcmp(s,"stop")!=0 && strcmp(s,"break")!=0;j++){
        m->members=(MenuMember*)memexpand(m->members,m->maxMembers,sizeof(MenuMember),MENU_MEMBER_MEM);
        m->maxMembers++;
        m->members[j].name=(char*)malloc(sizeof(char)*(strlen(s)+1));
        strcpy(m->members[j].name,s);
        m->members[j].fclr=menuMemberFclr;
        m->members[j].bclr=menuMemberBclr;
        s=va_arg(args,char*);
    }
    if(strcmp(s,"break")==0){
        //set break keys
        c=(char)va_arg(args,int);
        for(j=0;c!='\0';j++){
            m->breakKeys=(char*)memexpand(m->breakKeys,m->maxBreakKeys,sizeof(char),MENU_BREAKKEY_MEM);
            m->maxBreakKeys++;
            m->breakKeys[j]=c;
            c=(char)va_arg(args,int);
        }
    }
    va_end(args);
}
//add a new member to a menu
void add_menuMember(Menu* m,const char* name,short fclr,short bclr){
    m->members=(MenuMember*)memexpand(m->members,m->maxMembers,sizeof(MenuMember),MENU_MEMBER_MEM);
    m->members[m->maxMembers].name=(char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(m->members[m->maxMembers].name,name);
    m->members[m->maxMembers].fclr=fclr;
    m->members[m->maxMembers].bclr=bclr;
    m->maxMembers++;
}
//add a new menu member following global values
void gladd_menuMember(Menu* m,const char* name){
    add_menuMember(m,name,menuMemberFclr,menuMemberBclr);
}

///LINK
//links a menu to another one
void link_menu(Menu* first,Menu* second){
    first->next=second;
    second->prev=first;
}

///OUTPUT
//print a menu and let the user interact with it
void print_menu(Menu m){
    int j;
    //print title
    set_pair(m.title.fclr,m.title.bclr);
    mvprintw(m.y,m.x,"%s",m.title.name);
    //print members
    for(j=0;j<m.maxMembers;j++){
        set_pair(m.members[j].fclr,m.members[j].bclr);
        mvprintw(m.y+j+2,m.x,"%s",m.members[j].name);
    }
    refresh();
}
//erases a menu
void erase_menu(Menu m){
    int j;
    unsigned int k;
    //erase title
    for(k=0;k<strlen(m.title.name);k++){
        set_pair(menuEclr,menuEclr);
        mvprintw(m.y,m.x+k," ");
    }
    //erase members
    for(j=0;j<m.maxMembers;j++){
        for(k=0;k<strlen(m.members[j].name);k++){
            set_pair(menuEclr,menuEclr);
            mvprintw(m.y+j+2,m.x+k," ");
        }
    }
    refresh();
}

///INPUT
//make a menu interactive until some menu member is chosen and return choice as and integer
int idinter_menu(Menu* m){
    int j;
    char com;
    short mfclr=curFclr,mbclr=curBclr;
    int breakKeyPressed=0;
    print_menu(*m);
    m->lastBreakKey='\0';
    do{
        set_pair(m->selFclr,m->selBclr);
        mvprintw(m->y+m->curPos+2,m->x,"%s",m->members[m->curPos].name);
        refresh();
        com=getch();
        for(j=0;j<m->maxBreakKeys;j++){
            if(m->breakKeys[j]==com){
                breakKeyPressed=1;
                break;
            }
        }
        if(com==m->selKey || breakKeyPressed){
            m->lastBreakKey=com;
            set_pair(m->members[m->curPos].fclr,m->members[m->curPos].bclr);
            mvprintw(m->y+m->curPos+2,m->x,"%s",m->members[m->curPos].name);
            set_pair(mfclr,mbclr);
            return m->curPos;
        }
        else if((com==m->upKey && m->curPos>0) || (com==m->downKey && m->curPos<m->maxMembers-1)){
            set_pair(m->members[m->curPos].fclr,m->members[m->curPos].bclr);
            mvprintw(m->y+m->curPos+2,m->x,"%s",m->members[m->curPos].name);
            if(com==m->upKey) m->curPos--;
            else m->curPos++;
        }
        /*else if(com==m->backKey && m->prev!=NULL){
            erase_menu(*m);
            idinter_menu(m->prev);
            erase_menu(*(m->prev));
            print_menu(*m);
        }*/
    }while(!m->nodelayMode);
}
//make a menu interactive until some menu member is chosen and return choice
char* inter_menu(Menu* m){
    int id=idinter_menu(m);
    return m->members[id].name;
}
//make menu interactive and link it to another menu. If the back key is pressed, return to the linked menu. Return choice
/*!char* follow_menu(Menu* m,Menu* oldm){
    erase_menu(*oldm);
    link_menu(oldm,m);
    inter_menu(*m);
}*/

///UTITLITY
//move the menu
void move_menu(Menu *m,int x,int y){
    m->x=x;
    m->y=y;
}
//return the number of lines used by a menu
int lines_menu(Menu m){
    return m.maxMembers+2;
}
//return last break key pressed
char lastBreakKey_menu(Menu m){
    return m.lastBreakKey;
}
//set menu to nodelay mode (WIP)
void nodelay_menu(Menu *m,int mode){
    m->nodelayMode=mode;
}
