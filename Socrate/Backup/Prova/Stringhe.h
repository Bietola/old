#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define LEN 100
#define ASCII_NUM_SHIFT '0'

#define ERR_C 'X'

int minore(int,int);
int contrLet(char);
void taglia(char*);
char* chrToStr(char);
char* intToStr(int);
char* fltToStr(float,int);
int chrToNum(char);
int strcom(char*,char*);
char* strget();
char* stradd(char*,char*);
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

//INT TO STRing
char* intToStr(int n){
	char* str=(char*)malloc(sizeof(char)*LEN);
	int j,mn=n;
	for(j=0;n>0;j++,n/=(j*10));
	str[--j]='\0';
	n=mn;
	while(j>=0){
		str[j]=n%10+ASCII_NUM_SHIFT;
		n/=10;
		j--;
	}
	taglia(str);
	return str;
}

//FLoaT TO STRing
char* fltToStr(float f,int pre){	//Float, PREcisione
	int j,pp,pz=-1;	//j - contatore; p - Posizione Punto; pz - Posizione di inizio Zeri inutili
	char* str=(char*)malloc(sizeof(char)*LEN);
	int n=(int)f;		//Numero intero
	float d=f-(int)f;	//parte Decimale
	int mn=n;
	//trasformazione n
	for(j=0;n>1;j++,n/=(j*10));
	n=mn;
	pp=j+1;
	while(j>=0){
		str[j]=n%10+ASCII_NUM_SHIFT;
		n/=10;
		j--;
	}
	if(d!=0){
		//punto
		str[pp]='.';
		//trasformazione d
		for(j=pp+1;j<=pre+1;j++){
			/*DEBUG*/printf("db1 : %f\t",d);
			d*=10;
			/*DEBUG*/printf("db2 : %f\t",d);
			str[j]=(int)d+ASCII_NUM_SHIFT;
			/*DEBUG*/printf("db3 : %c\t",str[j]);
			d-=(int)d;
			/*DEBUG*/printf("db4 : %f\n",d);
			if(str[j]=='0' && pz==-1) pz=j;
			if(str[j]!='0') pz=-1;
		}
	}
	//rimozione degli zero in eccesso
	if(pz!=-1) j=pz;
	str[j]='\0';
	//fine
	taglia(str);
	return str;
}

//CHaR TO NUMber
int chrToNum(char c){
	int n=c-ASCII_NUM_SHIFT;
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
