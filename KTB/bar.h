#ifndef BAR_H
#define BAR_H

#include <assert.h>
#include <string.h>

#include <curses.h>

#include "palette.h"

///ENUMERATIONS
typedef enum{
    BAR_HORIZONTAL,
    BAR_VERTICAL,
}bar_t;

///STRUCTURES
typedef struct{
    bar_t type;
    int x,y;
    int length;
    double maxVal;
    double curVal;
    ClrGra noGra,yesGra;
    const char *s;
}Bar;

///FUNCTIONS
//initialize bar
void start_bar(Bar *bar,bar_t type,int x,int y,int length,double maxVal,double curVal,ClrGra noGra,ClrGra yesGra);
void extstart_bar(Bar *bar,bar_t type,int x,int y,int length,double maxVal,double curVal,short noFclr,short noBclr,char noGra,short yesFclr,short yesBclr,char yesGra);
void sstart_bar(Bar *bar,bar_t type,int x,int y,const char *s,int length,double maxVal,double curVal,ClrGra noGra,ClrGra yesGra);
//update bar
void update_bar(Bar *bar,double val);
//print bar
void print_bar(Bar bar);

#endif
