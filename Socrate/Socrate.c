/*Nome Programma : 		Socrate
 *Programmatore  : 		Stefano Montesi
 *Utlimo Aggiornamento:	22/4/2014 11:15
 *Scopo:				Risolvere problemi di geometria o fisica basandosi su un sistema
 						di input "dati, incognite, proprieta' e metodi". Più informazioni sono contenute nel
 						file "descrizione.txt".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Stringhe.h"
#include "Proc.h"


#define DEF_PRE_ERR 0.01
#define DEF_PRE_FLT 5


///STRUTTURE
//struttura che definisce un "ente", oggetto dotato di vari campi, divisi tra dati, incognite e proprieta'.
//Il puntatore "enti", inizializzato in intro(), e riallocato ogni volta che serve, contiene
//tutti gli enti creati dall'utente.
struct Ente{
	char*  nome;
	int nDati;
	int nIncognite;
	char** nmDati;
	float* valDati;
	char** nmIncognite;
}*enti;

//struttura che definisce una "risposta". Tutte le informazioni di tutte le risposte indicate dal puntatore "risposte"
//vengono restituite come output al termine dello svolgimento del problema.
struct Risposta{
	char* nome;
	char* nmEnte;
	float valore;
}*risposte;


///VARIABILI GLOBALI
float errPre=DEF_PRE_ERR;                   //passata come parametro di "errPre" alla funzione risolvi_proc(), la funzione della variabile e' spiegata in dettagli nella funzione
int fltPre=DEF_PRE_FLT;                     //passata come parametro di "fltPre" alla funzione risolvi_proc(), la funzione della variabile e' spiegata in dettagli nella funzione
char nmEnteSel[LEN];					    //NoMe Ente Selezionato (l'ente selezionato e' quello a cui vengono aggiunti dati, incognite e proprieta' nel sistema di input
int nEnti=0;								//Numero ENTI
int nRis=0;                                 //Numero RISposte
int nInc=0;                                 //Numero INCognite
char com;									//COMando (char di input contenente la scelta dell'utente)
char inStr[LEN];                            //INput STRing (stringa di input)


///DICHIARAZIONE FUNZIONI
void aggiungi_ente(char*);
int aggiungi_dato(int,char*,float);
int entAggiungi_dato(char*,char*,float);
int aggiungi_incognita(int,char*);
int entAggiungi_incognita(char*,char*);
int aggiungi_risposta(int,char*,float);
int entAggiungi_risposta(char*,char*,float);
int aggiungi_metodo(char*,char*);
void risolvi();
void visualizza();
void intro();
void menu();


///MAIN
int main(){
	intro();
	do{
		menu();
	}while(com!='f');
	return 0;
}


///FUNZIONI
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
int aggiungi_dato(int jent,char* nome,float valore){
	if(jent<0 || jent>=nEnti) return 1; //ente non esiste
	//aggiunzione di uno spazio di memoria per contenere il nome del nuovo dato
	enti[jent].nmDati=(char**)realloc(enti[jent].nmDati,sizeof(char*)*(enti[jent].nDati+1));
	enti[jent].nmDati[enti[jent].nDati]=(char*)malloc(sizeof(char)*(strlen(nome)+1));
	//aggiunzione del nome
	strcpy(enti[jent].nmDati[enti[jent].nDati],nome);
	//aggiunzione di uno spazio di memoria per contenere il valore del nuovo dato
	enti[jent].valDati=(float*)realloc(enti[jent].valDati,sizeof(float)*(enti[jent].nDati+1));
	//aggiunzione del valore
	enti[jent].valDati[enti[jent].nDati]=valore;
	//aumento del contatore contenuto nell'ente dedicato al numero di dati
	enti[jent].nDati++;
	return 0;		//ente trovato e dato creato con successo
}

//funzione alternativa per aggiungere un dato
int entAggiungi_dato(char* nmEnte,char* nome,float valore){
	int j;
	//ricerca di un ente con il nome uguale a quello specificato nei parametri
	for(j=0;j<nEnti;j++){
		//ente trovato
		if(strcmp(enti[j].nome,nmEnte)==0){
			aggiungi_dato(j,nome,valore);
			return 0;		//ente trovato e dato creato con successo
		}
	}
	return 1;	//nome ente non trovato
}

//funzioni per aggiungere un'incognita ad un determinato ente (procedimento sotto inteso perche' simile
//a quello della funzione "aggiungi_dato()")
int aggiungi_incognita(int jent,char *nome){
    if(jent<0 || jent>=nEnti) return 1; //ente non esiste
    //processo simile a quello usato in "aggiungi_dato()"
    enti[jent].nmIncognite=(char**)realloc(enti[jent].nmIncognite,sizeof(char*)*(enti[jent].nIncognite+1));
    enti[jent].nmIncognite[enti[jent].nIncognite]=(char*)malloc(sizeof(char)*(strlen(nome)+1));
    strcpy(enti[jent].nmIncognite[enti[jent].nIncognite],nome);
    enti[jent].nIncognite++;
    //aumento della variabile globale contenente il numero complessivo di incognite (usata dopo per capire se il problema e' stato risolto)
    nInc++;
    return 0;	//nome ente trovato e incognita assegnata con successo
}

//analogo a "nmAggiungi_dato()"
int entAggiungi_incognita(char* nmEnte,char* nome){
	int j;
	for(j=0;j<nEnti;j++){
		if(strcmp(enti[j].nome,nmEnte)==0){
			aggiungi_incognita(j,nome);
			return 0;		//ente trovato e incognita creata con successo
		}
	}
	return 1;	//nome ente non trovato
}

//funzione per aggiungere una risposta all'array "risposte". La funzione cerca anche di rimuovere la risposta dalle incognite
//dell'ente interessato, e di aggiungerla ai suoi dati.
int aggiungi_risposta(int jent,char* nome,float valore){
    if(jent<0 || jent>=nEnti) return 2; //ente non esiste
    int j; //contatore
    //controllo dell'esistenza del dato nell'ente
    for(j=0;j<enti[jent].nDati;j++){
        //dato trovato
        if(strcmp(enti[jent].nmDati[j],nome)==0){
            return 0; //il dato esiste gia'
        }
    }
    //il dato non esiste ancora; aggiunta dato
    aggiungi_dato(jent,nome,valore);
    //rimozione incognita
    for(j=0;j<enti[jent].nIncognite;j++){
        //incognita trovata
        if(strcmp(enti[jent].nmIncognite[j],nome)==0){
            //rimozione incognita
            enti[jent].nmIncognite[j]=NULL;
            //visto che esisteva un incognita con lo stesso nome della risposta, allora si aggiunge la risposta all'array "risposte" per poi
            //darla come input all'utente (visto che l'obbiettivo del programma e' quello di trovare un valore a tutte le incognite)
            risposte=(struct Risposta*)realloc(risposte,sizeof(struct Risposta)*(nRis+1));
            risposte[nRis].nome=nome;
            risposte[nRis].nmEnte=enti[jent].nome;
            risposte[nRis].valore=valore;
            //aumento della variabile globale contenente il numero complessivo di risposte (usato per determinare se il problema e' stato risolto)
            nRis++;
        }
    }
    return 1; //il dato non esisteva
}

//funzione alternativa (fa lo stesso di quella sopra, restituisce 2 se non esiste l'ente)
int entAggiungi_risposta(char* nmEnte,char* nome,float valore){
	int j=0; //contatore
	//ricerca dell'ente
	for(j=0;j<nEnti;j++){
        //ente trovato
		if(strcmp(enti[j].nome,nmEnte)==0){
		    //aggiunta risposta
		    return aggiungi_risposta(j,nome,valore);
		}
	}
	return 2; //l'ente specificato non esiste
}

//funzione per aggiungere un nuovo metodo al file "metodi.txt"
int aggiungi_metodo(char* nome,char* proc){
	FILE* f=fopen("metodi.txt","a");
	if(f==NULL) return 1; //file non esiste
	fprintf(f,"\nmetodo:\n");
	fprintf(f,"\tnome: %s\n",nome);
	fprintf(f,"\tprocedimento:\n\t\t%s",proc);
	fclose(f);
	return 0; //file esiste
}

//funzione che cerca di risolvere il problema proposto attraverso l'input fin'ora ricevuto
//dall'utente (sotto forma di dati e incognite associate ad enti)
void risolvi(){
	int chk;                                //CHecK, utilizzo spiegato piu' avanti
	int jent,jdip,jris;                     //contatori (rispettivamente: ENTe, DatoIncognitaProprieta',RISposta)
	char trash[LEN];                        //stringa usata per saltare parole inutili, di scopo puramente descrittivo, nel file "metodi.txt". E' anche impiegata nel riconoscimento dei metodi nel file.
	char nmMet[LEN],proc[LEN],memProc[LEN]; //stringhe usate per contenere, rispettivamente, il nome di un metodo, il procedimento di un metodo (soggetto a modifiche) e sempre il p.m. (che rimarra' pero' uguale)
	char nmDatIncProp[LEN];                 //stringa per contenere un nome di un dato, di un incognita o di una proprieta' (le proprieta' non esistono ancora)
	FILE* f=fopen("metodi.txt","r");
	//loop principale (basato sul controllo di "chk")
	do{
        //chk viene messo uguale a zero ogni volta che il ciclo ricomincia.
        //In caso di "inattivita'" (se non viene trovato nessun metodo risolvibile)
        //rimarra' uguale a zero e il ciclo terminera'
		chk=0;
        //ciclo che analizza ogni stringa nel file
		while(fscanf(f,"%s",trash)==1){
			//metodo trovato
			if(strcmp(trash,"metodo:")==0){
				//memorizzazione nome metodo
				fscanf(f,"%s",trash);
				fscanf(f,"%s",nmMet);
				//memorizzazione della stringa di procedimento del metodo (procedimento metodo)
				fscanf(f,"%s",trash);
				fscanf(f,"%s",proc);
				//memorizzazione del procedimento completo per l'output
				strcpy(memProc,proc);
				//ciclo che passa per ogni ente esistente
				for(jent=0;jent<nEnti;jent++){
					//sostituzione dati
					for(jdip=0;jdip<enti[jent].nDati;jdip++){
                        //formazione del nome completo del dato (con il riferimento all'ente)
                        strcpy(nmDatIncProp,enti[jent].nome);
                        strcat(nmDatIncProp,".");
                        strcat(nmDatIncProp,enti[jent].nmDati[jdip]);
                        //sostituzione del nome completo nella stringa di procedimento del metodo
						sost(proc,nmDatIncProp,fltToStr(enti[jent].valDati[jdip],fltPre));
					}
					//!(WIP) sostituzione proprieta'
				}
				//!(DEBUG) stampo procedimento con sostituzioni - printf("metodo %s: %s\n",nome,proc);
				//controllo risolvibilita' del procedimento del metodo dopo la sostituzione
				int err=risolvi_proc(proc,errPre,PROC_ERR_RET);
				//stringa risolvibile
				if(err==0){
	                //ricavamento del risultato
					float valRis=risolvi_proc(proc,errPre,0);
					char* nmEntRis=trova_var(proc);
					//stampo procedimento
	                printf("\ntrovato metodo risolvibile: %s\n%s\n%s\n%s=%f\n",nmMet,memProc,proc,nmEntRis,valRis);
	                //divisione del nome ricavato dal procedimento (composto dall'ente+un punto+la risposta) nel nome dell'ente e quello della risposta
	                int j=0,k=0;                 //contatori
	                char nmEnt[LEN],nmRis[LEN]; //stringhe per contenere i nomi, rispettivamente, dell'ente e della risoluzione
                    //ricavamento nome ente
                    while(nmEntRis[j]!='.'){
                        nmEnt[j]=nmEntRis[j];
                        j++;
                    }
                    nmEnt[j]='\0';
                    //ricavamento nome risposta (il terminatore e' gia' aggiunto nel loop)
                    while(nmEntRis[j]!='\0'){
                        j++;
                        nmRis[k]=nmEntRis[j];
                        k++;
                    }
                    //aggiunta della risposta all'array dinamico "risposte" e rimozioni di eventuali incognite, e aggiunta della risposta come nuovo dato, all'ente in questione
                    //(guarda la funzione "aggiungi_risposta()" + aggiornamento di "chk" (se e' stata trovata anche solo una risposta, il loop sara' ripetuto nel caso che i nuovi dati
                    //possano rendere metodi precedentemente irrisolvibili, risolvibili.
					if(chk==0) chk=entAggiungi_risposta(nmEnt,nmRis,valRis);
					else entAggiungi_risposta(nmEnt,nmRis,valRis);
				}
				//se viene trovato un metodo che propone una stringa di procedimento impossibile (ERR_IMP), o una divisione per zero (ERR_DPZ), viene segnalato all'utente.
				else if(err==ERR_IMP || err==ERR_DPZ){
					//avvertimento del riscontramento di una equazione impossibile (cosa che potrebbe provenire da dei dati o dei metodi incorretti)
					printf("\ntrovato metodo impossibile: %s\n%s\n%s\nil metodo, o i dati compresi in esso, potrebbero essere incorretti\n",nmMet,memProc,proc);
				}
			}
		}
		//inizio del file
		fseek(f,0,SEEK_SET);
	}while(chk==1);
	//output risultato secondo metodi risolvibili (il risultato potrebbe essere parziale)
	if(nRis==nInc) printf("\nproblema risolto");
	else printf("\nproblema non risolto");
	printf("\nincognite trovate: ");
	if(nRis==0) printf("nessuna");
	for(jris=0;jris<nRis;jris++){
		printf("\n%s.%s=%f",risposte[jris].nmEnte,risposte[jris].nome,risposte[jris].valore);
	}
	printf("\n\n");
	//chiusura file
	fclose(f);
	//terminazione della funzione "menu()"
	com='f';
}

//funzione per visualizzare a schermo l'ente attualmente selezionato piu' una lista di tutti
//gli enti creati, con i rispettivi dati e le rispettive variabili assegnate
void visualizza(){
	int j; //contatore
	//precisioni
	printf("precisione decimale: %d\nprecisione di errore: %f\n",fltPre,errPre);
	//ente selezionato
	printf("ente selezionato: ");
	//se l'ente non e' stato ancora selezionato, secondo la funzione "intro()" =, la stringa dedicata al compito dovrebbe avere un terminatore come primo carattere
	if(*nmEnteSel=='\0'){
		printf("nessuno\n");
	}
	else{
		printf("%s\n",nmEnteSel);
	}
	//stampo enti esistenti
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

//stampo dell'introduzione, inizializzazione della memoria e altro
void intro(){
	printf("Benvenuto a \"Socrate\". Inserisci un qualsiasi comando (h per lista di comandi)\n");
	enti=(struct Ente*)malloc(sizeof(struct Ente));
	risposte=(struct Risposta*)malloc(sizeof(struct Risposta));
	*nmEnteSel='\0';
}

//funzione principale del programma, dedicata all'input.
void menu(){
	if(com!='\n') printf(">");
	scanf("%c",&com);
	if(com=='h'){
		printf("Comandi disponibili:\n");
		printf("e: aggiungi ente\n");
		printf("s: seleziona ente\n");
		printf("d: aggiungi dato a ente selezionato\n");
		printf("i: aggiungi incognita a ente selezionato\n");
		printf("m: aggiungi metodo al file \"metodi.txt\"\n");
		printf("v: visualizza enti, dati e incognite\n");
		printf("r: risolvi\n");
		printf("F: cambia precisione decimale\n");
		printf("E: cambia precisione di errore\n");
		printf("f: termina programma\n");
	}
	else if(com=='e'){
		char nome[LEN];
		printf("Nome ente: ");
		scanf("%s",nome);
		aggiungi_ente(nome);
	}
	else if(com=='d'){
        if(*nmEnteSel=='\0') printf("ente non ancora selezionato\n");
        else{
            char nome[LEN];
            float val;
            printf("Nome dato: ");
            scanf("%s",nome);
            printf("Valore dato: ");
            scanf("%f",&val);
            entAggiungi_dato(nmEnteSel,nome,val);
        }
	}
	else if(com=='i'){
        if(*nmEnteSel=='\0') printf("ente non ancora selezionato\n");
        else{
            char nome[LEN];
            printf("Nome incongnita: ");
            scanf("%s",nome);
            entAggiungi_incognita(nmEnteSel,nome);
        }
	}
	else if(com=='m'){
		char nome[LEN],proc[LEN];
		printf("Nome metodo: ");
		scanf("%s",nome);
		printf("Procedimento: ");
		scanf("%s",proc);
		if(aggiungi_metodo(nome,proc)==1) printf("creazione metodo fallita; il file \"metodi.txt\" non esiste\n");
	}
	else if(com=='s'){
        int j,chk=0;
		printf("Nome ente: ");
		scanf("%s",nmEnteSel);
		for(j=0;j<nEnti;j++){
            if(strcmp(enti[j].nome,nmEnteSel)==0) chk=1;
		}
		if(chk==0){
            printf("non esiste un ente chiamato \"%s\"\n",nmEnteSel);
            *nmEnteSel='\0';
		}
	}
	else if(com=='v'){
		visualizza();
	}
	else if(com=='r'){
		risolvi();
	}
	else if(com=='F'){
        printf("Nuova precisione decimale: ");
        scanf(" %d",&fltPre);
	}
	else if(com=='E'){
        printf("Nuova precisione di errore: ");
        scanf(" %f",&errPre);
	}
	else if(com!='\n' && com!='f'){
		printf("il comando \"%c\" non esiste\n",com);
	}
}
