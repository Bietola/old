#ifndef H_PALETTE
#define H_PALETTE

#include<stdlib.h>
#include<time.h>
#include<curses.h>

///CONSTANTS
#define PALETTE_ERR_MAX_PAIRS -1

#define TERMINAL_MAX_X 79
#define TERMINAL_MAX_Y 24

#define COLOR_NULL 100
#define COLOR_RAND 101
#define COLOR_RAIN 102

///STRUCTURES
#ifdef __cplusplus
struct ClrGra{
    char gra;
    short fclr;
    short bclr;
    ClrGra():gra(' '),fclr(COLOR_WHITE),bclr(COLOR_BLACK){}
    ClrGra(char gra,short fclr,short bclr):gra(gra),fclr(fclr),bclr(bclr){}
};
#else
typedef struct ClrGra{
    char gra;
    short fclr;
    short bclr;
}ClrGra;
#endif

#ifdef __cplusplus
namespace{
#endif

///GLOBAL DATA
short cPalette=0;
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
            curFclr=fclr;
            curBclr=bclr;
            return 0; //success
        }
    }
    //if no pair is found, check if another one can be created
    cPalette++;
    if(cPalette>=COLOR_PAIRS) return PALETTE_ERR_MAX_PAIRS; //reached the maximum number of color pairs
    //if another pair can be created, create it and turn it on
    init_pair(cPalette,fclr,bclr);
    attron(COLOR_PAIR(cPalette));
    curFclr=fclr;
    curBclr=bclr;
    return 0; //success
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
void clear_rectangle(int sx,int sy,int ex,int ey){
    draw_rectangle(sx,sy,ex,ey,COLOR_BLACK,COLOR_BLACK,' ');
}
void fill_screen(short color){
    draw_rectangle(0,0,TERMINAL_MAX_X,TERMINAL_MAX_Y,color,color,' ');
}

#ifdef __cplusplus
}
#endif

#endif
