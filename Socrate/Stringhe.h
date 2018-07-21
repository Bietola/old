#ifndef STRINGHE_H
#define STRINGHE_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define LEN 100


int minore(int,int);
int conLet(char);
void taglia(char*);
char* intToStr(int);
char* fltToStr(float,int);
int chrToNum(char);
int sost(char*,char*,char*);


//semplice funzione per determinare il minore tra due numeri
int minore(int a,int b){
	if(a<b) return a;
	return b;
}

//CONTRollo LETTERA
int conLet(char c){
    if((c>='a' && c<='z') || (c>='A' && c<='Z')) return 1;  //true
    return 0;   //false
}

//funzione per riallocare le stringhe nel modo piu' efficiente possibile (fino al terminatore)
void taglia(char* str){
	int j;
	for(j=0;j<strlen(str);j++){
		if(str[j]=='\0'){
			str=(char*)realloc(str,sizeof(char)*j);
			break;
		}
	}
}

//INT TO STRing
char* intToStr(int n){
	char* str=(char*)malloc(sizeof(char)*LEN);
	int j,mn=n;
	if(n==0){
		str[0]='0';
		str[1]='\0';
		taglia(str);
		return str;
	}
	for(j=0;n>0;j++){
        n/=10;
	}
	str[j]='\0';
	j--;
	n=mn;
	while(j>=0){
		str[j]=n%10+'0';
		n/=10;
		j--;
	}
	taglia(str);
	return str;
}

//FLoaT TO STRing
char* fltToStr(float f,int pre){
    //int pre: usata per contenere il massimo numero dELLE cifre che possono essere dopo la virgola
    int j=0; //contatore. Usato per la precisione.
    //preparazione della stringa da restituire
    char* str;
    //acquisizione termine numerico senza decimali
    int n=(int)f;
    //acquisizione termine numerico decimale
    float d=f-(int)f;
    //
    if(d==0) return intToStr((int)f);
    //
    while(d-(int)d!=0 && j<pre){
        d*=10;
        j++;
    }
    //aggiunzione n
    str=intToStr(n);
    //aggiunzione del punto
    strcat(str,".");
    //aggiunzione d
    strcat(str,intToStr(d));
    //restituzione str
    taglia(str);
    return str;
}

//CHaR TO NUMber
int chrToNum(char c){
	int n=c-'0';
	if(n<0 || n>9) return -1; //il carattere non è una cifra
	return n;
}

//SOSTituzione: sostituisce una stringa (cStr) con un'altra (sStr) all'interno di un'altra stringa data (tStr)
//DIFETTI: viene usata in "Socrate" per la sostituzione dei dati nelle stringhe di procedimento, ma non basta. Infatti, per esempio, se un dato si chiama "diametr", verra' sostituito
//         parzialmente a una stringa che contiene il dato "diametro". (WIP)
int sost(char* tStr,char* cStr,char* sStr){		//Trova String, Cerca String, Sost String
	int ns=0,jt,jc,js,posInc,posCop;
	for(jt=0;jt<strlen(tStr);jt++){
		for(jc=0;jc<strlen(cStr);jc++){
			if(tStr[jt+jc]!=cStr[jc]) break;
			//sostituisci
			else if(jc==strlen(cStr)-1){
				//aumento della variabile dedicata alla conta delle stringhe sostituite
				ns++;
				//fase di sostituzione
				for(js=0;js<strlen(sStr);js++){
					tStr[jt+js]=sStr[js];
				}
				//fase di riposizionamento
				for(posInc=strlen(sStr)+jt,posCop=strlen(cStr)+jt;posCop<=strlen(tStr);posInc++,posCop++){
					tStr[posInc]=tStr[posCop];
				}
			}
		}
	}
	return ns;
}

#endif
