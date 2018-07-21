#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LEN 100
#define ASCII_NUM_SHIFT '0'

//semplice funzione per determinare il minore tra due numeri
int minore(int a,int b){
	if(a<b) return a;
	return b;
}

//funzione per riallocare le stringhe nel modo più efficente possibile (fino al terminatore)
void taglia(char* str){
	int j;
	for(j=0;j<strlen(str);j++){
		if(str[j]=='\0'){
			str=(char*)realloc(str,sizeof(char)*j);
			break;
		}
	}
}

//CHaR TO STRing
char* chrToStr(char c){
	char* str=(char*)malloc(sizeof(char)*2);
	str[0]=c;
	str[1]='\0';
	return str;
}

//NUMber TO STRing
char* numToStr(int n){
	char* str=(char*)malloc(sizeof(char)*LEN);
	int c,j,max;
	for(c=0,max=1;n-max>0;c++,max*=10);
	str[c--]='\0';
	for(j=10;j<=max;j*=10,c--){
		str[c]=((n%j)/(j/10))+ASCII_NUM_SHIFT;
	}
	return str;
}

//CHaR TO NUMber
int chrToNum(char c){
	int n=c+ASCII_NUM_SHIFT;
	if(n<0 || n>9) return -1; //il carattere non è una cifra
	return n;
}

//STRing COMpare: funzione per il confronto di due stringhe
int strcom(char* str1,char* str2){
	int j;
	for(j=0;j<minore(strlen(str1),strlen(str2));j++){
		if(str1[j]!=str2[j]) return 0;	//stringhe non uguali
	}
	return 1;	//stringhe uguali
}

//STRing GET: funzione per ricevere una stringa dall'utente
char* strget(){
	char* str=(char*)malloc(sizeof(char)*LEN);
	int j=0;
	do{
		str[j]=getchar();
		if(str[j]=='\n'){
			printf("\n%s\n",str);
			str[j]='\0';
		}
		j++;
	}while(str[j]!='\0');
	taglia(str);
	return str;
}

//STRing ADD: unisce due stringhe in un'unica stringa
char* stradd(char* str1,char* str2){
	int j;
	char* str=(char*)malloc(sizeof(char)*(strlen(str1)+strlen(str2)+1));
	for(j=0;j<strlen(str1);j++){
		str[j]=str1[j];
	}
	for(j=0;j<strlen(str2);j++){
		str[j+strlen(str1)]=str2[j];
	}
	str[j+strlen(str1)]='\0';
	return str;
}

//SOSTituzione: sostituisce una stringa (cStr) con un'altra (sStr) all'interno di un'altra stringa data (tStr)
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
