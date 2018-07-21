#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//constants
#define MAX_SP 10
#define ASCII_START 97
#define MAX_ROW_NAMES 5
#define PERCENT 100

//key constants
#define KEY_YES 'y'

// hack
#define getch() std::cin.get()

//enumerations
enum err_t{
	ERR_NULL=1,
	OUT_OF_RANGE,
	SELF_IMPALEMENT,
	ACTION_ON_DEAD,
	ACTION_ON_MURDERED,
	SELF_STAKE,
	ACTION_ON_STAKE,
	STAKE_ON_NOT_IMPALED,
	SELF_MAYOR_ELECT,
	SELF_WOLF_KILL,
	WOLF_KILL_WOLF,
	REVIVE_ON_LIVING,
	SELF_DIVINE,
	SELF_PROTECT,
	GUARD_ACTION_ON_GUARD,
	SELF_BITE,
	SELF_EXPLODE
};

enum status_t{
	ALIVE=1,
	IMPALED,
	SAVABLE,
	STAKE,
    MURDERED,
	DEAD,
};

enum contr_t{
	PLAYER=1,
	CPU,
};

enum role_t{
	NARRATORE=1,
	CASO,
	CONTADINO,
	GUARDIA,
	DRUIDO,
	VEGGENTE,
	GUARITORE,
	MERETRICE,
	PRETE,
	VAMPIRO,
	FIGLIO,
	LUPO,
	KAMIKAZE
};

enum special_t{
	SINDACO=1,
	PRESIDENTE,
	PROTETTO,
	DISSACRATO,
	DECANO,
};

enum weekday_t{
    MONDAY=1,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY,
};

//global functions for arrays
void arrReset(int*,int,int);
void arrReset(special_t*,special_t);
bool arrFind(int,int*,int,int);
void arrCopy(int*,int*,int);
void arrCopy(special_t*,special_t*);

//classes
class Player{
	private:
		//private variables
		int ID;
		status_t status;
		contr_t contr;
		role_t role;
		special_t special[MAX_SP];
	public:
		//static variables
		static int numPl;
		static int numWf;
		static bool mayor;
		static bool dean;
		//public variables
		string name;
		int votes;
		//constructors
		Player(){
			ID=numPl;
			numPl++;
			votes=0;
			name="Player";
			contr=CPU;
			status=ALIVE;
			role=CONTADINO;
			arrReset(special,(special_t)NULL);
		}
		Player(string nam,contr_t ctrl,role_t rl){
			ID=numPl;
			numPl++;
			votes=0;
			name=nam;
			contr=ctrl;
			role=rl;
			status=ALIVE;
			arrReset(special,(special_t)NULL);
		}
		Player(string nam,contr_t ctrl,role_t rl,special_t sp[MAX_SP]){
			ID=numPl;
			numPl++;
			votes=0;
			name=nam;
			contr=ctrl;
			role=rl;
			status=ALIVE;
			arrCopy(sp,special);
		}
		//get functions
		int getID(){
			return ID;
		}
		contr_t getContr(){
			return contr;
		}
		role_t getRole(){
			return role;
		}
		status_t getStatus(){
			return status;
		}
		bool isSpecial(special_t sp){
			for(int j=0;j<MAX_SP;j++){
				if(special[j]==sp) return true;
			}
			return false;
		}
		//set functions
		void setContr(contr_t ctrl){
			contr=ctrl;
		}
		void setRole(role_t rl){
			role=rl;
		}
		void setStatus(status_t st){
			status=st;
		}
		void addSpecialEle(special_t sp){
		    for(int j=0;j<MAX_SP;j++){
                if(special[j]==(special_t)NULL){
                    special[j]=sp;
                    break;
                }
		    }
		}
		void removeSpecialEle(special_t sp){
            for(int j=0;j<MAX_SP;j++){
                if(special[j]==sp) special[j]=(special_t)NULL;
            }
		}
		void setSpecialEle(int ele,special_t sp){
			special[ele]=sp;
		}
		void setSpecialArr(special_t *sp){
			arrCopy(sp,special);
		}
		special_t getSpecialEle(int ele){
            return special[ele];
		}
		//other functions
		bool isAlive(){
			if(status!=DEAD && status!=MURDERED) return true;
			return false;
		}
		void kill(){
			status=DEAD;
			numPl--;
			if(this->isSpecial(SINDACO)) mayor=false;
			if(this->isSpecial(DECANO)) dean=false;
			if(this->getRole()==LUPO) numWf--;
		}
		void revive(){
			status=ALIVE;
			numPl++;
			if(this->isSpecial(SINDACO)) mayor=true;
			if(this->isSpecial(DECANO)) dean=true;
			if(this->getRole()==LUPO) numWf++;
		}
		void change(string nam,contr_t ctrl,role_t rl){
			name=nam;
			contr=ctrl;
			role=rl;
		}
};

struct RandRoleNum{
	role_t role;
	int num;
};

//initialization of static class variables
int Player::numPl=0;
int Player::numWf=0;
bool Player::mayor=false;
bool Player::dean=false;

//global variables declarations
string village_name="";
Player *player;
RandRoleNum *rrns;
int date=0,maxPl=0;
//modes
bool modNarratore=false;
bool modColors=false;

//global functions prototypes and declarations
weekday_t getWeekDay(int date);
int toType(string);
string toString(string,int);
void printDivider();
bool checkError(bool,int,int,...);
void startGame();
void electDean();
void printPlayers(role_t,bool,char);
void printPlayers(Player,bool,char);
void getVotes(status_t);
int* orderPlayers();
int impalePlayers(bool);
int stakePlayers(bool);
void savePlayers(bool);
void electMayor(bool);
void day();
void night();
int checkEnd();
void endGame(int end);

//main
int main(){
    int end;
	startGame();
	do{
		day();
		end=checkEnd();
		if(end!=0) break;
		night();
        end=checkEnd();
	}while(end==0);
	endGame(end);
	getch();
	return 0;
}

//global functions
void arrReset(int *arr,int max,int resetValue){
	for(int j=0;j<max;j++){
		arr[j]=resetValue;
	}
}
void arrReset(special_t* arr,special_t resetValue){
	for(int j=0;j<MAX_SP;j++){
		arr[j]=resetValue;
	}
}
bool arrFind(int num,int *arr,int min,int max){
	for(int j=min;j<=max;j++){
		if(arr[j]==num) return true;
	}
	return false;
}
void arrCopy(int *from,int *to,int max){
	for(int j=0;j<max;j++){
		to[j]=from[j];
	}
}
void arrCopy(special_t *from,special_t *to){
	for(int j=0;j<MAX_SP;j++){
		to[j]=from[j];
	}
}

weekday_t getWeekDay(int date){
    return (weekday_t)(date%7);
}

int toType(string str){
	//contr_t
	if(str=="PLAYER") return PLAYER;
	else if(str=="CPU") return CPU;
	//role_t
	else if(str=="NARRATORE") return NARRATORE;
	else if(str=="CASO") return CASO;
	else if(str=="CONTADINO") return CONTADINO;
	else if(str=="GUARDIA") return GUARDIA;
	else if(str=="DRUIDO") return DRUIDO;
	else if(str=="VEGGENTE") return VEGGENTE;
	else if(str=="GUARITORE") return GUARITORE;
	else if(str=="MERETRICE") return MERETRICE;
	else if(str=="PRETE") return PRETE;
	else if(str=="VAMPIRO") return VAMPIRO;
	else if(str=="FIGLIO_DEI_LUPI" || str=="FIGLIO") return FIGLIO;
	else if(str=="LUPO") return LUPO;
	else if(str=="KAMIKAZE") return KAMIKAZE;
	//special_t
	else if(str=="SINDACO") return SINDACO;
	else if(str=="DECANO" || str=="VECCHIACCIA") return DECANO;
	else if(str=="PRESIDENTE") return PRESIDENTE;
	else if(str=="PROTETTO") return PROTETTO;
	else if(str=="DISSACRATO") return DISSACRATO;
	//weekday_t
	//!WIP not needed
	else return NULL;
}

bool isType(int type){
	//contr_t
	if(type==PLAYER) return true;
	else if(type==CPU) return true;
	//role_t
	else if(type==NARRATORE) return true;
	else if(type==CASO) return true;
	else if(type==CONTADINO) return true;
	else if(type==GUARDIA) return true;
	else if(type==DRUIDO) return true;
	else if(type==VEGGENTE) return true;
	else if(type==GUARITORE) return true;
	else if(type==MERETRICE) return true;
	else if(type==PRETE) return true;
	else if(type==VAMPIRO) return true;
	else if(type==FIGLIO) return true;
	else if(type==LUPO) return true;
	else if(type==KAMIKAZE) return true;
	//special_t
	else if(type==SINDACO) return true;
	else if(type==DECANO) return true;
	else if(type==PRESIDENTE) return true;
	else if(type==PROTETTO) return true;
	else if(type==DISSACRATO) return true;
	//weekday_t
	/*else if(type==MONDAY) return true;
	else if(type==TUESDAY) return true;
	else if(type==WEDNESDAY) return true;
	else if(type==THURSDAY) return true;
	else if(type==FRIDAY) return true;
	else if(type==SATURDAY) return true;
	else if(type==SUNDAY) return true;*/
	else return false;
}

string toString(string type,int n){
	//contr_t
	if(type=="contr"){
		if(n==PLAYER) return "PLAYER";
		else if(n==CPU) return "CPU";
	}
	//role_t
	else if(type=="role"){
		if(n==NARRATORE) return "NARRATORE";
		else if(n==CONTADINO) return "CONTADINO";
		else if(n==GUARDIA) return "GUARDIA";
		else if(n==DRUIDO) return "DRUIDO";
		else if(n==VEGGENTE) return "VEGGENTE";
		else if(n==GUARITORE) return "GUARITORE";
		else if(n==MERETRICE) return "MERETRICE";
		else if(n==PRETE) return "PRETE";
		else if(n==VAMPIRO) return "VAMPIRO";
		else if(n==FIGLIO) return "FIGLIO DEI LUPI";
		else if(n==LUPO) return "LUPO";
		else if(n==KAMIKAZE) return "KAMIKAZE";
	}
	//special_t
	else if(type=="special"){
		if(n==SINDACO) return "SINDACO";
		if(n==DECANO) return "DECANO";
		if(n==PRESIDENTE) return "PRESIDENTE";
		if(n==PROTETTO) return "PROTETTO";
		if(n==DISSACRATO) return "DISSACRATO";
	}
	//weekday_t
	else if(type=="week" || type=="weekday" || type=="weekdays"){
        if(n==MONDAY) return "Monday";
        else if(n==TUESDAY) return "Tuesday";
        else if(n==WEDNESDAY) return "Wednesday";
        else if(n==THURSDAY) return "Thursday";
        else if(n==FRIDAY) return "Friday";
        else if(n==SATURDAY) return "Saturday";
        else if(n==SUNDAY) return "Sunday";
	}
	//else
	else return NULL;
}

void printDivider(){
	const char gra='-';
	const int len=75;
	for(int j=0;j<len;j++){
		cout<<gra;
	}
	cout<<"\n";
}

bool checkError(bool print,int id,int n,...){
	bool err=false;
	int intErrChk;
	err_t errChk;
	va_list args;
	va_start(args, n);
	do{
   		intErrChk=va_arg(args,int);
   		errChk=(err_t)intErrChk;
   		if(errChk==OUT_OF_RANGE){
			if(n<0 || n>=maxPl){
				err=true;
				if(print){
					printDivider();
					cout<<"Choice must be higher than '"<<(char)ASCII_START<<"' and lower than '"<<(char)(ASCII_START+maxPl-1)<<"'.\n";
				}
			}
		}
		else if(errChk==SELF_IMPALEMENT){
			if(n==id){
				err=true;
				if(print){
					printDivider();
					cout<<"You can't send yourself to the impalement!\n";
				}
			}
		}
		else if(errChk==ACTION_ON_DEAD){
			if(player[n].getStatus()==DEAD){
				err=true;
				if(print){
					printDivider();
					cout<<player[n].name<<" is already dead!\n";
				}
			}
		}
		else if(errChk==ACTION_ON_MURDERED){
            if(player[n].getStatus()==MURDERED){
                if(player[n].getStatus()==MURDERED){
                    err=true;
                    if(print){
                        printDivider();
                        cout<<player[n].name<<" has already been eaten.\n";
                    }
                }
            }
		}
		else if(errChk==SELF_STAKE){
			if(n==id){
				err=true;
				if(print){
					printDivider();
					cout<<"You can't send yourself to the stake!\n";
				}
			}
		}
		else if(errChk==ACTION_ON_STAKE){
            if(player[n].getStatus()==STAKE){
                err=true;
                if(print){
                    printDivider(),
                    cout<<player[n].name<<" will already be burned. You must choose a citizen which has not been doomed yet.\n";
                }
            }
		}
		else if(errChk==SELF_MAYOR_ELECT){
            if(n==id){
                err=true;
                if(print){
                    printDivider();
                    cout<<"You can't vote for yourself.\n";
                }
            }
		}
		else if(errChk==STAKE_ON_NOT_IMPALED){
			if(player[n].getStatus()!=IMPALED){
				err=true;
				if(print){
					printDivider();
					cout<<player[n].name<<" is not impaled, you can only choose an impaled citizen.\n";
				}
			}
		}
		else if(errChk==SELF_WOLF_KILL){
			if(n==id){
				err=true;
				if(print){
					printDivider();
					cout<<"You can't kill yourself!\n";
				}
			}
		}
		else if(errChk==WOLF_KILL_WOLF){
			if(player[n].getRole()==LUPO){
				err=true;
				if(print){
					printDivider();
					cout<<player[n].name<<" is a warewolf, like you!\n";
				}
			}
		}
		else if(errChk==REVIVE_ON_LIVING){
            if(player[n].getStatus()==ALIVE){
                err=true;
                if(print){
                    printDivider();
                    cout<<player[n].name<<" is already alive.\n";
                }
            }
		}
		else if(errChk==SELF_DIVINE){
            if(n==id){
                err=true;
                if(print){
                    printDivider();
                    cout<<"I strictly believe that you're already aware of your soul's conditions...\n";
                }
            }
		}
		else if(errChk==SELF_PROTECT){
            if(n==id){
                err=true;
                if(print){
                    printDivider();
                    cout<<"You can't protect yourself, you selfish bastard...\n";
                }
            }
		}
		else if(errChk==GUARD_ACTION_ON_GUARD){
            if(player[id].getRole()==GUARDIA && player[n].getRole()==GUARDIA){
                err=true;
                if(print){
                    printDivider();
                    cout<<player[n].name<<" is a guard, like you!\n";
                }
            }
		}
		else if(errChk==SELF_BITE){
            if(id==n){
                err=true;
                if(print){
                    printDivider();
                    cout<<"If you bite yourself, you won't get any less thirsty, genius...\n";
                }
            }
		}
		else if(errChk==SELF_EXPLODE){
            if(id==n){
                err=true;
                if(print){
                    printDivider();
                    cout<<"You can't waste your life without being of any service to Allah!\n";
                }
            }
		}
	}while(errChk!=ERR_NULL);
	va_end(args);
	return err;
}

void startGame(){
	//READ FROM CONGIF FILE//
	//enums
	enum readMode_t{
		FRM,
		MOD,
		RRN,
		PL,
		CN,
		E,
	};
	//variables
	readMode_t readMode=FRM;
	string inStr=" ";
	int j;
	int maxRrcs=0,maxRrns=0; //maxRandomRoleContainerS and maxRandomRoleNumeberS
	bool areCounted=false;
	//initialization of random seed
	srand(time(NULL));
	//input players trhough file
	ifstream playerList("config.txt",ios::binary | ios::in);
	do{
		//input (method depends on read mode)
		if(readMode==FRM){
			//find readmode sign
			if(inStr!=":"){
				do{
					playerList>>inStr;
					//DEBUG cout<<inStr<<endl;
				}while(inStr!=":");
			}
			//find readmode type
			playerList>>inStr;
			//DEBUG cout<<"READMODE: "<<inStr<<endl;
			//DEBUG getch();
			if(inStr=="MOD") readMode=MOD;
			else if(inStr=="CN") readMode=CN;
			else if(inStr=="RRN") readMode=RRN;
			else if(inStr=="PL") readMode=PL;
			else if(inStr=="END") readMode=E;
			else{
				cout<<"ERROR: Incapable of Finding Correct ReadMode Type After the ReadMode Sign ("<<inStr<<")\n";
				///end game (WIP)
				;
			}
			//skip until readmode sign
			do{
				playerList>>inStr;
			}while(inStr!=":");
		}
		else if(readMode==MOD){
            //activate listed modes
			do{
				playerList>>inStr;
				if(inStr=="NARRATORE") modNarratore=true;
				else if(inStr=="COLORS") modColors=true;
			}while(inStr!=":");
			readMode=FRM;
		}
		else if(readMode==CN){
			for(int j=0;;j++){
				playerList>>inStr;
				if(inStr==":") break;
				if(j!=0) village_name+=" ";
				village_name+=inStr;
			}
			readMode=FRM;
		}
		else if(readMode==RRN){
			//count random role numbers
			int len=playerList.tellg();
			//*DEBUG*/ cout<<"len: "<<len<<endl;
			do{
				playerList>>inStr;
				//*DEBUG*/ cout<<inStr<<endl;
				if(isType(toType(inStr))) maxRrcs++;
			}while(inStr!=":");
			//put the rrn(s) into the rrns array
			playerList.seekg(len);
			rrns=new RandRoleNum[maxRrcs];
			for(j=0,maxRrns=0;j<maxRrcs;j++){
				playerList>>inStr;
				//*DEBUG*/cout<<"role: "<<inStr<<endl;
				//*DEBUG*/getch();
				rrns[j].role=(role_t)toType(inStr);
				playerList>>inStr;
				//*DEBUG*/cout<<"num: "<<inStr<<endl;
				//*DEBUG*/getch();
				rrns[j].num=atoi(inStr.c_str());
				maxRrns+=rrns[j].num;
				//*DEBUG*/ cout<<j<<") "<<toString("role",rrns[j].role)<<" - "<<rrns[j].num<<endl;
				//*DEBUG*/ getch();
			}
			readMode=FRM;
		}
		else if(readMode==PL){
			//count players
			int len=playerList.tellg();
			do{
				do{
					playerList>>inStr;
					if(inStr=="DECANO" || inStr=="VECCHIACCIA") Player::dean=true;
					if(inStr=="SINDACO") Player::mayor=true;
					if(inStr=="LUPO") Player::numWf++;
					//DEBUG cout<<inStr<<endl;
					//DEBUG getch();
				}while(isType(toType(inStr)));
				if(inStr!=":") maxPl++;
				//DEBUG cout<<"maxPL: "<<maxPl<<endl;
			}while(inStr!=":");
			player=new Player[maxPl];
			playerList.seekg (len);
			//put players into the player array
			for(int j=0;j<maxPl;j++){
				if(isType(toType(inStr)) || inStr==":") playerList>>inStr;
				player[j].name=inStr;
				//*DEBUG*/ cout<<"Name: "<<inStr<<endl;
				//*DEBUG*/ getch();
				playerList>>inStr;
				player[j].setContr((contr_t)toType(inStr));
				playerList>>inStr;
				if(inStr=="CASO"){
					if(maxRrns>0){
						int r;
						do{
							r=rand()%maxRrcs;
							//*DEBUG*/ cout<<r<<" - "<<rrns[r].role<<endl;
							//*DEBUG*/ getch();
						}while(rrns[r].num<=0);
						rrns[r].num--;
						maxRrns--;
						player[j].setRole(rrns[r].role);
						//increment numWf if role is wolf
						if(rrns[r].role==LUPO) Player::numWf++;
					}
					else{
						cout<<"ERROR: not enough randomRoleNumberS to distribute\n"<<endl;
						//end game (WIP)
						player[j].setRole(CONTADINO);
					}
				}
				else player[j].setRole((role_t)toType(inStr));
				//special
				do{
					static int k=0;
					playerList>>inStr;
					if(isType(toType(inStr))){
						player[j].setSpecialEle(k,(special_t)toType(inStr));
						k++;
						//DEBUG/ cout<<j<<" Special "<<k<<": "<<inStr<<endl;
						//DEBUG/ getch();
					}
				}while(isType(toType(inStr)));
			}
			readMode=FRM;
		}
	}while(readMode!=E);
}

void electDean(){
    int r;
    Player::dean=true;
    do{
        r=rand()%maxPl;
    }while(!player[r].isAlive());
    player[r].addSpecialEle(DECANO);
    printDivider();
    cout<<player[r].name<<" has been chosen as the new dean of "<<village_name<<".\n";
    getch();
}

int checkEnd(){
    int players=0,wolves=0;
    for(int j=0;j<maxPl;j++){
        if(player[j].isAlive()){
            if(player[j].getRole()==LUPO) wolves++;
            else players++;
        }
    }
    if(wolves<=0) return 1; //citizens win
    if(players-wolves<=0) return 2; //wolves win
    return 0;
}

void endGame(int end){
	system("cls");
	cout<<"The end :D\n";
	if(end==2) cout<<"The werewolves conquered "<<village_name<<"!\n";
	else{
        cout<<"The wolves were defeated by the power of democracy!\n"<<village_name<<" is finally free!\n";
        printDivider();
        cout<<"The survivors:\n";
        for(int j=0;j<maxPl;j++){
            if(player[j].getStatus()==ALIVE) cout<<player[j].name<<"\n";
        }
	}
	printDivider();
	cout<<"The wolves:\n";
	for(int j=0;j<maxPl;j++){
		if(player[j].getRole()==LUPO) cout<<player[j].name<<"\n";
	}
	printDivider();
	cout<<"The guards:\n";
	for(int j=0;j<maxPl;j++){
		if(player[j].getRole()==GUARDIA) cout<<player[j].name<<"\n";
	}
	printDivider();
	cout<<"The diviners:\n";
	for(int j=0;j<maxPl;j++){
		if(player[j].getRole()==VEGGENTE) cout<<player[j].name<<"\n";
	}
	printDivider();
	cout<<"The druids:\n";
	for(int j=0;j<maxPl;j++){
		if(player[j].getRole()==DRUIDO) cout<<player[j].name<<"\n";
	}
	printDivider();
	cout<<"\npress any key to close the program...";
}

void printPlayers(string str,...){
	;
}

void printPlayers(role_t rl,bool letters,char gra){
	for(int j=0;j<maxPl;j++){
		if(letters) cout<<(char)(ASCII_START+j);
		cout<<gra<<player[j].name;
		//special attributes visible to everyone
		if(rl!=NARRATORE){
            if(!player[j].isAlive()) cout<<" (DEAD)";
            if(player[j].getStatus()==MURDERED) cout<< "(MURDERED)";
            if(player[j].getStatus()==IMPALED) cout<<" (IMPALED)";
            if(player[j].isSpecial(SINDACO)) cout<<" (MAYOR)";
            if(player[j].isSpecial(DECANO)) cout<<" (DEAN)";
		}
		//special attributes only wolves can see
		if(rl==LUPO){
			if(player[j].getRole()==LUPO) cout<<" (WOLF)";
		}
		//special attributes only guards can see
		if(rl==GUARDIA){
            if(player[j].getRole()==GUARDIA) cout<<" (GUARD)";
		}
		//special attributes only visible to the omniscient narrator
		if(rl==NARRATORE){
			if(player[j].getRole()!=CONTADINO) cout<<" ("<<toString("role",player[j].getRole())<<")";
			if(player[j].getStatus()!=ALIVE) cout<<" ("<<toString("status",player[j].getStatus())<<")";
			for(int k=0;k<MAX_SP;k++){
                if(player[j].getSpecialEle(k)!=NULL) cout<<" ("<<toString("special",player[j].getSpecialEle(k))<<")";
			}
		}
		//go to the next line
		cout<<"\n";
	}
}

void printPlayers(Player pl,bool letters,char gra){
	for(int j=0;j<maxPl;j++){
		if(letters) cout<<(char)(ASCII_START+j);
		cout<<gra<<player[j].name;
		//special attributes visible to everyone
		if(!player[j].isAlive()) cout<<" (DEAD)";
		if(player[j].getStatus()==MURDERED) cout<<" (MURDERED)";
		if(player[j].getID()==pl.getID()) cout<<" (YOU)";
		if(player[j].isSpecial(SINDACO)) cout<<" (MAYOR)";
		if(player[j].isSpecial(DECANO)) cout<<" (DEAN)";
		if(player[j].getStatus()==IMPALED) cout<<" (IMPALED)";
		//special attributes only wolves can see
		if(pl.getRole()==LUPO){
			if(player[j].getRole()==LUPO) cout<<" (WOLF)";
		}
		//special attributes only guards can see
		if(pl.getRole()==GUARDIA){
            if(player[j].getRole()==GUARDIA) cout<< " (GUARD)";
		}
		//go to the next line
		cout<<"\n";
	}
}

int pickPlayer(Player pl){

}

void getVotes(status_t status){
    char choice;
    bool err;
    int power;
    //announce voting
    //introduction
    printDivider();
    if(status==IMPALED) cout<<"The voting for the stake has officially began!\n";
    else if(status==STAKE) cout<<"Too many have been sent. A second vote is required among the unfortunate.\n";
    else if(status==SAVABLE) cout<<"The town has no mayor... a new one must be elected.\n";
    getch();
    //reset votes
    for(int j=0;j<maxPl;j++) player[j].votes=0;
    //get votes
    for(int j=0;j<maxPl;j++){
        if(player[j].getStatus()==ALIVE){
            //president (5 votes)
            if(player[j].isSpecial(PRESIDENTE)) power=5;
            //mayor (2 votes)
            else if(player[j].isSpecial(SINDACO)) power=2;
            //normal citizen
            else power=1;
            //votes
            for(int k=0;k<power;k++){
                if(player[j].getContr()==PLAYER){
                    //get player vote for impalement
                    do{
                        err=false;
                        printDivider();
                        cout<<player[j].name<<", for whom would you like to vote?\n";
                        printPlayers(player[j],true,')');
                        cout<<"choice: ";
                        cin>>choice;
                        choice=choice-ASCII_START;
                        //check error
                        if(status==IMPALED) err=checkError(true,j,choice,OUT_OF_RANGE,SELF_IMPALEMENT,ACTION_ON_DEAD,ERR_NULL);
                        else if(status==STAKE) err=checkError(true,j,choice,OUT_OF_RANGE,SELF_STAKE,ACTION_ON_DEAD,STAKE_ON_NOT_IMPALED,ERR_NULL);
                        else if(status==SAVABLE) err=checkError(true,j,choice,OUT_OF_RANGE,ACTION_ON_DEAD,SELF_MAYOR_ELECT,ERR_NULL);
                    }while(err);
                    player[choice].votes++;
                }
                else if(player[j].getContr()==CPU){
                    //get random choice
                    do{
                        err=false;
                        choice=rand()%maxPl;
                        if(status==IMPALED) err=checkError(false,j,choice,SELF_IMPALEMENT,ACTION_ON_DEAD,GUARD_ACTION_ON_GUARD,ERR_NULL);
                        else if(status==STAKE) err=checkError(false,j,choice,STAKE_ON_NOT_IMPALED,SELF_STAKE,ACTION_ON_DEAD,ERR_NULL);
                        else if(status==SAVABLE) err=checkError(false,j,choice,OUT_OF_RANGE,ACTION_ON_DEAD,SELF_MAYOR_ELECT,ERR_NULL);
                    }while(err);
                    //print and implement choice
                    player[choice].votes++;
                    printDivider();
                    cout<<player[j].name<<" voted for "<<player[choice].name<<".\n";
                    getch();
                }
            }
        }
    }
}

int* orderPlayers(){
    int maxVotes=0,preVotes=0,maxVotesID=0;
    int* o=new int[maxPl];
    for(int j=0;j<maxPl;j++){
        for(int k=0;k<maxPl;k++){
            if(player[k].votes>maxVotes && ((player[k].votes<=preVotes && !arrFind(k,o,0,j)) || j==0)){
                maxVotes=player[k].votes;
                maxVotesID=k;
            }
        }
        preVotes=maxVotes;
        maxVotes=0;
        o[j]=maxVotesID;
    }
    return o;
}

int impalePlayers(bool print){
    int numImp=0;
    int* o=new int[maxPl];
    //order the players from the most voted to the less voted
    arrReset(o,maxPl,0);
    o=orderPlayers();
    //impale most voted players
    for(int j=0,k=0;j<maxPl;j++){
        if(player[o[j]].votes==0) break;
        if(j>0){
            if(player[o[j]].votes!=player[o[j-1]].votes || k<Player::numWf-1) k++;
            if(k>=Player::numWf) break;
        }
        numImp++;
        player[o[j]].setStatus(IMPALED);
    }
    //print the results
    if(print){
        printDivider();
        bool chk=false;
        for(int j=0;j<maxPl;j++){
            if(player[j].getStatus()==IMPALED){
                if(chk) cout<<", ";
                cout<<player[j].name<<" ("<<player[j].votes<<")";
                chk=true;
            }
        }
        if(numImp>Player::numWf) cout<<" are being impaled!\n";
        else cout<<" are sent to the stake!\n";
        getch();
    }
    //directly send players to the stake
    //!WIP: move this before the printing process when "just dead" is implemented
    if(numImp<=Player::numWf){
        for(int j=0;j<maxPl;j++){
            if(player[j].getStatus()==IMPALED) player[j].setStatus(STAKE);
        }
    }
    //return the number of impaled players
    return numImp;
}

int stakePlayers(bool print){
    int numSta=0;
    int* o=new int[maxPl];
    //order the players from the most voted to the less voted
    arrReset(o,maxPl,0);
    o=orderPlayers();
    //stake the most voted players and determine which ones can be saved by the mayor
    /*for(int j=0,k=0;j<maxPl;j++){
        if(player[o[j]].votes==0) break;
        if(j>0){
            if(player[o[j]].votes!=player[o[j-1]].votes || k<Player::numWf) k++;
            else{
                for(int i=j-1;i>=0;i--){
                    if(player[o[i]].votes==player[o[i+1]].votes) player[o[i]].setStatus(SAVABLE);
                    else break;
                }
                for(int i=j;i<maxPl;i++){
                    if(player[o[i]].votes==player[o[i-1]].votes){
                        numSta++;
                        player[o[i]].setStatus(SAVABLE);
                    }
                    else break;
                }
                break;
            }
            if(k>Player::numWf) break;
        }
        numSta++;
        player[o[j]].setStatus(STAKE);
    }*/
    numSta=Player::numWf;
    for(int j=0;j<Player::numWf;j++){
        player[o[j]].setStatus(STAKE);
    }
    int s=Player::numWf;
    for(int i=s-1;i>=0;i--){
        if(player[o[i]].votes==player[o[i+1]].votes) player[o[i]].setStatus(SAVABLE);
        else break;
    }
    for(int i=s;i<maxPl;i++){
        if(player[o[i]].votes==player[o[i-1]].votes){
            numSta++;
            player[o[i]].setStatus(SAVABLE);
        }
        else break;
    }
    //completely save players who are still impaled
    for(int j=0;j<maxPl;j++){
        if(player[j].getStatus()==IMPALED) player[j].setStatus(ALIVE);
    }
    //directly burn the players (mayor intervention not needed)
    if(numSta<=Player::numWf){
        for(int j=0;j<maxPl;j++){
            if(player[j].getStatus()==STAKE) player[j].setStatus(STAKE);
        }
    }
    //print the result of the voting
    if(print){
        //staked players
        printDivider();
        bool chk=false;
        for(int j=0;j<maxPl;j++){
            if(player[j].getStatus()==STAKE || player[j].getStatus()==SAVABLE){
                if(chk) cout<<", ";
                cout<<player[j].name<<" ("<<player[j].votes<<")";
                chk=true;
            }
        }
        cout<<" are sent to the stake.\n";
        getch();
    }
    //return the number of staked players
    return numSta;
}

void savePlayers(bool print){
    char choice;
    int mayorID;
    //check if the mayor is still alive
    if(Player::mayor){
        //find the mayor
        for(int j=0;j<maxPl;j++){
            if(player[j].isSpecial(SINDACO)){
                mayorID=j;
                break;
            }
        }
        //check if mayor is at the stake
        if(player[mayorID].getStatus()!=STAKE){
            //announce that the mayor is going to decide who to send to the stake
            if(print){
                printDivider();
                cout<<"Since too many citizen are at the stake,\n"<<player[mayorID].name<<" (MAYOR) will now decide who to burn and who to save!\n";
                getch();
            }
            //check if the mayor is a player or a cpu
            //player
            if(player[mayorID].getContr()==PLAYER){
                for(int j=0;j<maxPl;j++){
                    if(player[j].getStatus()==SAVABLE){
                        printDivider();
                        cout<<player[mayorID].name<<" (MAYOR), would you like to burn "<<player[j].name<<"?\n";
                        if(getch()==KEY_YES){
                            cout<<player[j].name<<" has been sent to his death by the mayor...\n";
                            player[j].setStatus(STAKE);
                        }
                        else{
                            cout<<player[j].name<<" has been saved by the mayor!\n";
                            player[j].setStatus(ALIVE);
                        }
                    }
                }
            }
            //cpu
            else{
                for(int j=0;j<maxPl;j++){
                    if(player[j].getStatus()==SAVABLE){
                        choice=rand()%PERCENT+1;
                        if(print) printDivider();
                        if(choice<PERCENT/2){
                            if(print) cout<<player[j].name<<" has been sent to his death by the mayor...\n";
                            player[j].setStatus(STAKE);
                        }
                        else{
                            if(print) cout<<player[j].name<<" has been saved by the mayor!\n";
                            player[j].setStatus(ALIVE);
                        }
                        if(print) getch();
                    }
                }
            }
        }
        else{
            //announce that the mayor is at the stake
            if(print){
                printDivider();
                cout<<"Since the mayor appears to be occupied elsewhere, all who have tied in the voting will be saved!\n";
            }
            //save all of savable players
            for(int j=0;j<maxPl;j++){
                if(player[j].getStatus()==SAVABLE) player[j].setStatus(ALIVE);
            }
        }
    }
    else{
        //announce that there is no mayor
        if(print){
            printDivider();
            cout<<"Since this town currently has no mayor, all who have tied in the voting will be saved!\n";
        }
        //save all of savable players
        for(int j=0;j<maxPl;j++){
            if(player[j].getStatus()==SAVABLE) player[j].setStatus(ALIVE);
        }
    }
}

void electMayor(bool print){
    int numElected=0;
    int newMayorId;
    int* o=new int[maxPl];
    int* elected=new int[maxPl];
    //order the players from the most voted to the less voted
    arrReset(o,maxPl,0);
    arrReset(elected,maxPl,-1);
    o=orderPlayers();
    //impale most voted players
    for(int j=0;j<maxPl;j++){
        if(player[o[j]].votes==0) break;
        if(j>0 && player[o[j]].votes!=player[o[j-1]].votes) break;
        numElected++;
        elected[j]=o[j];
    }
    //check if there any problems with multiple elections
    if(numElected>1){
        //find dean
        int deanId;
        for(int j=0;j<maxPl;j++){
            if(player[j].isSpecial(DECANO)){
                deanId=j;
                break;
            }
        }
        //make dean decide
        printDivider();
        cout<<"Since too many have been elected, The dean will now have to decide.\n";
        for(int j=0;j<maxPl && elected[j]!=-1;j++){
            if(j!=0) cout<<", ";
            cout<<player[elected[j]].name;
        }
        cout<<"\n";
        getch();
        bool err;
        char choice;
        do{
            //reset err
            err=false;
            if(player[deanId].getContr()==PLAYER){
                //get choice
                printDivider();
                cout<<player[deanId].name<<" (DEAN), who would you like to be mayor?\n";
                //!EVIL: custom print players check (cause: lazyness)
                for(int j=0;j<maxPl;j++){
                    cout<<(char)(ASCII_START+j);
                    cout<<')'<<player[j].name;
                    //special attributes visible to everyone
                    if(!player[j].isAlive()) cout<<" (DEAD)";
                    if(player[j].getStatus()==MURDERED) cout<<" (MURDERED)";
                    if(player[j].getID()==deanId) cout<<" (YOU)";
                    if(player[j].isSpecial(SINDACO)) cout<<" (MAYOR)";
                    if(player[j].isSpecial(DECANO)) cout<<" (DEAN)";
                    if(player[j].getStatus()==IMPALED) cout<<" (IMPALED)";
                    //special attributes only wolves can see
                    if(player[deanId].getRole()==LUPO){
                        if(player[j].getRole()==LUPO) cout<<" (WOLF)";
                    }
                    //special attributes only guards can see
                    if(player[deanId].getRole()==GUARDIA){
                        if(player[j].getRole()==GUARDIA) cout<< " (GUARD)";
                    }
                    //!EVIL custom attributes (specific to mayor elections)
                    if(arrFind(j,elected,0,maxPl)) cout<<" (ELECTED)";
                    //go to the next line
                    cout<<"\n";
                }
                //!EVIL OVER
                //get choice
                cout<<"choice: ";
                cin>>choice;
                choice=choice-ASCII_START;
            }
            else{
                choice=rand()%maxPl;
            }
            //check for errors, if there are any repeat
            err=checkError(player[deanId].getContr()==PLAYER,deanId,choice,OUT_OF_RANGE,ACTION_ON_DEAD,ERR_NULL);
            //!EVIL: custom error check (cause: lazyness)
            for(int j=0;j<maxPl;j++){
                if(elected[j]==choice) break;
                if(j==maxPl-1){
                    err=true;
                    if(player[deanId].getContr()==PLAYER){
                        printDivider();
                        cout<<player[choice].name<<" has not been elected, you can only choose an elected citizen.\n";
                    }
                }
            }
        }while(err);
        //finally decide new mayor
        newMayorId=choice;
    }
    //immediately decide new mayor
    else newMayorId=elected[0];
    //make new mayor mayor
    player[newMayorId].addSpecialEle(SINDACO);
    Player::mayor=true;
    //print results
    if(print){
        printDivider();
        cout<<player[newMayorId].name<<" has been elected the mayor of "<<village_name<<"!\n";
        getch();
    }
}

void day(){
    char choice;
    bool err;
	int numImp,numSta;
	//clear the screen
	system("cls");
	//first day, introduction to the game
	if(date==0){
		printDivider();
		cout<<"It's your first day at the village of "<<village_name;
		if(modNarratore) cout<<", oh all mighty narrator!";
		cout<<"\nHere is a list of the people who inhabit this place:\n";
		printDivider();
		if(modNarratore) printPlayers(NARRATORE,false,'-');
		else printPlayers(CONTADINO,false,'-');
		printDivider();
		cout<<"It has been heard that one or more werewolves may live among these people.\n";
		cout<<"It is your duty as a villager to kill them with the power of democracy...\nwithout letting democracy kill you!\n";
		cout<<"all of this, of course, will only be valid if you're not one of the wolves\nyourself ;)\n";
		cout<<"Press any key to start!\n";
		getch();
		return;
	}
	//other days
	//announce daytime
	printDivider();
	cout<<"The sun appears on the horizon. It is the "<<date;
    if(date==1) cout<<"st";
    else if(date==2) cout<<"nd";
    else if(date==3) cout<<"rd";
    else cout<<"th";
    cout<<" day since the invasion.\n";
    cout<<"It is "<<toString("weekday",getWeekDay(date))<<".\n";
    getch();
    //morning special abilities
    if(date>0){
        //HEALER (only if people have died in the preceding night)
        for(int j=0;j<maxPl;j++){
            if(player[j].getStatus()==MURDERED){
                for(int j=0;j<maxPl;j++){
                    if(player[j].isAlive() && player[j].getRole()==GUARITORE){
                        if(player[j].getContr()==PLAYER){
                            printDivider();
                            cout<<player[j].name<<" (HEALER), would you like to revive someone?\nReviving someone will reveal your profession.";
                            if(getch()==KEY_YES){
                                cout<<endl;
                                do{
                                    //reset err
                                    err=false;
                                    //get choice
                                    printDivider();
                                    cout<<player[j].name<<" (HEALER), who would you like to revive?\n";
                                    printPlayers(player[j],true,')');
                                    cout<<"choice: ";
                                    cin>>choice;
                                    choice=choice-ASCII_START;
                                    //check for errors, if there are any repeat
                                    err=checkError(true,j,choice,OUT_OF_RANGE,ACTION_ON_DEAD,REVIVE_ON_LIVING,SELF_DIVINE,ERR_NULL);
                                }while(err);
                                //revive selected player
                                player[j].setRole(CONTADINO);
                                player[choice].setStatus(ALIVE);
                                printDivider();
                                cout<<player[choice].name<<" has been brought back from the dead!\n";
                                getch();
                            }
                            else cout<<endl;
                        }
                        else if(player[j].getContr()==CPU){
                            //see if the player "wants" to revive someone
                            int r=rand()%100;
                            if(r==0){
                                //get random choice
                                do{
                                    err=false;
                                    choice=rand()%maxPl;
                                    err=checkError(false,j,choice,OUT_OF_RANGE,ACTION_ON_DEAD,REVIVE_ON_LIVING,SELF_DIVINE,GUARD_ACTION_ON_GUARD,ERR_NULL);
                                }while(err);
                                //revive fallen player and make a the healer a normal citizen
                                player[j].setRole(CONTADINO);
                                player[choice].setStatus(ALIVE);
                                printDivider();
                                cout<<player[j].name<<" (HEALER) has decided to revive "<<player[choice].name<<"!\n";
                                getch();
                            }
                        }
                    }
                }
                break;
            }
        }
    }
    //kill players who have been murdered by wolves
    for(int j=0;j<maxPl;j++){
        if(player[j].getStatus()==MURDERED) player[j].kill();
    }
    //DEAN REELECION (in case the previous dean has died)
    if(!Player::dean) electDean();
    //MAYOR REELECTION (in case he previous mayor has died)
    if(!Player::mayor){
        getVotes(SAVABLE);
        electMayor(true);
    }
    //IMPALEMENT (voting)
    getVotes(IMPALED);
    //IMPALEMENT (impale voted players)
    //send the most voted players to the impalement and print the result of the voting
    numImp=impalePlayers(true);
    //check if there are too much people to be sent to the stake
    if(numImp>Player::numWf){
        //STAKE (voting)
        //get votes for the stake
        getVotes(STAKE);
        //STAKE (stake voted players)
        //send the most voted players to the stake and print the result of the voting
        numSta=stakePlayers(true);
        //AFTERSTAKE (check)
        //check if too many citizen are being sent to the stake
        if(numSta>Player::numWf) savePlayers(true);
    }
    //priest's special ability
    for(int j=0;j<maxPl;j++){
        if(player[j].isAlive() && player[j].getRole()==PRETE && player[j].getStatus()!=STAKE && getWeekDay(date)==SUNDAY){
            if(player[j].getContr()==PLAYER){
                printDivider();
                cout<<player[j].name<<" (PRIEST), would you like to stake an additional citizen?\nDoing this will reveal your profession.";
                if(getch()==KEY_YES){
                    cout<<endl;
                    do{
                        //reset err
                        err=false;
                        //get choice
                        printDivider();
                        cout<<player[j].name<<" (PRIEST), who would you like to send to the stake?\n";
                        printPlayers(player[j],true,')');
                        cout<<"choice: ";
                        cin>>choice;
                        choice=choice-ASCII_START;
                        //check for errors, if there are any repeat
                        err=checkError(true,j,choice,OUT_OF_RANGE,ACTION_ON_DEAD,SELF_STAKE,ACTION_ON_STAKE,ERR_NULL);
                    }while(err);
                    //send selected player to the stake
                    player[choice].setStatus(STAKE);
                    printDivider();
                    cout<<player[choice].name<<" has been condemned by the priest!\n";
                    getch();
                }
                else cout<<endl;
            }
            else if(player[j].getContr()==CPU){
                //see if the player "wants" to add somebody to the stake someone
                int r=rand()%100;
                if(r<25){
                    //get random choice
                    do{
                        err=false;
                        choice=rand()%maxPl;
                        err=checkError(false,j,choice,OUT_OF_RANGE,ACTION_ON_DEAD,GUARD_ACTION_ON_GUARD,SELF_STAKE,ACTION_ON_STAKE,ERR_NULL);
                    }while(err);
                    //add chosen player to the staked ones
                    player[choice].setStatus(STAKE);
                    printDivider();
                    cout<<player[j].name<<" (PRIEST) has decided to add "<<player[choice].name<<" to the list!\n";
                    getch();
                }
            }
        }
    }
    //print the players who have been burned alive and actually burn them...
    for(int j=0;j<maxPl;j++){
        if(player[j].getStatus()==STAKE) player[j].kill();
    }
    printDivider();
    cout<<"The staked citizens have died in flames...\n";
    getch();
}

void night(){
	char choice;
	bool err;
	//clear the screen
	system("cls");
	//announce nighttime
    printDivider();
    cout<<"darkness has fallen over "<<village_name<<".\n";
    getch();
    //special abilities (always active)
    //DIVINER
    printDivider();
    cout<<"The diviners awake and take a look into the town's souls...\n";
    getch();
    for(int j=0;j<maxPl;j++){
        if(player[j].isAlive() && player[j].getRole()==VEGGENTE){
            if(player[j].getContr()==PLAYER){
                do{
                    //reset err
                    err=false;
                    //get choice
                    printDivider();
                    cout<<player[j].name<<" (DIVINER), who would you like to identify?\n";
                    printPlayers(player[j],true,')');
                    cout<<"choice: ";
                    cin>>choice;
                    choice=choice-ASCII_START;
                    //check for errors, if there are any repeat
                    err=checkError(true,j,choice,OUT_OF_RANGE,ACTION_ON_DEAD,ERR_NULL);
                }while(err);
                //identity the selected player
                printDivider();
                if(player[choice].getRole()==LUPO) cout<<player[choice].name<<" is indeed a werewolf!\n";
                else cout<<"Evil is not within "<<player[choice].name<<"'s soul...\n";
                getch();
            }
            else if(player[j].getContr()==CPU){
                //get random choice
                do{
                    err=false;
                    choice=rand()%maxPl;
                    err=checkError(false,j,choice,OUT_OF_RANGE,ACTION_ON_DEAD,GUARD_ACTION_ON_GUARD,ERR_NULL);
                }while(err);
                //if a werewolf is identified, try to get the diviner to vote for him in the next day
                //!WIP
            }
        }
    }
	//special abilities (only active after the first night)
	if(date!=0){
        //DRUID
        printDivider();
        cout<<"The druids awake and spread herbs all over the village.\n";
        getch();
        for(int j=0;j<maxPl;j++){
            if(player[j].isAlive() && player[j].getRole()==DRUIDO){
                if(player[j].getContr()==PLAYER){
                    do{
                        //reset err
                        err=false;
                        //get choice
                        printDivider();
                        cout<<player[j].name<<" (DRUID), who would you like to protect?\n";
                        printPlayers(player[j],true,')');
                        cout<<"choice: ";
                        cin>>choice;
                        choice=choice-ASCII_START;
                        //check for errors, if there are any repeat
                        err=checkError(true,j,choice,SELF_PROTECT,OUT_OF_RANGE,ACTION_ON_DEAD,ERR_NULL);
                    }while(err);
                }
                else if(player[j].getContr()==CPU){
                    //get random choice
                    do{
                        err=false;
                        choice=rand()%maxPl;
                        err=checkError(false,j,choice,SELF_PROTECT,OUT_OF_RANGE,ACTION_ON_DEAD,ERR_NULL);
                    }while(err);
                }
                //protect the selected player from wolf attacks
                player[choice].addSpecialEle(PROTETTO);
            }
        }
        //VAMPIRE
        printDivider();
        cout<<"The vampires awake, hungry for blood...\n";
        getch();
        for(int j=0;j<maxPl;j++){
            if(player[j].isAlive() && player[j].getRole()==VAMPIRO){
                if(player[j].getContr()==PLAYER){
                    do{
                        //reset err
                        err=false;
                        //get choice
                        printDivider();
                        cout<<player[j].name<<" (VAMPIRE), where would you like to feed tonight?\n";
                        printPlayers(player[j],true,')');
                        cout<<"choice: ";
                        cin>>choice;
                        choice=choice-ASCII_START;
                        //check for errors, if there are any repeat
                        err=checkError(true,j,choice,SELF_BITE,OUT_OF_RANGE,ACTION_ON_DEAD,ERR_NULL);
                    }while(err);
                    //announce wolf kill
                    if(player[choice].getRole()==LUPO){
                        printDivider();
                        cout<<"You taste wolf blood and kill the food thief!\n";
                        getch();
                    }
                }
                else if(player[j].getContr()==CPU){
                    //get random choice
                    do{
                        err=false;
                        choice=rand()%maxPl;
                        err=checkError(false,j,choice,SELF_BITE,OUT_OF_RANGE,ACTION_ON_DEAD,ERR_NULL);
                    }while(err);
                }
                //kill the selected player if he's a wolf
                if(player[choice].getRole()==LUPO) player[choice].setStatus(MURDERED);
                //make the priest unholy if bitten
                else if(player[choice].getRole()==PRETE) player[choice].addSpecialEle(DISSACRATO);
            }
        }
        //KAMIKAZE
        printDivider();
        cout<<"...\n";
        getch();
        for(int j=0;j<maxPl;j++){
            if(player[j].isAlive() && player[j].getRole()==KAMIKAZE){
                if(player[j].getContr()==PLAYER){
                    char explodeCom;
                    printDivider();
                    cout<<player[j].name<<" (KAMIKAZE), would you like to explode tonight (y/n)? ";
                    cin>>explodeCom;
                    if(explodeCom == 'y'){
                        do{
                            //reset err
                            err=false;
                            //get choice
                            printDivider();
                            cout<<player[j].name<<" (KAMIKAZE), where would you like to explode?\n";
                            printPlayers(player[j],true,')');
                            cout<<"choice: ";
                            cin>>choice;
                            choice=choice-ASCII_START;
                            //check for errors, if there are any repeat
                            err=checkError(true,j,choice,SELF_EXPLODE,OUT_OF_RANGE,ACTION_ON_DEAD,ERR_NULL);
                        }while(err);
                        //announce kill
                        printDivider();
                        cout<<"ALLAHU AKBAR!!\n";
                        getch();
                    }
                }
                else if(player[j].getContr()==CPU){
                    if(rand()%100 < 20){
                        //get random choice
                        do{
                            err=false;
                            choice=rand()%maxPl;
                            err=checkError(false,j,choice,SELF_EXPLODE,OUT_OF_RANGE,ACTION_ON_DEAD,ERR_NULL);
                        }while(err);
                        //announce explosion
                        printDivider();
                        cout<<"The air is filled with an ear-piercing screech...\n";
                        printDivider();
                        getch();
                        cout<<"ALLAHU AKBAR!!\n";
                        getch();
                    }
                }
                //kill the player
                player[j].setStatus(MURDERED);
                //kill the selected player
                player[choice].setStatus(MURDERED);
            }
        }
	}
	//make the wolves decide their pray (only active after the first night)
	if(date!=0){
        for(int j=0;j<maxPl;j++){
            if(player[j].isAlive() && player[j].getRole()==LUPO){
                if(player[j].getContr()==PLAYER){
                    do{
                        //reset err
                        err=false;
                        //get wolf choice
                        printDivider();
                        cout<<player[j].name<<" (WOLF), who would you like to kill tonight?\n";
                        printPlayers(player[j],true,')');
                        cout<<"choice: ";
                        cin>>choice;
                        choice=choice-ASCII_START;
                        //check for errors, if there are any repeat
                        err=checkError(true,j,choice,OUT_OF_RANGE,SELF_WOLF_KILL,ACTION_ON_DEAD,ACTION_ON_MURDERED,WOLF_KILL_WOLF,ERR_NULL);
                    }while(err);
                    //druid protection
                    if(player[choice].isSpecial(PROTETTO)){
                        printDivider();
                        cout<<player[choice].name<<"'s house is surrounded by venomous herbs!\nIt looks like it's going to be a supper-less night...\n";
                        getch();
                    }
                    //house is not protected. Kill the chosen player
                    else{
                        //son of the wolves
                        if(player[choice].getRole()==FIGLIO){
                            printDivider();
                            cout<<player[choice].name<<" was raised by werewolves.\nHe finally discovers his true nature!\n";
                            Player::numWf++;
                            getch();
                        }
                        //murder chosen player
                        player[choice].setStatus(MURDERED);
                    }
                }
                else if(player[j].getContr()==CPU){
                    //get random choice
                    do{
                        err=false;
                        choice=rand()%maxPl;
                        err=checkError(false,j,choice,OUT_OF_RANGE,SELF_WOLF_KILL,ACTION_ON_DEAD,ACTION_ON_MURDERED,WOLF_KILL_WOLF,ERR_NULL);
                    }while(err);
                    //druid protection
                    if(player[choice].isSpecial(PROTETTO)){
                        printDivider();
                        cout<<player[choice].name<<" has been saved by a druid!\n";
                        getch();
                    }
                    //house is not protected. Kill the chosen player
                    else{
                        player[choice].setStatus(MURDERED);
                        //son of wolves
                        if(player[choice].getRole()==FIGLIO){
                            printDivider();
                            cout<<"A loud howl echoes throughout the night...\n";
                            getch();
                        }
                        else{
                            printDivider();
                            cout<<player[choice].name<<" has been found dead.\n";
                            getch();
                        }
                    }
                }
            }
        }
	}
	for(int j=0;j<maxPl;j++){
        //transform sons of wolves
        if(player[j].getRole()==FIGLIO && player[j].getStatus()==MURDERED){
            player[j].setRole(LUPO);
            player[j].setStatus(ALIVE);
        }
        //remove special effects from druids
        player[j].removeSpecialEle(PROTETTO);
	}
	//make the vigilante decide their pray
	//increase the date counter
	date++;
}
