/*Nome Programma : 		String Solver
 *Programmatore  : 		Stefano Montesi
 *Utlimo Aggiornamento:	23/4/2014 11:15
 *Scopo:				Il programma prende come input stringhe di procedimento (equazioni sotto forma di stringhe) e
                        le usa per sfruttare al massimo le funzioni offerte dalla funzione "risolvi_proc()" nel file Proc.h.
                        Il programma e' nato per puri scopi di debug nei confronti della funzione citata.
 */


#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "Stringhe.h"
#include "Proc.h"


///MAIN
int main(){
    int pos=0;
	char proc[LEN];
	float terNum;
	float ris;
	int err;
	float pre;
	char com;
	int db=0;
	//input precisione
	printf("AVVISO:\tin questo programma, una precisione maggiore di 0 sarebbe inutile\n\te contribuirebbe solamente ad aggiungere degli errori\n\tnella determinazione dell'impossibilita'.\n\tL'input e' qui solamente per scopi di debug.\n");
	printf("inserire la precisione di errore desiderata: ");
	scanf(" %f",&pre);
	//input debug  errori
	printf("\nattivare funzioni di debug visivo? (s/n): ");
	scanf(" %c",&com);
	printf("\n");
	if(com=='s') db=PROC_DB_COEF+PROC_DB_TN+PROC_DB_MOD;
	//main loop
	do{
	    printf("str proc: ");
        scanf("%s",proc);
        err=risolvi_proc(proc,pre,PROC_ERR_PRINT+PROC_ERR_RET);
        if(err==0) printf("ris: %f\n",risolvi_proc(proc,pre,db));
        printf("\n");
    }while(proc!="END");
	return 0;
}
