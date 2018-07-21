#include "palette.h"

///GLOBAL DATA
short cPalette=0;
short oldFclr=COLOR_WHITE;
short oldBclr=COLOR_BLACK;
short curFclr=COLOR_WHITE;
short curBclr=COLOR_BLACK;

///FUNCTIONS
short set_pair(short fclr,short bclr){
    int j;
    short f,b;
    //choose a random color
    if(fclr==COLOR_RAIN) fclr=rand()%7+1;
    if(bclr==COLOR_RAIN) bclr=rand()%7+1;
    if(fclr==COLOR_RAND) fclr=rand()%8;
    if(bclr==COLOR_RAND) bclr=rand()%8;
    //search for the desired pair in already existing ones
    for(j=1;j<=cPalette;j++){
        pair_content(j,&f,&b);
        //if the pair is found, turn it on
        if(f==fclr && b==bclr){
            attron(COLOR_PAIR(j));
            oldFclr=curFclr;
            oldBclr=curBclr;
            curFclr=fclr;
            curBclr=bclr;
            return 0; //success
        }
    }
    //if no pair is found, check if another one can be created
    cPalette++;
    if(cPalette>=COLOR_PAIRS){
        mvprintw(0,0,"REACHED MAX PAIRS");
        refresh();
        getch();
        return PALETTE_ERR_MAX_PAIRS; //reached the maximum number of color pairs
    }
    //if another pair can be created, create it and turn it on
    init_pair(cPalette,fclr,bclr);
    attron(COLOR_PAIR(cPalette));
    oldFclr=curFclr;
    oldBclr=curBclr;
    curFclr=fclr;
    curBclr=bclr;
    return 0; //success
}
void set_old_pair(){
    return set_pair(oldFclr,oldBclr);
}
void draw_rectangle(int sx,int sy,int ex,int ey,short fclr,short bclr,char gra){
    int x,y;
    short lastFclr=curFclr;
    short lastBclr=curBclr;
    set_pair(fclr,bclr);
    for(y=sy;y<=ey;y++){
        for(x=sx;x<=ex;x++){
            if(fclr>=COLOR_RAND || bclr>=COLOR_RAND) set_pair(fclr,bclr);
            mvaddch(y,x,gra);
        }
    }
    set_pair(lastFclr,lastBclr);
}
void clear_line(int x,int y,int space){
	int j;
	for(j=0;j<space;j++){
		mvaddch(y,x+j,' ');
	}
}
void clear_rectangle(int sx,int sy,int ex,int ey){
    draw_rectangle(sx,sy,ex,ey,COLOR_BLACK,COLOR_BLACK,' ');
}
void fill_screen(short color){
    draw_rectangle(0,0,TERMINAL_MAX_X,TERMINAL_MAX_Y,color,color,' ');
}
