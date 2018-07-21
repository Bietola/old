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

///GLOBAL DATA
extern short cPalette;
extern short curFclr;
extern short curBclr;

///FUNCTIONS
short set_pair(short fclr,short bclr);
void set_old_pair();
void draw_rectangle(int sx,int sy,int ex,int ey,short fclr,short bclr,char gra);
void clear_line(int x,int y,int space);
void clear_rectangle(int sx,int sy,int ex,int ey);
void fill_screen(short color);

#endif
