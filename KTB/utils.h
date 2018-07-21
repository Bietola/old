#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdlib>
#include <curses.h>

#include "palette.h"

#define DB_MODE 0

///macros
/*#define uptxty do{static int j=0;if(s[j]=='\n') txty++;j++;}while(s[j]!='\0');
#define txtf(form,data) mvprintw(txty,0,form,data); uptxty
#define txts(s) mvaddstr(txty,0,s); uptxty*/

///randomness functions
int rrange(int min,int max);
int rrange(int max);
bool perc(int p);
bool pert(int p);

///string related functions
char capitalize(char c);
void capitalize(std::string &s);
std::string retCapitalize(std::string s);

///output functions
int para(std::string s);
int para(const char *s);
int para(const char *s,int maxSpaces);
int para(short fclr,short bclr,const char *s);
int para(short fclr,short bclr,const char *s,int maxSpaces);

///debugging functions
namespace db{
    void stop(const char *mes);
    void stop(char c);
}

#endif
