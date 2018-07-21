#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "Stringhe.h"

int main(){
    float f;
    int pre;
    do{
        scanf("%f",&f);
        printf("%s\n\n",fltToStr(f));
    }while(f!=0);
    return 1;
}
