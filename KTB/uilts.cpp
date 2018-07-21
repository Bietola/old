#include "utils.h"

///macros
#define PARA(s) para("s\0")

///randomness functions
int rrange(int min,int max){
    if(max==0) return 0;
    return (rand()%(max-min))+min;
}
int rrange(int max){
    return rrange(0,max);
}
bool perc(int p){
    if(rrange(100)<p) return true;
    return false;
}
bool pert(int p){
    if(rrange(1000)<p) return true;
    return false;
}

///string related functions
char capitalize(char c){
    return c-32;
}
void capitalize(std::string &s){
    s[0]=capitalize(s[0]);
}
std::string retCapitalize(std::string s){
    s[0]=capitalize(s[0]);
    return s;
}

///output functions
int para(std::string s){
    return para((s+"\0").c_str());
}
int para(const char *s){
    return para(s,TERMINAL_MAX_X);
}
int para(const char *s,int maxSpaces){
    set_pair(COLOR_WHITE,COLOR_BLACK);
    int spaces=0,lines=1;
    for(int j=0;s[j]!='\0';j++){
        char word[100];
        int k;
        for(k=0;s[j+k]!=' ' && s[j+k]!='\0';k++){
            spaces++;
            word[k]=s[j+k];
        }
        word[k]='\0';
        if(spaces<maxSpaces){
            spaces++;
            j+=k;
            printw("%s ",word);
        }
        else{
            lines++;
            spaces=0;
            j--;
            addch('\n');
        }
    }
    return lines;
}
int para(short fclr,short bclr,const char *s){
    return para(fclr,bclr,s,TERMINAL_MAX_X);
}
int para(short fclr,short bclr,const char *s,int maxSpaces){
    short mfclr=curFclr,mbclr=curBclr;
    int spaces=0,lines=0,wordlen;
    set_pair(fclr,bclr);
    for(int j=0;s[j]!='\0';j++){
        char word[100];
        int k;
        wordlen=0;
        for(k=0;s[j+k]!=' ';k++){
            spaces++;
            wordlen++;
            word[k]=s[j+k];
        }
        word[k]='\0';
        if(spaces<=maxSpaces){
            spaces++;
            j+=k;
            printw("%s ",word);
        }
        else{
            lines++;
            for(spaces-=wordlen;spaces<maxSpaces;spaces++) addch(' ');
            spaces=0;
            j--;
            for(int i=0;i<=maxSpaces;i++) addch(' ');
            addch('\r');
        }
    }
    set_pair(mfclr,mbclr);
    return lines;
}

///debugging functions
namespace db{
    void stop(const char *mes){
        addstr(mes);
        refresh();
        getch();
    }
    void stop(char c){
        addch(c);
        refresh();
        getch();
    }
}
