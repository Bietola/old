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
#include "Proc.h"

//struttura che definisce un "ente", oggetto dotato di varie proprietà, divise tra dati e incognite
//il puntatore "enti", inizializzato in intro(), e reallocato ogni volta che serve, contiene
//tutti gli enti creati dall'utente.
struct Ente{
	char*  nome;
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
//dall'utente (sottoforma di dati e incognite associate ad enti)
void risolvi(){
	int jent,jdi;
	char inStr[LEN];
	char *nome;
	char *proc;
	char *nomeDi;
	nome=(char*)malloc(sizeof(char));
	proc=(char*)malloc(sizeof(char));
	FILE* f=fopen("metodi.txt","r");
	//ciclo che passa per ogni ente esistente
	for(jent=0;jent<nEnti;jent++){
		//ricerca di metodi nel file
		while(fscanf(f,"%s",inStr)==1){
			//metodo trovato
			if(strcom(inStr,"metodo:")==1){
				//riallocazione di stringhe temporanee
				nome=(char*)realloc(nome,sizeof(char)*LEN);
				proc=(char*)realloc(proc,sizeof(char)*LEN);
				//memorizzazione nome metodo
				fscanf(f,"%s",inStr);
				fscanf(f,"%s",nome);
				taglia(nome);
				//memorizzazione della stringa di procedimento del metodo (procedimento metodo)
				fscanf(f,"%s",inStr);
				fscanf(f,"%s",proc);
				taglia(proc);
				//sostituzione di eventuali incognite, appartenenti all'ente correntemente selezionato
				//nel ciclo, e inclusi nella stringa di procedimento
				for(jdi=0;jdi<enti[jent].nIncognite;jdi++){
					nomeDi=stradd(enti[jent].nome,".");
					nomeDi=stradd(nomeDi,enti[jent].nmIncognite[jdi]);
					sost(proc,nomeDi,enti[jent].nmIncognite[jdi]);
				}
				//sostituzione dati
				for(jdi=0;jdi<enti[jent].nDati;jdi++){
					nomeDi=stradd(enti[jent].nome,".");
					nomeDi=stradd(nomeDi,enti[jent].nmDati[jdi]);
					sost(proc,nomeDi,numToStr(enti[jent].valDati[jdi]));
				}
			}
			//stampo procedimento con sostituzioni (avviene per ogni ente)
			/*DEBUG*/printf("metodo %s: %s\n",nome,proc);

			if(risolvi_proc(proc,PROC_ERR_RET)==0){
                printf("risolvibile\n");
				float valRis=risolvi_proc(proc,PROC_ERR_PRINT);
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
		printf("Nome incongnita: ");
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
