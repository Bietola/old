#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "Stringhe.h"

#define ERR_MULTIVAR 8
#define ERR_GRA      4
#define ERR_IND      2
#define ERR_INV      1

float prendiTerNum(int mov,char* proc,int* pos){
    int ERR=-123;
	int* indPos=pos;
	int memPos=*pos;
	float d=0.1;
	float num=1;
	int segno;
	int mod=1;
	int j=0;
	while(proc[*pos]!='\0' && proc[*pos]!='+' && proc[*pos]!='-' && proc[*pos]!='=' && conLet(proc[*pos])==0){
		if(chrToNum(proc[*pos])!=-1){
            if(j==0) num=0;
			if(mod==1) num=num*10+chrToNum(proc[*pos]);
			else{
				num+=chrToNum(proc[*pos])*d;
				d/=10;
			}
			*pos=(*pos)+1;
            j++;
		}
		else if(proc[*pos]=='.'){
			mod=2;
			*pos=(*pos)+1;
		}
		else if(proc[*pos]=='*' || proc[*pos]=='/'){
            char op=proc[*pos];
            float n;
            *pos=(*pos)+1;
            segno=1;
            while(proc[*pos]=='-' || proc[*pos]=='+'){
                if(proc[*pos]=='-') segno*=-1;
                *pos=(*pos)+1;
            }
            n=prendiTerNum(1,proc,indPos);
            if(n==ERR) return ERR;//ter. num. falso
            n*=segno;
			if(op=='*') num*=n;
			else if(op=='/') num/=n;
		}
		else{
			return ERR; //ter. num. falso
		}
	}
	if(mov==0){
		*pos=memPos;
	}
	return num;
}

//funzione che trova il risultato di equazioni di primo grado sotto forma di stringa (aka: stringhe di procedimento)
//DIFETTI: La funzione utilizza un sistema di restituzione di numeri speciali in caso di errore... il sistema non e' totalmente funzionale, e un errore potrebbe benissimo essere confuso con un normale risultato (WIP)
//         La funzione non permette la lettura di stringhe che presentano un segno di somma (+ o -) che e' situato subito dopo un coefficiente e subito prima di una variabile (ES: area=5*3*-area/2; il meno prima di "area" e dopo il "*" causa un comportamento anomalo della funzione) (WIP)
float risolvi_proc(char* proc){
    char var[LEN];        //buffer per contenere il nome della variabile
    float dNum,sNum;      //numeri a destra e a sinistra della variabile
    float coef;           //coefficiente variabile
    int segno=1;          //variabile che viene sempre moltiplicata per il risultato per ottenere il risultato finale (usata per determinare se un termine e' positivo o negativo)
    char dSegno,sSegno;   //segni a destra e a sinistra della variabile
    float totTerNot=0;    //TOTale TERmine NOTo
    float totCoef=0;      //TOTale COEFficiente
    int parte=1;          //parte dell'uguale (1=sinistra 2=destra)
    int pos=0;            //posizione nella stringa "proc"
    do{
        //calcolo del segno del prossimo monomio (positivo o negativo)
        segno=1;
        while(proc[pos]=='-' || proc[pos]=='+'){
            //se il segno e' un meno, viene cambiato di conseguenza il valore dalla variabile "segno"
            if(proc[pos]=='-') segno*=-1;
            pos++;
        }
        //prendi il primo termine numerico del monomio in questione (a sinistra di una presunta variabile)
        sNum=prendiTerNum(1,proc,&pos);
        ///printf("db:%f\n\tpos:%d(%c)",sNum,pos,proc[pos]);
        ///getchar();
        //se dopo il termine numerico si trova uno dei seguenti simboli, di sicuro non ci sara' una variabile.
        //Quindi viene aggiunto il termine numerico alla somma dei termini noti (il tipo di somma dipende dalla parte dell'uguale)
        if(proc[pos]=='\0' || proc[pos]=='+' || proc[pos]=='-' || proc[pos]=='='){
            //aggiungo a somma termine noto
            if(parte==1) totTerNot-=sNum*segno;
            else if(parte==2) totTerNot+=sNum*segno;
        }
        //altrimenti ci deve essere per forza una variabile. Il controllo e' comunque svolto per sicurezza
        else if(conLet(proc[pos])==1){
            //se il nome della variabile e' all'inizio della stringa proc, non esiste il coefficiente di sinistra (e quindi neanche il segno)
            if(pos==0){
                sSegno=0;      //si mette segno=0 per indicare che non esiste
            }
            //se prima della variabile c'e' un segno di somma o un uguale, non esiste il coefficiente di sinistra (e quindi neanche il segno)
            else if(proc[pos-1]=='+' || proc[pos-1]=='-' || proc[pos-1]=='='){
                sSegno=0;      //si mette segno=0 per indicare che non esiste
                //se si arriva all'uguale viene cambiato di conseguenza il valore della variabile "parte" (da controllare l'utilita')
                ///POTENZIALMENTE INUTILE f(proc[pos]=='=') parte=2;
            }
            //altrimenti, memorizza il segno alla sinistra della variabile (si presume che ci sia sempre un segno prima di una variabile)
            else sSegno=proc[pos-1];
            //memorizza il nome della variabile (inutile per ora)
            int j=0;
            while(conLet(proc[pos])==1){
                var[j]=proc[pos];
                j++;
                pos++;
            }
            var[j]='\0';
            ///printf("lettera time pos:%d(%c)\n",pos,proc[pos]);
            ///getchar();
            //controlla se esiste un'altro termine numerico dopo la variabile (verificando se c'e' un * o /)
            if(proc[pos]=='*' || proc[pos]=='/'){
                //memorizza il segno a destra della variabile
                dSegno=proc[pos];
                //prendi il termine numerico a destra della variabile (dopo il segno) (si presume che ci sia sempre un termine numerico
                //dopo un segno, se esso e' situato subito dopo una variabile, per la ragione che il procedimento dovrebbe essere risolvibile
                //(e quindi presentare solo un tipo di variabile) e di primo grado (WIP))
                pos++;
                dNum=prendiTerNum(1,proc,&pos);
                //calcola il coefficiente
                if(sSegno==0 && dSegno=='*'){
                    coef=dNum;
                }
                else if(sSegno==0 && dSegno=='/'){
                    coef=1/dNum;
                }
                else if(sSegno=='*' && dSegno=='*'){
                    coef=sNum*dNum;
                }
                else if(sSegno=='*' && dSegno=='/'){
                    coef=sNum/dNum;
                }
                else if(sSegno=='/' && dSegno=='*'){
                    return -2;      //equazione di secondo grado
                }
                else if(sSegno=='/' && dSegno=='/'){
                    return -2;      //equazione di secondo grado (da controllare)
                }
            }
            //se non c'e' un segno dopo calcolare il coefficiente usando solo il segno prima (se esiste)
            else if(sSegno!=0){
                if(sSegno=='*'){
                    coef=sNum;
                }
                else if(sSegno='/'){
                    return -2;     //equazione di secondo grado (da controllare)
                }
            }
            //se non esiste neanche il segno prima, il coefficiente della variabile e' uguale a uno
            else{
                coef=1;
            }
            //aggiunta alla somma (tipo di somma dipende da che parte si e' dell'uguale)
            if(parte==1) totCoef+=coef*segno;
            else if(parte==2) totCoef-=coef*segno;
        }
        //se una variabile non e' presente, allora la stringa di procedimento non e' corretta
        else{
            return -1;      //stringa di procedimento invalida
        }
        //se si arriva all'uguale viene cambiato di conseguenza il valore della variabile "parte"
        if(proc[pos]=='='){
            parte=2;
            pos++;
        }
        //si noti che la posizione non viene incrementata se a proc[pos] si trova un terminatore
    }while(proc[pos]!='\0');
    //debug
    printf("totCoef:%.2f\ttotTerNot:%.2f\t",totCoef,totTerNot);
    //calcolo e restituzione del risultato della stringa di procedimento
    return totTerNot/totCoef;
}

//CONtrolla RISolvibilita': determina la risolvibilita' di una stringa di procedimento. Restituisce valori diversi a seconda dell'errore
int conRis(char* proc){
    int err=0;
    //si noti che si usa lo stesso procedimento nella funzione "risolvi_proc()" ma senza il calcolo del termine numerico (che non serve per determinare eventuali errori)
    //serve comunque calcolare il coefficiente totale per determinare la indeterminazione dell'equazione o se essa supera il primo grado
    //serve anche leggiere la stringa come se la si stesse risolvendo, per determinare l'invalididita' della stringa
    char priVar[LEN];
    float dNum,sNum;
    float coef;
    int segno=1;
    char dSegno,sSegno;
    float totCoef=0;
    int parte=1;
    int pos=0;
    //
    priVar[0]='\0';
    do{
        segno=1;
        while(proc[pos]=='-' || proc[pos]=='+'){
            if(proc[pos]=='-') segno*=-1;
            pos++;
        }
        sNum=prendiTerNum(1,proc,&pos);
        if(conLet(proc[pos])==1){
            if(pos==0){
                sSegno=0;
            }
            else if(proc[pos-1]=='+' || proc[pos-1]=='-' || proc[pos-1]=='='){
                sSegno=0;
                ///POTENZIALMENTE INUTILE if(proc[pos-1]=='=') parte=2;
            }
            else if(proc[pos-1]=='*' || proc[pos-1]=='/'){
                sSegno=proc[pos-1];
            }
            //se davanti a una variabile c'e' un carattere che non e' un segno di somma (+,-), un uguale, o un segno di operazione (*,/) allora la stringa e' invalida
            //WIP: i numeri direttamente prima delle variabili non sono sopportati
            else{
                return ERR_INV;
            }
            //si controlla se esiste solo una variabile nella funzione. Se c'e' ne sono di piu', si restituisce un errore
            if(priVar[0]=='\0'){
                int j=0;
                while(conLet(proc[pos])==1 || proc[pos]=='.'){
                    priVar[j]=proc[pos];
                    j++;
                    pos++;
                }
                priVar[j]='\0';
            }
            else{
                char var[LEN];
                int j=0;
                while(conLet(proc[pos])==1 || proc[pos]=='.'){
                    var[j]=proc[pos];
                    j++;
                    pos++;
                }
                var[j]='\0';
                if(strcom(var,priVar)==0) return ERR_MULTIVAR;      //esiste piu' di una variabile
            }
            //
            if(proc[pos]=='*' || proc[pos]=='/'){
                dSegno=proc[pos];
                pos++;
                dNum=prendiTerNum(1,proc,&pos);
                if(sSegno==0 && dSegno=='*'){
                    coef=dNum;
                }
                else if(sSegno==0 && dSegno=='/'){
                    coef=1/dNum;
                }
                else if(sSegno=='*' && dSegno=='*'){
                    coef=sNum*dNum;
                }
                else if(sSegno=='*' && dSegno=='/'){
                    coef=sNum/dNum;
                }
                else if(sSegno=='/' && dSegno=='*'){
                    return ERR_GRA;     //equazione di secondo grado
                }
                else if(sSegno=='/' && dSegno=='/'){
                    return ERR_GRA;     //equazione di secondo grado (da controllare)
                }
            }
            else if(sSegno!=0){
                if(sSegno=='*'){
                    coef=sNum;
                }
                else if(sSegno='/'){
                    return ERR_GRA;     //equazione di secondo grado (da controllare)
                }
            }
            else{
                coef=1;
            }
            if(parte==1) totCoef+=coef*segno;
            else if(parte==2) totCoef-=coef*segno;
        }
        else if(proc[pos]!='\0' && proc[pos]!='+' && proc[pos]!='-' && proc[pos]!='='){
            return ERR_INV;
        }
        if(proc[pos]=='='){
            parte=2;
            pos++;
        }
    }while(proc[pos]!='\0');
    if(totCoef==0) return ERR_IND;      //equazione indeterminata
    if(parte==1) return ERR_INV;        //stringa invalida perche' senza uguale
    return 0;   //stringa risolvibile
}

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
        err=conRis(proc);
        if(err!=0){
            if(err==ERR_MULTIVAR) printf("equazione contiene piu' di una variabile\n");
            else if(err==ERR_GRA) printf("equazione di secondo grado\n");
            else if(err==ERR_IND) printf("equazione indeterminata\n");
            else if(err==ERR_INV) printf("equazione invalida\n");
            printf("\n");
        }
        else printf("ris:%f\n\n",risolvi_proc(proc));
    }while(proc!="END");
	return 0;
}
