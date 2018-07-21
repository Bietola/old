/*Nome Programma : 		Socrate
 *Programmatore  : 		Stefano Montesi
 *Utlimo Aggiornamento:	15/12/2013
 *Scopo:				Risolvere problemi di geometria o fisica basandosi su un sistema
 						di input "dati, incognite, proprietà e metodi". Più informazioni sono contenute nel
 						file "descrizione.txt".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Stringhe.h"

//struttura che definisce un "ente", oggetto dotato di varie proprietà, divise tra dati e incognite
//il puntatore "enti", inizializzato in intro(), e riallocato ogni volta che serve, contiene
//tutti gli enti creati dall'utente.
struct Ente{
	char* nome;
	int nDati;
	int nIncognite;
	char** nmDati;
	float* valDati;
	char** nmIncognite;
}*enti;


//variabili globali
char* nmEnteSel;							//NoMe Ente Selezionato (l'ente selezionato è quello a cui vengono aggiunti
											//dati e incognite
int nEnti=0;								//Numero Enti
char com;									//COMando (char di input contenente la scelta dell'utente)


//stampo dall'introduzione e inizializzazione della memoria
void intro(){
	printf("Benvenuto a \"Socrate\". Inserisci un qualsiasi comando (h per lista di comandi)\n");
	enti=(struct Ente*)malloc(sizeof(struct Ente));
	nmEnteSel=(char*)malloc(sizeof(char)*LEN);
	nmEnteSel[0]='\0';
}

//funzione per aggiungere un ente all'array dinamico "enti"
void aggiungi_ente(char* nome){
	//aggiunzione di uno spazio di memoria
	enti=(struct Ente*)realloc(enti,sizeof(struct Ente)*(nEnti+1));
	//riallocazione e assegnamento del nome dell'ente
	enti[nEnti].nome=(char*)malloc(sizeof(char)*(strlen(nome)+1));
	strcpy(enti[nEnti].nome,nome);
	//inizializzazione delle variabili nDati e nIncognite (0 per via dell'assenza di dati e incognite
	//alla creazione dell'ente
	enti[nEnti].nDati=0;
	enti[nEnti].nIncognite=0;
	//allocazione iniziale di nmDati, nmIncognite e valDati, per rendere possibile la reallocazione
	//in futuro
	enti[nEnti].nmDati=(char**)malloc(sizeof(char*));
	enti[nEnti].valDati=(float*)malloc(sizeof(float));
	enti[nEnti].nmIncognite=(char**)malloc(sizeof(char*));
	//aumento del contatore globale dedicato al numero di enti esistenti (per futuri usi della funzione)
	nEnti++;
}

//funzione per aggiungere un dato ad un determinato ente
int aggiungi_dato(char* nmEnte,char* nome,float valore){
	int j;
	//ricerca di un ente con il nome uguale a quello specificato nei parametri
	for(j=0;j<nEnti;j++){
		//ente trovato
		if(strcom(enti[j].nome,nmEnte)==1){
			//aggiunzione di uno spazio di memoria per contenere il nome del nuovo dato
			enti[j].nmDati=(char**)realloc(enti[j].nmDati,sizeof(char*)*(enti[j].nDati+1));
			enti[j].nmDati[enti[j].nDati]=(char*)malloc(sizeof(char)*strlen(nome));
			//aggiunzione del nome
			strcpy(enti[j].nmDati[enti[j].nDati],nome);
			//aggiunzione di uno spazio di memoria per contenere il valore del nuovo dato
			enti[j].valDati=(float*)realloc(enti[j].valDati,sizeof(float)*(enti[j].nDati+1));
			//aggiunzione del valore
			enti[j].valDati[enti[j].nDati]=valore;
			//aumento del contatore contenuto nell'ente dedicato al numero di dati
			enti[j].nDati++;
			return 0;		//ente trovato e dato creato con successo
		}
	}
	return 1;	//nome ente non trovato
}

//funzione per aggiungere un'incognita ad un determinato ente (procedimento sottointeso perchè simile
//a quello della funzione "aggiungi_dato()")
int aggiungi_incognita(char* nmEnte,char* nome){
	int j;
	for(j=0;j<nEnti;j++){
		if(strcom(enti[j].nome,nmEnte)==1){
			enti[j].nmIncognite=(char**)realloc(enti[j].nmIncognite,sizeof(char*)*(enti[j].nIncognite+1));
			enti[j].nmIncognite[enti[j].nIncognite]=(char*)malloc(sizeof(char)*strlen(nome));
			strcpy(enti[j].nmIncognite[enti[j].nIncognite],nome);
			enti[j].nIncognite++;
			return 0;	//nome ente trovato e incognita assegnata con successo
		}
	}
	return 1;	//nome ente non trovato
}

//funzione per aggiungere un nuovo metodo al file "metodi.txt"
void aggiungi_metodo(char* nome,char* proc){
	FILE* f=fopen("metodi.txt","a");
	fprintf(f,"\nmetodo:\n");
	fprintf(f,"\tnome: %s\n",nome);
	fprintf(f,"\tprocedimento:\n\t\t%s",proc);
	fclose(f);
}

//PRENDI TERmine NUMerico
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
                if(proc[pos]=='=') parte=2;
            }
            //altrimenti, memorizza il segno alla sinistra della variabile (si presume che ci sia sempre un segno prima di una variabile)
            else sSegno=proc[pos-1];
            //memorizza il nome della variabile (inutile per ora)
            int j=0;
            while(conLet(proc[pos])==1 || proc[pos]=='.'){
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

//CONtrolla RISolvibilita': cerca e attribuisce a ogni problema di risovlibilità che trova una potenza di 2; infine somma tutti i numeri e restituisce il risultato (WIP: non funziona correttamente)
//CONtrolla RISolvibilita': cerca e attribuisce a ogni problema di risovlibilità che trova una potenza di 2; infine somma tutti i numeri e restituisce il risultato
int conRis(char* proc){
    int err=0;
    //si noti che si usa lo stesso procedimento nella funzione "risolvi_proc()" ma senza il calcolo del termine numerico (che non serve per determinare eventuali errori)
    //serve comunque calcolare il coefficiente totale per determinare la indeterminazione dell'equazione o se essa supera il primo grado
    //serve anche leggiere la stringa come se la si stesse risolvendo, per determinare l'invalididità della stringa
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
                if(proc[pos]=='=') parte=2;
            }
            else sSegno=proc[pos-1];
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
                if(strcom(var,priVar)==0) err+=8;      //esiste piu' di una variabile
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
                    err+=4;     //equazione di secondo grado
                    break;
                }
                else if(sSegno=='/' && dSegno=='/'){
                    err+=4;     //equazione di secondo grado (da controllare)
                    break;
                }
            }
            else if(sSegno!=0){
                if(sSegno=='*'){
                    coef=sNum;
                }
                else if(sSegno='/'){
                    err+=4;     //equazione di secondo grado (da controllare)
                    break;
                }
            }
            else{
                coef=1;
            }
            if(parte==1) totCoef+=coef*segno;
            else if(parte==2) totCoef-=coef*segno;
        }
        else if(proc[pos]!='\0' && proc[pos]!='+' && proc[pos]!='-' && proc[pos]!='='){
            err+=1;       //stringa invalida
        }
        if(proc[pos]=='='){
            parte=2;
            pos++;
        }
    }while(proc[pos]!='\0');
    if(totCoef==0) err+=2;      //equazione indeterminata
    //restituzione
    return err;
}

//funzione per visualizzare a shermo l'ente attualmente selezionato + una lista di tutti
//gli enti creati con i rispettivi dati e le rispettive variabili assegnate
void visualizza(){
	int j;
	//ente selezionato
	printf("ente selezionato: ");
	if(nmEnteSel[0]=='\0'){
		printf("nessuno\n");
	}
	else{
		printf("%s\n",nmEnteSel);
	}
	//enti esistenti
	printf("enti esistenti: ");
	if(nEnti==0){
		printf("nessuno\n");
	}
	else{
		for(j=0;j<nEnti;j++){
			printf("\n-)%s",enti[j].nome);
			//dati assegnati
			if(enti[j].nDati==0){
				printf("\n\tnessun dato associato");
			}
			else{
				int k;
				printf("\n\tdati associati:");
				for(k=0;k<enti[j].nDati;k++){
					printf("\n\t-)%s: %f",enti[j].nmDati[k],enti[j].valDati[k]);
				}
			}
			//incognite assegnate
			if(enti[j].nIncognite==0){
				printf("\n\tnessuna incognita associata");
			}
			else{
				int k;
				printf("\n\tincognite associate:");
				for(k=0;k<enti[j].nIncognite;k++){
					printf("\n\t-)%s",enti[j].nmIncognite[k]);
				}
			}
		}
		printf("\n");
	}
}

//funzione che cerca di risolvere il problema proposto attraverso l'input fin'ora ricevuto
//dall'utente (sotto forma di dati e incognite associate ad enti)
void risolvi(){
	int jent,jdi;
	char inStr[LEN];
	char *nomeMet;
	char *proc;
	nomeMet=(char*)malloc(sizeof(char));
	proc=(char*)malloc(sizeof(char));
	FILE* f=fopen("metodi.txt","r");
	//ciclo che passa per ogni ente esistente
	for(jent=0;jent<nEnti;jent++){
		//ricerca di metodi nel file
		while(fscanf(f,"%s",inStr)==1){
			//metodo trovato
			if(strcom(inStr,"metodo:")==1){
				//riallocazione di stringhe temporanee
				nomeMet=(char*)realloc(nomeMet,sizeof(char)*LEN);
				proc=(char*)realloc(proc,sizeof(char)*LEN);
				//memorizzazione nome metodo
				fscanf(f,"%s",inStr);
				fscanf(f,"%s",nomeMet);
				taglia(nomeMet);
				//memorizzazione della stringa di procedimento del metodo (procedimento metodo)
				fscanf(f,"%s",inStr);
				fscanf(f,"%s",proc);
				taglia(proc);
				//sostituzione di eventuali incognite, appartenenti all'ente correntemente selezionato
				//nel ciclo, e inclusi nella stringa di procedimento
				for(jdi=0;jdi<enti[jent].nIncognite;jdi++){
					static char* nomeInc;
					nomeInc=stradd(enti[jent].nome,".");
					nomeInc=stradd(nomeInc,enti[jent].nmIncognite[jdi]);
				}
				//sostituzione dati
				for(jdi=0;jdi<enti[jent].nDati;jdi++){
					static char* nomeDat;
					nomeDat=stradd(enti[jent].nome,".");
					nomeDat=stradd(nomeDat,enti[jent].nmDati[jdi]);
					sost(proc,nomeDat,intToStr(enti[jent].valDati[jdi]));
				}
			}
			//stampo procedimento con sostituzioni (avviene per ogni ente)
			/*DEBUG*/printf("metodo %s: %s\n",nomeMet,proc);
			//risoluzione della stringa (se risolvibile)
			if(conRis(proc)==0){
				float valRis=risolvi_proc(proc);
				///char* nmRis=trova_nome_var(proc);
				//debug
                printf("risposta: %f\n",valRis);
				///aggiungi_risposta(nmRis,valRis);
				///aggiungi_dato(enti[jent],,ris);
			}

			//(WIP) determinazione della risolvibilità del procedimento proposto dal metodo
			//		in questione (dopo il processo di sostituzione)

			//(WIP) funzione per la risoluzione delle stringhe di procedimento
			//		-svolgimento delle 4 operazioni di base in ordine giusto
			//		-processi di ribaltamento formule
			//		-implementazione di parentesi '('
			//		-implementazione di condizioni 'se[triangolo.base=2] proc...'
			//		-interazioni tra diversi enti 'quadrato.area=triangolo.area*2'
		}
		//inizio del file
		fseek(f,0,SEEK_SET);
	}
	//(WIP) output risultato secondo metodi risolvibili
	//		-richiesta di aggiunzione di nuovo metodo appropriato se il risultato non è stato trovato
	//		-output risultati parziali
	fclose(f);
}

void menu(){
	if(com!='\n') printf(">");
	com=getchar();
	if(com=='h'){
		printf("Comandi disponibili:\n");
		printf("e: aggiungi nuovo ente\n");
		printf("d: aggiungi nuovo dato\n");
		printf("i: aggiungi nuova incognita\n");
		printf("m: aggiungi nuovo metodo\n");
		printf("s: seleziona ente\n");
		printf("v: visualizza enti, dati e incognite\n");
		printf("r: risolvi\n");
		printf("f: termina programma\n");
	}
	else if(com=='e'){
		char* nome;
		nome=(char*)malloc(sizeof(char)*LEN);
		printf("Nome ente: ");
		scanf("%s",nome);
		taglia(nome);
		aggiungi_ente(nome);
	}
	else if(com=='d'){
		char* nome;
		float val;
		nome=(char*)malloc(sizeof(char)*LEN);
		printf("Nome dato: ");
		scanf("%s",nome);
		printf("Valore dato: ");
		scanf("%f",&val);
		taglia(nome);
		aggiungi_dato(nmEnteSel,nome,val);
	}
	else if(com=='i'){
		char* nome;
		float val;
		nome=(char*)malloc(sizeof(char)*LEN);
		printf("Nome incognita: ");
		scanf("%s",nome);
		taglia(nome);
		aggiungi_incognita(nmEnteSel,nome);
	}
	else if(com=='m'){
		char *nome,*proc;
		nome=(char*)malloc(sizeof(char)*LEN);
		proc=(char*)malloc(sizeof(char)*LEN);
		printf("Nome metodo: ");
		scanf("%s",nome);
		taglia(nome);
		printf("Procedimento: ");
		scanf("%s",proc);
		taglia(proc);
		aggiungi_metodo(nome,proc);
	}
	else if(com=='s'){
		printf("Nome ente: ");
		scanf("%s",nmEnteSel);
		taglia(nmEnteSel);
	}
	else if(com=='v'){
		visualizza();
	}
	else if(com=='r'){
		risolvi();
	}
	else if(com!='\n'){
		printf("il comando \"%c\" non esiste\n",com);
	}
}

int main(){
	intro();
	do{
		menu();
	}while(com!='f');
	return 0;
}
