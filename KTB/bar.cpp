#include "bar.h"

///FUNCTIONS
//initialize bar
void start_bar(Bar *bar,bar_t type,int x,int y,int length,double maxVal,double curVal,ClrGra noGra,ClrGra yesGra){
    bar->type=type;
    bar->x=x;
    bar->y=y;
    bar->length=length;
    bar->maxVal=maxVal;
    bar->curVal=curVal;
    bar->noGra=noGra;
    bar->yesGra=yesGra;
    bar->s=NULL;
}
void extstart_bar(Bar *bar,bar_t type,int x,int y,int length,double maxVal,double curVal,short noFclr,short noBclr,char noGra,short yesFclr,short yesBclr,char yesGra){
    bar->type=type;
    bar->x=x;
    bar->y=y;
    bar->length=length;
    bar->maxVal=maxVal;
    bar->curVal=curVal;
    bar->noGra.fclr=noFclr;
    bar->noGra.bclr=noBclr;
    bar->noGra.gra=noGra;
    bar->yesGra.fclr=yesFclr;
    bar->yesGra.bclr=yesBclr;
    bar->yesGra.gra=yesGra;
    bar->s=NULL;
}
void sstart_bar(Bar *bar,bar_t type,int x,int y,const char *s,int length,double maxVal,double curVal,ClrGra noGra,ClrGra yesGra){
    bar->type=type;
    bar->x=x;
    bar->y=y;
    bar->length=length;
    bar->maxVal=maxVal;
    bar->curVal=curVal;
    bar->noGra=noGra;
    bar->yesGra=yesGra;
    bar->s=s;
}
//update bar
void update_bar(Bar *bar,double val){
    bar->curVal=val;
}
//print bar
void print_bar(Bar bar){
    //get relative value
    int relVal=(int)((bar.curVal*(double)bar.length)/bar.maxVal);
    //print bar
    int x=0,y=0,val=0;
    while(x<bar.length && y<bar.length){
        ClrGra clgra=(val<relVal ? bar.yesGra : bar.noGra);
        set_pair(clgra.fclr,clgra.bclr);
        if(bar.s==NULL || val>=strlen(bar.s)) mvaddch(bar.y+y,bar.x+x,clgra.gra);
        else mvaddch(bar.y+y,bar.x+x,bar.s[val]);
        if(bar.type==BAR_HORIZONTAL) x++;
        else if(bar.type==BAR_VERTICAL) y++;
        val++;
    }
}
