#ifndef PROC_H
#define PROC_H


#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#define PROC_ERR_RET   1
#define PROC_DB_TN     2
#define PROC_DB_COEF   4
#define PROC_DB_MOD    8
#define PROC_ERR_PRINT 16

#define ERR_MULTIVAR 1
#define ERR_GRA      2
#define ERR_IMP      3
#define ERR_IND      4
#define ERR_INV      5
#define ERR_DPZ		 6


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

//funzione che trova il risultato di una equazione di primo grado sotto forma di stringa (aka: stringhe di procedimento)
//DIFETTI: La funzione utilizza un sistema di restituzione di numeri speciali in caso di errore... il sistema non e' totalmente funzionale, e un errore potrebbe benissimo essere confuso con un normale risultato. Se si vuole usare la funzione in questo modo, c'e' il bisogno di specificarlo nei parametri
//         La funzione non permette la lettura di stringhe che presentano un segno di somma (+ o -) che e' situato subito dopo un coefficiente e subito prima di una variabile (ES: area=5*3*-area/2; il meno prima di "area" e dopo il "*" causa un comportamento anomalo della funzione) (WIP)
float risolvi_proc(char* proc,float errPre,int mod){
    //int errPre: quando viene trovata un'equazione apparentemente indeterminata (totCoef=0) per determinare se la funzione e' impossibile, si dovrebbe normalmente controllare se totTerNot!=0.
    //            Il controllo, invece, e' il seguente: if(totTerNot<-errPre || totTerNot>errPre). Questo per via delle svariate imprecisioni che avvengono durante le operazioni tra float.
    //            Maggiore e' il valore di questa variabile, minore e' l'imprecisione della determinazione dell'impossibilita' di un equazione; l'imprecisione pero' aumenta al diminuire del valore
    //            di totTerNot (se esso e' addirittura piu' basso, si rischia un comportamento anomalo nella determinazione del tipo di errore)
	char priVar[LEN];	  //buffer per contenere il nome della prima variabile trovata
    char var[LEN];        //buffer per contenere il nome delle prossime variabili (metodo usato per controllare la presenza di piu' nomi di variabili nella stringa)
    float dNum,sNum;      //numeri a destra e a sinistra della variabile
    float coef;           //coefficiente variabile
    int modCoef=0;        //MODalita' COEFficiente: variabile per capire che tipo di coefficiente si sta usando.
                          //-)se =0 -> la modalita' non e' ancora stata determinata
                          //-)se =1 -> modalita' normale. Con "coefficiente" si intende la quantita' numerica MOLTIPLICATA per la variabile
                          //-)se =2 -> modalita' invertita. Con "coefficiente" si intende la quantita' numerica DIVISA per la variabile
    int segno=1;          //variabile che viene sempre moltiplicata per il risultato per ottenere il risultato finale (usata per determinare se un termine e' positivo o negativo)
    char dSegno,sSegno;   //segni a destra e a sinistra della variabile
    float totTerNot=0;    //TOTale TERmine NOTo
    float totCoef=0;      //TOTale COEFficiente
    int parte=1;          //parte dell'uguale (1=sinistra 2=destra)
    int pos=0;            //posizione nella stringa "proc"
    int err=0;            //variabile che contiene il valore di un eventuale errore durante la risoluzione della stringa. La variabile e' restituita se diversa da 0.
    //inizializzazione di priVar (per far capire che non e' stata ancora utilizzata)
    *priVar='\0';
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
            }
            //altrimenti, memorizza il segno alla sinistra della variabile (si presume che ci sia sempre un segno prima di una variabile)
            else sSegno=proc[pos-1];
            //memorizza il nome della variabile in priVar; in caso fosse la prima variabile incontrata
            if(*priVar=='\0'){
	        	int j=0;
	            while(conLet(proc[pos])==1 || proc[pos]=='.'){
	                priVar[j]=proc[pos];
	                j++;
	                pos++;
	            }
	            priVar[j]='\0';
	    	}
	    	//altrimenti, memorizza il nome in "var" e controlla se e' uguale a quello in "priVar". Se non lo e', restituisci un errore (ERR_MULTIVAR)
	    	else{
	    		int j=0;
	    		while(conLet(proc[pos])==1 || proc[pos]=='.'){
	                var[j]=proc[pos];
	                j++;
	                pos++;
	            }
	            var[j]='\0';
	            if(strcmp(priVar,var)!=0) err=ERR_MULTIVAR;
	    	}
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
                    ///il seguente processo di determinazione della modalita' e' ripetuto in tutti i casi
                    //si cambia la modalita' a "normale (1)" (o a "invertita" (2) in altri casi)
                    if(modCoef==0) modCoef=1;
                    //se la modalita' e' gia' stata determinata, e non e' uguale alla modalita' che si vuole stabilire, l'equazione e' sicuramente di grado superiore al primo
                    else if(modCoef==2) err=ERR_GRA;
                    //assegnazione del coefficiente
                    coef=dNum;
                }
                else if(sSegno==0 && dSegno=='/'){
                    if(modCoef==0) modCoef=1;
                    else if(modCoef==2) err=ERR_GRA;
                    coef=1/dNum;
                }
                else if(sSegno=='*' && dSegno=='*'){
                    if(modCoef==0) modCoef=1;
                    else if(modCoef==2) err=ERR_GRA;
                    coef=sNum*dNum;
                }
                else if(sSegno=='*' && dSegno=='/'){
                    if(modCoef==0) modCoef=1;
                    else if(modCoef==2) err=ERR_GRA;
                    coef=sNum/dNum;
                }
                else if(sSegno=='/' && dSegno=='*'){
                    if(modCoef==0) modCoef=2;
                    else if(modCoef==1) err=ERR_GRA;
                    coef=sNum*dNum;
                }
                else if(sSegno=='/' && dSegno=='/'){
                    if(modCoef==0) modCoef=2;
                    else if(modCoef==1) err=ERR_GRA;
                    coef=sNum/dNum;
                }
            }
            //se non c'e' un segno dopo, calcolare il coefficiente usando solo il segno prima (se esiste)
            else if(sSegno!=0){
                if(sSegno=='*'){
                    if(modCoef==0) modCoef=1;
                    else if(modCoef==2) err=ERR_GRA;
                    coef=sNum;
                }
                else if(sSegno='/'){
                    if(modCoef==0) modCoef=2;
                    else if(modCoef==1) err=ERR_GRA;
                    coef=sNum;
                }
            }
            //se non esiste neanche il segno prima, il coefficiente della variabile e' uguale a uno
            else{
                if(modCoef==0) modCoef=1;
                else if(modCoef==2) err=ERR_GRA;
                coef=1;
            }
            //aggiunta alla somma (tipo di somma dipende da che parte si e' dell'uguale)
            if(err==0){
                if(parte==1) totCoef+=coef*segno;
                else if(parte==2) totCoef-=coef*segno;
            }
        }
        //se una variabile non e' presente, allora la stringa di procedimento non e' corretta
        else{
            err=ERR_INV;      //stringa di procedimento invalida
        }
        //restituzione e stampo (se necessario) di eventuali errori avvenuti durante la risoluzione
        if(err!=0){
            if(mod-PROC_ERR_PRINT>=0){
                if(err==ERR_MULTIVAR) printf("ERR: equazione contenente piu' di una variabile\n");
                else if(err==ERR_GRA) printf("ERR: equazione di grado superiore al primo\n");
                else if(err==ERR_INV) printf("ERR: stringa di procedimento invalida\n");
            }
            return err;
        }
        //se si arriva all'uguale viene cambiato di conseguenza il valore della variabile "parte"
        if(proc[pos]=='='){
            parte=2;
            pos++;
        }
        //si noti che la posizione non viene incrementata se a proc[pos] si trova un terminatore
    }while(proc[pos]!='\0');
    //determinazione di possibili errori
    if(totCoef==0){
    	if(totTerNot>-errPre && totTerNot<errPre) err=ERR_IND; //equazione indeterminata
		else err=ERR_IMP; //equazione impossibile
	}
    if(modCoef==2 && totTerNot==0) err=ERR_DPZ; //divisione per zero
    if(parte==1) err=ERR_INV;                   //stringa invalida perche' senza uguale
    //restituzione e stampo (se necessario) di eventuali errori avvenuti durante la risoluzione
    if(mod-PROC_ERR_PRINT>=0){
        if(err==ERR_IND) printf("ERR: equazione indeterminata\n");
        else if(err==ERR_IMP) printf("ERR: equazione impossibile\n");
        else if(err==ERR_INV) printf("ERR: stringa di procedimento invalida\n");
        else if(err==ERR_DPZ) printf("ERR: equazione impossibile (divisione per 0)\n");
        mod-=PROC_ERR_PRINT;
    }
    if(err!=0) return err;
    //output a schermo (solo se mod ha determinati valori) (per ragioni di debug)
    if(mod-PROC_DB_MOD>=0){
        printf("modCoef: %d\n",modCoef);
        mod-=PROC_DB_MOD;
    }
    if(mod-PROC_DB_COEF>=0){
        printf("totCoef: %f\n",totCoef);
        mod-=PROC_DB_COEF;
    }
    if(mod-PROC_DB_TN>=0){
        printf("totTerNot: %f\n",totTerNot);
        mod-=PROC_DB_TN;
    }
    //restituzione normale (in caso di nessun errore). Se PROC_ERR_RET fa parte della somma "mod", allora verra' restituito sempre 0 se l'equazione e' risolvibile
    //(tecnica usata per il controllo della risolvibilita')
    if(mod-PROC_ERR_RET>=0) return 0;   //stringa risolvibile
    //calcolo e restituzione del risultato della stringa di procedimento (calcolo dipende dalla definizione di coefficiente)
    if(modCoef==1) return totTerNot/totCoef;
    if(modCoef==2) return totCoef/totTerNot;
}

//
char* trova_var(char* proc){
	char* var=(char*)malloc(sizeof(char)*LEN);
	int jproc=0,jvar=0;
	while(conLet(proc[jproc])==0 || proc[jproc]=='\0') jproc++;
	if(proc[jproc]!='\0'){
		while(conLet(proc[jproc])==1 || proc[jproc]=='.'){
			var[jvar]=proc[jproc];
		    jproc++;jvar++;
		}
		var[jvar]='\0';
	}
	else{
		*var='\0'; //nome variabile non trovato
	}
	taglia(var);
	return var;
}

#endif
