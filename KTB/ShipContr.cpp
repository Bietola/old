#include "ShipContr.h"

#include "Ship.h"

///USER Ship Fight Control class
//static members initialization
Comm *UserSFC::curComlog=NULL;
Comm *UserSFC::curBatlog=NULL;
Comm *UserSFC::curEnelog=NULL;
//initialization
void UserSFC::init(Ship *ene){
    //initiate timer (DB)
    startTime=clock();
    //STart checkers
    start=true;
    stCannons=false;
    stSails=false;
    eneStCrew=false;
    eneStCannons=false;
    eneStSails=false;
    eneStCrew=false;
    //man to man holder
    manToMan=NULL;
    //clear
    clear();
    //print battle title
    set_pair(COLOR_WHITE,COLOR_BLACK);
    mvprintw(0,0,"%s VS %s",name.c_str(),ene->getName().c_str());
    set_old_pair();
    //health bars
    start_bar(&hpbar,BAR_VERTICAL,BAT_HPBAR_X,BAT_HPBAR_Y,BAT_HPBAR_LENGTH,getDeck()->getMaxHp(),getDeck()->getHp(),{' ',COLOR_RED,COLOR_RED},{' ',COLOR_GREEN,COLOR_GREEN});
    start_bar(&enehpbar,BAR_VERTICAL,BAT_ENEHPBAR_X,BAT_ENEHPBAR_Y,BAT_ENEHPBAR_LENGTH,ene->getDeck()->getMaxHp(),ene->getDeck()->getHp(),{' ',COLOR_RED,COLOR_RED},{' ',COLOR_GREEN,COLOR_GREEN});
    //crew comments
    batlog=start_comm(batlog,"batlog.txt",BAT_LOG_X,BAT_LOG_Y,BAT_LOG_LINES,BAT_LOG_LENGTH);
    comlog=start_comm(comlog,"comlog.txt",BAT_COMLOG_X,BAT_COMLOG_Y,BAT_COMLOG_LINES,BAT_COMLOG_LENGTH);
    enelog=start_comm(enelog,"enelog.txt",BAT_ENELOG_X,BAT_ENELOG_Y,BAT_ENELOG_LINES,BAT_ENELOG_LENGTH);
    //select random person to say "KTB!"
    getCrew(rrange(getCrew().size()))->say(batlog,"kill those bastards!");
    //cannons menu
    if(!cannons.empty()){
        stCannons=true;
        start_menu(&nm,0,0,"cannons:",COLOR_WHITE,COLOR_BLACK,menuSelKey,menuUpKey,menuDownKey,menuBackKey,COLOR_WHITE,COLOR_YELLOW,"break",menuRightKey,' ','c','e','t','f','F','C','\0');
        for(int j=0;j<cannons.size();j++){
            add_menuMember(&nm,"*",COLOR_WHITE,COLOR_WHITE);
        }
    }
    //sails menu
    if(!sails.empty()){
        stSails=true;
        start_menu(&sm,0,0,"sails:",COLOR_WHITE,COLOR_BLACK,menuSelKey,menuUpKey,menuDownKey,menuBackKey,COLOR_WHITE,COLOR_YELLOW,"break",menuRightKey,' ','c','e','t','f','F','C','\0');
        for(int j=0;j<sails.size();j++){
            add_menuMember(&sm,"*",COLOR_WHITE,COLOR_WHITE);
        }
    }
    //crew menu
    if(!getCrew().empty()){
        stCrew=true;
        start_menu(&cm,0,0,"crew:",COLOR_WHITE,COLOR_BLACK,menuSelKey,menuUpKey,menuDownKey,menuBackKey,COLOR_WHITE,COLOR_YELLOW,"break",menuLeftKey,' ','e','f','F','C','\0');
        for(int j=0;j<getCrew().size();j++){
            add_menuMember(&cm,"*",COLOR_WHITE,COLOR_WHITE);
        }
        print_menu(cm);
    }
    //enemy cannons menu
    if(!ene->cannons.empty()){
        eneStCannons=true;
        start_menu(&enm,0,0,"cannons:",COLOR_WHITE,COLOR_BLACK,menuSelKey,menuUpKey,menuDownKey,menuBackKey,COLOR_WHITE,COLOR_YELLOW,"break",menuRightKey,' ','c','e','t','f','F','C','\0');
        for(int j=0;j<ene->cannons.size();j++){
            add_menuMember(&enm,"*",COLOR_WHITE,COLOR_WHITE);
        }
    }
    //enemy sails menu
    if(!ene->sails.empty()){
        eneStSails=true;
        start_menu(&esm,0,0,"sails:",COLOR_WHITE,COLOR_BLACK,menuSelKey,menuUpKey,menuDownKey,menuBackKey,COLOR_WHITE,COLOR_YELLOW,"break",menuRightKey,' ','c','e','t','f','F','C','\0');
        for(int j=0;j<ene->sails.size();j++){
            add_menuMember(&esm,"*",COLOR_WHITE,COLOR_WHITE);
        }
    }
    //enemy crew menu
    if(!ene->getCrew().empty()){
        eneStCrew=true;
        start_menu(&ecm,0,0,"crew:",COLOR_WHITE,COLOR_BLACK,menuSelKey,menuUpKey,menuDownKey,menuBackKey,COLOR_WHITE,COLOR_YELLOW,"break",menuLeftKey,' ','e','f','F','C','\0');
        for(int j=0;j<ene->getCrew().size();j++){
            add_menuMember(&ecm,"*",COLOR_WHITE,COLOR_WHITE);
        }
    }
    //set menus to nodelay mode
    nodelay_menu(&nm,1);
    nodelay_menu(&sm,1);
    nodelay_menu(&cm,1);
    nodelay_menu(&enm,1);
    nodelay_menu(&esm,1);
    nodelay_menu(&ecm,1);
    //start current menus
    curm=(stCannons ? &nm : &sm);
    curLm=curm;
    curRm=&cm;
}
//fight
void UserSFC::fight(Ship *ene){
    //initialize if necessary
    if(!start) init(ene);
    //keep track of time (for delaying purposes)
    int cycleTime=clock();
    //print what's not there
    if(!stSails && !stCannons) mvprintw(BAT_LMENU_Y,BAT_LMENU_X,"you have no buildings...");
    if(!stCrew) mvprintw(BAT_RMENU_Y,BAT_RMENU_X,"you have no crew members...");
    //update person default logs
    Creature::setLogs(comlog,batlog,batlog);
    //update and print health bars
    update_bar(&hpbar,getDeck()->getHp());
    update_bar(&enehpbar,ene->getDeck()->getHp());
    print_bar(hpbar);
    print_bar(enehpbar);
    //print ship placement menus
    move_menu(curLm,BAT_LMENU_X,BAT_LMENU_Y);
    move_menu(curRm,BAT_RMENU_X,BAT_RMENU_Y);
    print_menu(*curLm);
    print_menu(*curRm);
    //print menu info
    Menu *curms[2]={curLm,curRm};
    int xPos,yPos;
    for(int mj=0;mj<2;mj++){
        //left menu
        if(mj==0){
            xPos=BAT_LMENU_X;
            yPos=BAT_LMENU_Y;
        }
        //right menu
        else if(mj==1){
            xPos=BAT_RMENU_X;
            yPos=BAT_RMENU_Y;
        }
        //print it
        //cannons
        if(curms[mj]==&nm){
            for(int j=0;j<cannons.size();j++){
                cannons[j]->invPrint(xPos+1,yPos+2+j);
            }
        }
        //sails
        else if(curms[mj]==&sm){
            for(int j=0;j<sails.size();j++){
                sails[j]->invPrint(xPos+1,yPos+2+j);
            }
        }
        //crew
        else if(curms[mj]==&cm){
            for(int j=0;j<getCrew().size();j++){
                getCrew(j)->invPrint(xPos+1,yPos+2+j);
            }
        }
        //ene cannons
        if(curms[mj]==&enm){
            for(int j=0;j<ene->cannons.size();j++){
                ene->cannons[j]->invPrint(xPos+1,yPos+2+j);
            }
        }
        //ene sails
        else if(curms[mj]==&esm){
            for(int j=0;j<ene->sails.size();j++){
                ene->sails[j]->invPrint(xPos+1,yPos+2+j);
            }
        }
        //ene crew
        else if(curms[mj]==&ecm){
            for(int j=0;j<ene->getCrew().size();j++){
                ene->getCrew(j)->invPrint(xPos+1,yPos+2+j);
            }
        }
    }
    //print logs
    print_comm(*comlog);
    print_comm(*batlog);
    print_comm(*enelog);
    //print log boxes
    set_pair(COLOR_WHITE,COLOR_WHITE);
    for(int y=BAT_COMLOG_Y-1;y<=TERMINAL_MAX_Y;y++){
        for(int x=BAT_COMLOG_X;x<=TERMINAL_MAX_X;x++){
            if(y==BAT_COMLOG_Y-1) /*mvaddch(y,x,' ')*/;
            else if(y==TERMINAL_MAX_Y) /*mvaddch(y,x,' ')*/;
            else{
                if(x==BAT_LOG_X-1) mvaddch(y,x,' ');
                else if(x==BAT_ENELOG_X-1) mvaddch(y,x,' ');
            }
        }
    }
    set_pair(COLOR_WHITE,COLOR_BLACK);
    //!print timer (DB)
    mvprintw(0,TERMINAL_MAX_X-21,"time: %f",(float)(clock()-startTime));refresh();
    //get user input
    int choice=idinter_menu(curm);
    char com=curm->lastBreakKey;
    //change menu - from left to right
    if(com==menuRightKey) curm=curRm;
    //change menu - from crew to buildings
    else if(com==menuLeftKey) curm=curLm;
    //toggle build menu (sails/cannons)
    else if(com=='c'){
        //erase menus
        draw_rectangle(BAT_LMENU_X,BAT_LMENU_Y,BAT_HPBAR_X-1,BAT_COMLOG_Y-1,COLOR_BLACK,COLOR_BLACK,' ');
        //friend
        if(curm==&nm && stSails){
            if(curm==curLm) curLm=&sm;
            else if(curm==curRm) curRm=&sm;
            curm=&sm;
        }
        else if(curm==&sm && stCannons){
            if(curm==curLm) curLm=&nm;
            else if(curm==curRm) curRm=&nm;
            curm=&nm;
        }
        //enemy
        if(curm==&enm && eneStSails){
            if(curm==curLm) curLm=&esm;
            else if(curm==curRm) curRm=&esm;
            curm=&esm;
        }
        else if(curm==&esm && eneStCannons){
            if(curm==curLm) curLm=&enm;
            else if(curm==curRm) curRm=&enm;
            curm=&enm;
        }
    }
    //toggle menus - enemy and friend
    else if(com=='e'){
        //erase menus
        draw_rectangle(BAT_LMENU_X,BAT_LMENU_Y,BAT_HPBAR_X-1,BAT_COMLOG_Y-1,COLOR_BLACK,COLOR_BLACK,' ');
        int leftOrRight;
        //toggle left/right menus
        if(curm==curLm) leftOrRight=1;
        else leftOrRight=2;
        //friend to enemy
        if(curm==&cm) curm=eneStCrew ? &ecm : &cm;
        else if(curm==&nm) curm=eneStCannons ? &enm : (eneStSails ? &esm : &nm);
        else if(curm==&sm) curm=eneStSails ? &esm : (eneStCannons ? &enm : &sm);
        //enemy to friend
        if(curm==&ecm) curm=stCrew ? &cm : &ecm;
        else if(curm==&enm) curm=stCannons ? &nm : (stSails ? &sm : &enm);
        else if(curm==&esm) curm=stSails ? &sm : (stCannons ? &nm : &esm);
        //toggle left/right menus
        if(leftOrRight==1) curLm=curm;
        else curRm=curm;
    }
    //man stuff
    else if(com==menuSelKey){
        //crew menu - put selection in man to man
        if(curm==&cm){
            manToMan=getCrew(choice);
        }
        //building menus - man the selection with the man to man
        else if(manToMan!=NULL){
            if(!manToMan->isFree()) manToMan->free();
            if(curm==&nm) cannons[choice]->addPerson(manToMan);
            else if(curm==&sm) sails[choice]->addPerson(manToMan);
            manToMan=NULL;
        }
    }
    //toggle building selection
    else if(com=='t'){
        //toggle building selection
        if(curm==&nm) cannons[choice]->toggleSelect();
        else if(curm==&sm) sails[choice]->toggleSelect();
    }
    //free stuff
    else if(com=='f'){
        //crew menu - free crew member
        if(curm==&cm) getCrew(choice)->free();
        //building menus - free building completely
        else if(curm==&nm) cannons[choice]->free();
        else if(curm==&sm) sails[choice]->free();
    }
    //SHIP COMMANDS
    //fire!
    else if(com=='F'){
        fire(ene);
    }
    //charge cannons
    else if(com=='C'){
        chargeCannons();
    }
    //activate autofire
    else if(com=='A'){
        toggleAutoFire();
    }
    //pause battle
    else if(com==' '){
        pauseCannons();
        nodelay(stdscr,FALSE);
        clear();
        printInventory();
        clear();
        nodelay(stdscr,TRUE);
    }
    //update batlog
    curComlog=comlog;
    curBatlog=batlog;
    curEnelog=enelog;
    refresh();
    //delay some
    while(clock()-cycleTime<30);
}

///DUMMY Ship Fight Control class
void DummySFC::init(Ship *ene){
    start=true;
    //initialize logs
    comlog=UserSFC::curEnelog;
    batlog=NULL;
    enelog=UserSFC::curBatlog;
    //select all cannons
    for(int j=0;j<cannons.size();j++){
        cannons[j]->toggleSelect();
    }
    //man all cannons
    for(int j=0;j<getCrew().size() && j<cannons.size();j++){
        cannons[j]->addPerson(getCrew(j));
    }
}
void DummySFC::fight(Ship *ene){
    //initialize if necessary
    if(!start) init(ene);
    //initialize person default logs
    Creature::setLogs(comlog,NULL,NULL);
    //charge and fire cannons repeatedly
    chargeCannons();
    fire(ene);
}
