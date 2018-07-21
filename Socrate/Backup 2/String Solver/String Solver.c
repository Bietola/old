#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "Stringhe.h"
#include "Proc.h"

//main
int main(){
    int pos=0;
	char proc[LEN];
	float terNum;
	float ris;
	int err;
	do{
	    printf("str proc: ");
        gets(proc);
        err=risolvi_proc(proc,PROC_ERR_PRINT+PROC_ERR_RET+PROC_DB_COEF+PROC_DB_MOD);
        if(err==0) printf("ris: %f\n",risolvi_proc(proc,0));
        printf("\n");
    }while(proc!="END");
	return 0;
}
