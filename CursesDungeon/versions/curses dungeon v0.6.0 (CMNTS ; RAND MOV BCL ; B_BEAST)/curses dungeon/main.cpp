/*
Programmatore       : Stefano Montesi
Programma           : "Dungeon" (il nome cambiera' eventualmente)
Descrizione         : Un piccolo giochino stile "rough" fortemente in fase beta (:D)
Ultima modifica     : 6/24/2013 - 10:45 PM
*/

#include <stdio.h>
#include <stdlib.h>  //for rand() & srand()
#include <time.h>    //for srand(time(NULL))
#include <curses.h>
#include <string>
#include <sstream> //for variable types conversions
using namespace std;

#define MAS 200

int Ir,j,k,x,y,x2,y2,vx,vy,ax,ay,mx,my;
float aAtt,aAmL,aAML,aAtS,aDod,aHP,aMHP,aReg,aMov,aVel,aVis,aG,aRs;
short aFcl,aBcl,aCpp;
string aNam;
float Fr;
char com;
int mvx=32; //max=78 || max=maxx //def.=32 //HAS TO BE EVEN
int mvy=17; //max=20 || max=maxy //def.=17
int maxx=150;
int maxy=150;
float  status[200][200][200];
string text[200][200][200];
string cmnts;
short Icmt; //indicatore commenti
int Iinv='a';

//game modes (for debugging and easter eggs :D)
bool M_ghost=false;  //the player turns into a ghost and continues playing with negative HP instead of dying when his HP reach 0
bool M_zombie=false; //when the player's HP reach 0, he revives as a zombie (mHP/2;att*2;reg=0)

//status[x][y][ps_rs] - "Role Status" the role that the entity is playing
short rs_adv=1;
short rs_ene=2;
short rs_fri=3;
short rs_und=4;

//***status[x][y][n] - "Position Status" the position in the third dimension of the status array in which to strore what (n)***//
//graphics and attributes
short maxst=21;  //st != 12<x<20
short ps_gro=0;
short ps_G  =1;
short ps_fcl=2;  //character (front) color
short ps_bcl=3;  //background color
short ps_cpp=4;  //color pairs pointer
short ps_rs =5;  //role status
short ps_HP =6;  //hit point (health)
short ps_mHP=7;  //maximum hit points
short ps_att=8;  //basic phisical attack (garanteed damage if the attack is succesful)
short ps_amL=9;  //Minimum Attack Luck >atL = >chances of having a single attack deal more damage
short ps_aML=10; //Maximum Attack Luck >atL = >chances of having a single attack deal more damage
short ps_atS=11; //ATtack Speed: >atS = >chances of attacking a foe
short ps_dod=12; //ATtack Speed: >dod = >chances of dodging an incoming attack
short ps_reg=13; //HP regenerated each turn
short ps_vis=14; //length of vision range
short ps_mov=15; //movement points
short ps_vel=16; //ammount of movement points gained each game-phase
//item drops
short ps_id1=20;
short ps_Mr1=21;
short ps_mr1=22;
//items
short ps_iG =30;
//**text
short ps_nam=0;
short ps_des=1;
//items
short ps_inm=10;
short ps_ide=11;

/*
//Controlls:
char c_N =72;
char c_NE=71;
char c_E =75;
char c_SE=79;
char c_S =80;
char c_SO=81;
char c_O =77;
char c_NO=73;
*/

//number controls (for numpad without numlock)
char c_N ='8';
char c_NE='7';
char c_E ='4';
char c_SE='1';
char c_S ='2';
char c_SO='3';
char c_O ='6';
char c_NO='9';

//Technical stuff
char t_null=0;

//color pairs positions
short cpp_def=1;

//character colors
short fcl_def=COLOR_WHITE;

//background colors
short bcl_def=COLOR_BLACK;


//sleep function using time.h (copied from "http://ubuntuforums.org/showthread.php?t=296142")
void sleep(unsigned int mseconds){
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

/*string get_string(){
	char c;
	string s="",rs="";
	stringstream ss;
	while(c!='o'){
		c=getch();
		ss << (char)c;
		ss >> s;
		rs=rs+s;
		printw("%c",c);
		s="";
		ss.flush();
	}
	return rs;
}*/

void print_attr(const char* name,float n,int row,int col){
	//a f. that prints an ATTRibute
	move(row,40+(col*15));
	addstr(name);
	printw(" =");
	if(n<10) printw("   ");
	if(n>=10 && n<100) printw("  ");
	if(n>=100 && n<1000) printw(" ");
	printw("%.1f",n);
}

bool lifeChk(int xx,int yy){
	if(status[xx][yy][ps_rs]!=t_null) return true;
	else return false;
}

bool spawnChk(int xx,int yy){
	if(status[xx][yy][ps_rs]==t_null) return true;
	else return false;
}

bool monsterChk(int xx,int yy){
	if(status[xx][yy][ps_rs]==rs_ene) return true;
	else if(status[xx][yy][ps_rs]==rs_fri) return true;
	else return false;
}

bool AIChk(int xx,int yy){
	if(status[xx][yy][ps_rs]==rs_ene) return true;
	else if(status[xx][yy][ps_rs]==rs_fri) return true;
	else return false;
}

bool moveChk(int xx,int yy,int ox,int oy){
	if(status[ox][oy][ps_rs]==rs_und) return false;
	else{
		if(status[xx][yy][ps_rs]==rs_adv){
			if(status[ox][oy][ps_rs]==t_null) return true;
			else return false;
		}
		else if(status[xx][yy][ps_rs]==rs_ene){
			if(status[ox][oy][ps_rs]==t_null) return true;
			else return false;
		}
		else return false;
	}
}

bool attkChk(int xx,int yy,int ox,int oy){
	if(status[ox][oy][ps_rs]==rs_und) return false;
	else{
		if(status[xx][yy][ps_rs]==rs_adv){
			if(status[ox][oy][ps_rs]==rs_ene) return true;
			else return false;
		}
		else if(status[xx][yy][ps_rs]==rs_ene){
			if(status[ox][oy][ps_rs]==rs_adv || status[ox][oy][ps_rs]==rs_fri) return true;
			else return false;
		}
		else return false;
	}
}

bool doChk(int xx,int yy,int ox,int oy){
	if(moveChk(xx,yy,ox,oy)==true || attkChk(xx,yy,ox,oy)==true) return true;
	else return false;
}

bool enemyChk(int xx,int yy,int ox,int oy){
	if(status[xx][yy][ps_rs]==rs_adv){
		if(status[ox][oy][ps_rs]==rs_ene) return true;
		else return false;
	}
	else if(status[xx][yy][ps_rs]==rs_ene){
		if(status[ox][oy][ps_rs]==rs_adv || status[ox][oy][ps_rs]==rs_fri) return true;
		else return false;
	}
	else return false;
}

bool dangerChk(int xx,int yy,int ox,int oy){
	if(enemyChk(xx,yy,ox,oy)==true && status[ox][oy][ps_att]>=1) return true;
	else return false;
}

float FrandRng(float min, float max){
	float r = min + (float)rand()/((float)RAND_MAX/(max-min));
	return r;
}

int IrandRng(int min,int max){
	int r = min + (int)rand()/((int)RAND_MAX/(max-min));
	return r;
}

void setG(int xx,int yy,char n){
	status[xx][yy][ps_G]=n;
}

char getG(int xx,int yy){
	return status[xx][yy][ps_G];
}

void setFcl(int xx,int yy,short n){
	status[xx][yy][ps_fcl]=n;
}

short getFcl(int xx,int yy){
	return status[xx][yy][ps_fcl];
}

void setBcl(int xx,int yy,short n){
	status[xx][yy][ps_bcl]=n;
}

short getBcl(int xx,int yy){
	return status[xx][yy][ps_bcl];
}

void setCpp(int xx,int yy,short n){
	status[xx][yy][ps_cpp]=n;
}

short getCpp(int xx,int yy){
	return status[xx][yy][ps_cpp];
}

void setRS (int xx,int yy,int n){
	status[xx][yy][ps_rs]=n;
}

int getRS(int xx,int yy){
	return status[xx][yy][ps_rs];
}

void setAtt(int xx,int yy,float n){
	status[xx][yy][ps_att]=n;
}

float getAtt(int xx,int yy){
	return status[xx][yy][ps_att];
}

void chaAtt(int xx,int yy,float n){
	status[xx][yy][ps_att]+=n;
}

void molAtt(int xx,int yy,float n){
	status[xx][yy][ps_att]*=n;
}

void setAtS(int xx,int yy,float n){
	status[xx][yy][ps_atS]=n;
}

float getAtS(int xx,int yy){
	return status[xx][yy][ps_atS];
}

void chaAtS(int xx,int yy,float n){
	status[xx][yy][ps_atS]+=n;
}

void molAtS(int xx,int yy,float n){
	status[xx][yy][ps_atS]*=n;
}

void setDod(int xx,int yy,float n){
	status[xx][yy][ps_dod]=n;
}

float getDod(int xx,int yy){
	return status[xx][yy][ps_dod];
}

void chaDod(int xx,int yy,float n){
	status[xx][yy][ps_dod]+=n;
}

void molDod(int xx,int yy,float n){
	status[xx][yy][ps_dod]*=n;
}

void setAmL(int xx,int yy,float n){
	status[xx][yy][ps_amL]=n;
}

float getAmL(int xx,int yy){
	return status[xx][yy][ps_amL];
}

void chaAmL(int xx,int yy,float n){
	status[xx][yy][ps_amL]+=n;
}

void molAmL(int xx,int yy,float n){
	status[xx][yy][ps_amL]*=n;
}

void setAML(int xx,int yy,float n){
	status[xx][yy][ps_aML]=n;
}

float getAML(int xx,int yy){
	return status[xx][yy][ps_aML];
}

void chaAML(int xx,int yy,float n){
	status[xx][yy][ps_aML]+=n;
}

void molAML(int xx,int yy,float n){
	status[xx][yy][ps_aML]*=n;
}

void setHP(int xx,int yy,float n){
	status[xx][yy][ps_HP]=n;
}

float getHP(int xx,int yy){
	return status[xx][yy][ps_HP];
}

void chaHP(int xx,int yy,float n){
	status[xx][yy][ps_HP]+=n;
}

void molHP(int xx,int yy,float n){
	status[xx][yy][ps_HP]*=n;
}

void setMHP(int xx,int yy,float n){
	status[xx][yy][ps_mHP]=n;
}

float getMHP(int xx,int yy){
	return status[xx][yy][ps_mHP];
}

void chaMHP(int xx,int yy,float n){
	status[xx][yy][ps_mHP]+=n;
}

void molMHP(int xx,int yy,float n){
	status[xx][yy][ps_mHP]*=n;
}

void setReg(int xx,int yy,float n){
	status[xx][yy][ps_reg]=n;
}

float getReg(int xx,int yy){
	return status[xx][yy][ps_reg];
}

void chaReg(int xx,int yy,float n){
	status[xx][yy][ps_reg]+=n;
}

void molReg(int xx,int yy,float n){
	status[xx][yy][ps_reg]*=n;
}

void setVis(int xx,int yy,int n){
	status[xx][yy][ps_vis]=n;
}

int getVis(int xx,int yy){
	return status[xx][yy][ps_vis];
}

void chaVis(int xx,int yy,int n){
	status[xx][yy][ps_vis]+=n;
}

void molVis(int xx,int yy,float n){
	status[xx][yy][ps_vis]*=n;
}

void setMov(int xx,int yy,float n){
	status[xx][yy][ps_mov]=n;
}

float getMov(int xx,int yy){
	return status[xx][yy][ps_mov];
}

void chaMov(int xx,int yy,float n){
	status[xx][yy][ps_mov]+=n;
}

void molMov(int xx,int yy,float n){
	status[xx][yy][ps_mov]*=n;
}

void setVel(int xx,int yy,float n){
	status[xx][yy][ps_vel]=n;
}

float getVel(int xx,int yy){
	return status[xx][yy][ps_vel];
}

void chaVel(int xx,int yy,float n){
	status[xx][yy][ps_vel]+=n;
}

void molVel(int xx,int yy,float n){
	status[xx][yy][ps_vel]*=n;
}

void setName(int xx,int yy,string n){
	text[xx][yy][ps_nam]=n;
}

void Tspawn_e(int xx,int yy,string name,char g,short fcl,short bcl,short cpp,int rs,float att,float amL,float aML,float atS,float dod,float hp,float mhp,float reg,float mov,float vel,int vis){
	setName(xx,yy,name);
	setG(xx,yy,g);
	setFcl(xx,yy,fcl);
	setBcl(xx,yy,bcl);
	setCpp(xx,yy,cpp);
	setRS(xx,yy,rs);
	setHP(xx,yy,hp);
	setMHP(xx,yy,mhp);
	setReg(xx,yy,reg);
	setAtt(xx,yy,att);
	setAmL(xx,yy,amL);
	setAML(xx,yy,aML);
	setAtS(xx,yy,atS);
	setDod(xx,yy,dod);
	setMov(xx,yy,mov);
	setVel(xx,yy,vel);
	setVis(xx,yy,vis);
}

void spawn_e(int xx,int yy,const char* ent/*ENTity to spawn_*/){
	float sRS,sHP,sMHP,sReg,sAtt,sAmL,sAML,sAtS,sDod,sVis,sMov,sVel;
	char sG;
	string sNam;
	short sFcl,sBcl,sCpp;
	//playable entities (2-100)
	if(ent=="adv" || ent=="adventurer"){
		sNam =aNam;
		sG   ='@';
		sFcl =COLOR_BLACK;
		sBcl =COLOR_WHITE;
		sCpp =2;
		sRS  =rs_adv;
		sMHP =30.0;
		sHP  =sMHP;
		sReg =0.2;
		sAtt =3.0;
		sAmL =0.5;
		sAML =2.0;
		sAtS =9.0;
		sDod =2.0;
		sVis =t_null;
		sMov =t_null;
		sVel =t_null;
	}
	//entities that can't move (101-200)
	if(ent=="wall"){
		sNam ="wall";
		sG   ='#';
		sFcl =COLOR_YELLOW;
		sBcl =COLOR_BLACK;
		sCpp =101;
		sRS  =rs_und;
		sMHP =0.0;
		sHP  =0.0;
		sReg =0.0;
		sAtt =0.0;
		sAmL =0.0;
		sAML =0.0;
		sAtS =0.0;
		sDod =0.0;
		sVis =0.0;
		sMov =0.0;
		sVel =0.0;
	}
	if(ent=="tree"){
		sNam ="tree";
		sG   ='T';
		sFcl =COLOR_GREEN;
		sBcl =COLOR_BLACK;
		sCpp =102;
		sRS  =rs_ene;
		sMHP =FrandRng(50.0,100.0);
		sHP  =sMHP;
		sReg =FrandRng(0.5,1.0);
		sAtt =0.0;
		sAmL =0.0;
		sAtS =0.0;
		sDod =0.0;
		sAML =0.0;
		sVis =0.0;
		sVel =0.0;
		sMov =0.0;
	}
	//entities that move (201-)
	if(ent=="slime"){
		sNam ="slime";
		sG   ='s';
		sFcl =COLOR_MAGENTA;
		sBcl =COLOR_BLACK;
		sCpp =201;
		sRS  =rs_ene;
		sMHP =FrandRng(3.0,8.0);
		sHP  =sMHP;
		sReg =FrandRng(0.3,0.5);
		sAtt =FrandRng(1.0,2.0);
		sAmL =FrandRng(0.0,0.3);
		sAML =FrandRng(sAmL+0.1,sAmL+1.0);
		sAtS =FrandRng(10.0,11.0);
		sDod =FrandRng(0.8,1.2);
		sVis =IrandRng(20.0,40.0);
		sVel =1.0;
		sMov =sVel;
	}
	else if(ent=="creep" || ent=="creeper"){
		sNam ="creeper";
		sG   ='c';
		sFcl =COLOR_CYAN;
		sBcl =COLOR_BLACK;
		sCpp =202;
		sRS  =rs_ene;
		sMHP =FrandRng(5.0,12.0);
		sHP  =sMHP;
		sReg =FrandRng(0.0,0.2);
		sAtt =FrandRng(3.0,4.0);
		sAmL =FrandRng(0.2,0.5);
		sAML =FrandRng(sAmL+0.5,sAmL+2.5);
		sAtS =FrandRng(9.5,10.5);
		sDod =FrandRng(0.3,0.7);
		sVis =IrandRng(20.0,40.0);
		sVel =1.0;
		sMov =sVel;
	}
	else if(ent=="masti" || ent=="mastichino"){
		sNam ="mastichino";
		sG   ='m';
		sFcl =COLOR_RED;
		sBcl =COLOR_BLACK;
		sCpp =203;
		sRS  =rs_ene;
		sMHP =FrandRng(4.0,7.0);
		sHP  =sMHP;
		sReg =FrandRng(0.1,0.3);
		sAtt =FrandRng(2.0,4.0);
		sAmL =FrandRng(0.1,0.4);
		sAML =FrandRng(sAmL+0.1,sAmL+1.5);
		sAtS =FrandRng(10.5,11.5);
		sDod =FrandRng(1.8,2.5);
		sVis =IrandRng(20.0,40.0);
		sVel =FrandRng(1.2,1.5);
		sMov =sVel;
	}
	else if(ent=="b_beast" || ent=="blind_beast"){
		sNam ="blind beast";
		sG   ='b';
		sFcl =COLOR_BLUE;
		sBcl =COLOR_BLACK;
		sCpp =204;
		sRS  =rs_ene;
		sMHP =FrandRng(14.0,22.0);
		sHP  =sMHP;
		sReg =FrandRng(0.1,0.3);
		sAtt =FrandRng(3.0,6.0);
		sAmL =FrandRng(0.0,1.0);
		sAML =FrandRng(sAmL+2.0,sAmL+4.0);
		sAtS =FrandRng(7.5,9.0);
		sDod =FrandRng(0.0,0.2);
		sVis =IrandRng(5.0,15.0);
		sVel =FrandRng(0.5,1.0);
		sMov =sVel;
	}
	Tspawn_e(xx,yy,sNam,sG,sFcl,sBcl,sCpp,sRS,sAtt,sAmL,sAML,sAtS,sDod,sHP,sMHP,sReg,sMov,sVel,sVis);
}

/*void despawn(int xx,int yy){
	life[xx][yy]=t_null;
    for (j=0;j<maxst;j++) status[xx][yy][j]=t_null;
}*/

void despawn(int xx,int yy){
	status[xx][yy][ps_G]=t_null;
	status[xx][yy][ps_fcl]=t_null;
	status[xx][yy][ps_cpp]=t_null;
    status[xx][yy][ps_rs]=t_null;
    status[xx][yy][ps_HP]=t_null;
    status[xx][yy][ps_mHP]=t_null;
    status[xx][yy][ps_reg]=t_null;
    status[xx][yy][ps_att]=t_null;
    status[xx][yy][ps_amL]=t_null;
    status[xx][yy][ps_aML]=t_null;
    status[xx][yy][ps_atS]=t_null;
    status[xx][yy][ps_dod]=t_null;
    status[xx][yy][ps_mov]=t_null;
    status[xx][yy][ps_vel]=t_null;
    status[xx][yy][ps_vis]=t_null;
}

/*//TEST: put maxx = mxv & maxy = mxy
void generate_g(){
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			status[x][y][ps_gro]='.';
			if(x==maxy/2 && y!=7) spawn_e(x,y,"wall");
			//default
			if(x==maxx-1 || x==0 || y==maxy-1 || y==0) spawn_e(x,y,"wall");
		}
	}
}/**/

void generate_g(){
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			status[x][y][ps_gro]='.';
			Ir=rand()%1000;
			if(Ir>=750 && Ir<985) spawn_e(x,y,"wall");
			if(Ir>=985) spawn_e(x,y,"tree");
			//default
			if(x==maxx-1 || x==0 || y==maxy-1 || y==0) spawn_e(x,y,"wall");
		}
	}
}/**/

void start_game(){
	move(0,0);
	//start random seed
	srand(time(NULL));
	//reset
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			//reset status
			for(j=0;j<maxst;j++) status[x][y][j]=t_null;
		}
	}
	//adv. name input (WIP)
	aNam="you";
	//level initialization (WIP)
	generate_g();
	//Tspawn_e character
	j=0;
	for(y=maxy/2;y>=0;y--){
		for(x=maxx/2;x>=0;x--){
			if(spawnChk(x,y)==true){
				spawn_e(x,y,"adv");
				//ini. ax & ay
				ax=x;
				ay=y;
				//ini. vx & vy
				vx=mvx/2+x;
				vy=mvy/2+y;
				j=1;
			}
			if(j==1) break;
		}
		if(j==1) break;
	}
}

void i_spawn_ms(){
	//spawns monsters (made to be used one time only)
    for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			Ir=rand()%250;
			if(spawnChk(x,y)==true){
				if(Ir==0){
					Ir=rand()%100;
                	if(Ir<=45) spawn_e(x,y,"slime");
                	if(Ir>45 && Ir<=75) spawn_e(x,y,"masti");
					if(Ir>75 && Ir<=95) spawn_e(x,y,"creeper");
					if(Ir>95) spawn_e(x,y,"blind_beast");
				}
			}
		}
	}
}

void spawn_ms(){
	//spawns monsters (made to be used continusly)
   	for(y=0;y<maxx;y++){
		for(x=0;x<maxy;x++){
			Fr=FrandRng(0.00000,1000000000000000.00000);
			if(spawnChk(x,y)==true){
				if(Fr==0.00000){
					Ir=rand()%100;
                	if(Ir<=40) spawn_e(x,y,"slime");
                	if(Ir>40 && Ir<=75) spawn_e(x,y,"masti");
					if(Ir> 75) spawn_e(x,y,"creeper");
				}
			}
		}
	}
}

void board_updt(){
	//**COMMENTS**//
	size_t found;
	//erase excess comments
	if(Icmt>5){
		Icmt=5;
		found=0;
		for(j=0;j<=5;j++){
			found=cmnts.find("\n",found+1);
			//*DEBUG*/mvprintw(24,0,"%d - %d",(int)found,j);
			//*DEBUG*/refresh();
			//*DEBUG*/getch();
		}
		cmnts.erase(cmnts.begin()+found+1,cmnts.end());
	
	}
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			if(lifeChk(x,y)==true){
				//**UNDESTRUCTABLE OBJECTS**//
				if(status[x][y][ps_rs]==rs_und){
					;
				}
				//**ADVENTURER, MONSTERS, ALLIES, and LIFELESS**//
				if(status[x][y][ps_rs]==rs_adv || status[x][y][ps_rs]==rs_ene || status[x][y][ps_rs]==rs_fri){
					//regenerate HP
					status[x][y][ps_HP]+=status[x][y][ps_reg];
					//keep HP lower than mHP
					if(status[x][y][ps_HP]>status[x][y][ps_mHP]) status[x][y][ps_HP]=status[x][y][ps_mHP];
					//**MONSTERS and ALLIES ONLY**//
					if(status[x][y][ps_rs]==rs_ene || status[x][y][ps_rs]==rs_fri){
						//regenerate mov
						status[x][y][ps_mov]+=status[x][y][ps_vel];
						//despawn monsters when their HP reahces 0
						if(status[x][y][ps_HP]<1 && status[x][y][ps_rs]!=rs_adv) despawn(x,y);
					}
				}
				//**ADVENTURER ONLY (modes)**//
				if(status[x][y][ps_rs]==rs_adv){
					//ghost mode
					if(M_ghost==true){
						if(status[ax][ay][ps_HP]<1) setG(ax,ay,'G');
						if(status[ax][ay][ps_HP]>=1) setG(ax,ay,'@');
					}
					//zombie mode
					else if(M_zombie==true){
						if(status[ax][ay][ps_HP]<1){
							if(status[ax][ay][ps_G]!='Z'){
								setG(ax,ay,'Z');
								setFcl(ax,ay,COLOR_GREEN);
								molMHP(ax,ay,0.5);
								setHP(ax,ay,status[ax][ay][ps_mHP]);
								molAtt(ax,ay,2);
								setReg(ax,ay,0);
							}
						}
					}
				}
			}
		}
	}
}

void print_vision(){
	int DmnsN=0,SmnsN=0;  //Detiled and Superficial MoNSter Numbers
	move(0,0);
	//erase side comments
	for(y=0;y<100;y++){
		for(x=38;x<100;x++){
			mvprintw(y,x," ");
		}
	}
	//print board
   	for(y=0;y<mvy;y++){
		for(x=0;x<mvx;x++){
			//set colors
			init_pair(status[vx-x][vy-y][ps_cpp],status[vx-x][vy-y][ps_fcl],status[vx-x][vy-y][ps_bcl]);
			attron(COLOR_PAIR(status[vx-x][vy-y][ps_cpp]));
			//print game-screen
			if(lifeChk(vx-x,vy-y)==true) mvaddch(y,x,status[vx-x][vy-y][ps_G]);
			else mvaddch(y,x,status[vx-x][vy-y][ps_gro]);
			//reset default color
			init_pair(cpp_def,fcl_def,bcl_def);
			attron(COLOR_PAIR(cpp_def));
			//print monsters and items inside vision range(WIP)
			if(dangerChk(ax,ay,vx-x,vy-y)==true){
				attron(COLOR_PAIR(status[vx-x][vy-y][ps_cpp]));
				mvprintw(8+DmnsN*2,38,"%c = %s",(char)status[vx-x][vy-y][ps_G],text[vx-x][vy-y][ps_nam].c_str());
				mvprintw(9+DmnsN*2,38,"att:%.1f amL:%.1f aML:%.1f HP :%.1f reg:%.1f\t",status[vx-x][vy-y][ps_att],status[vx-x][vy-y][ps_amL],status[vx-x][vy-y][ps_aML],status[vx-x][vy-y][ps_HP],status[vx-x][vy-y][ps_reg]);
				attron(COLOR_PAIR(cpp_def));
				DmnsN++;
			}
			else if(monsterChk(vx-x,vy-y)==true){
				attron(COLOR_PAIR(status[vx-x][vy-y][ps_cpp]));
				mvprintw(7,38+7*SmnsN,"%c=%.1f",(char)status[vx-x][vy-y][ps_G],status[vx-x][vy-y][ps_HP]);
				attron(COLOR_PAIR(cpp_def));
				SmnsN++;
			}
		}
	}
	//**PRINT ADV ATTRIBUTES**//
	print_attr("ATT",status[ax][ay][ps_att],1,0);
	print_attr("HP ",status[ax][ay][ps_HP],2,0);
	print_attr("REG",status[ax][ay][ps_reg],3,0);
	print_attr("mHP",status[ax][ay][ps_mHP],2,1);
	//*--DEBUG--*/print_attr("xAT",status[ax][ay+1][ps_att],1,1);
	//*--DEBUG--*/print_attr("xHP",status[ax][ay+1][ps_HP],2,1);
	//**PRINT COMMENTS**//
	mvprintw(18,0,"%s",cmnts.c_str());
}

void attk(int atx,int aty,int dex,int dey){
	//WIP
	stringstream SSatG,SSdeG;
	string SatG,SdeG,atNam,deNam,attVerb,attDes;
	float dam,att,amL,aML,atL,atP,atS,deD,FatS,FdeD,doP;
	bool hit;
	//damage calculation
	att=status[atx][aty][ps_att];
	amL=status[atx][aty][ps_amL];
	aML=status[atx][aty][ps_aML];
	atL=FrandRng(amL,aML);
	dam=att+atL;
	//dodge chance calculation
	atS=status[atx][aty][ps_atS];
	deD=status[dex][dey][ps_dod];
	FatS=atS-deD;
	FdeD=FrandRng(0.0,10.0);
	if(FdeD<=FatS) hit=true;
	else hit=false;
	//damage infliction
	if(hit==true) chaHP(dex,dey,-dam);
	//attack comments
	SSatG << (char)status[atx][aty][ps_G];
	SSatG >> SatG;
	SSdeG << (char)status[dex][dey][ps_G];
	SSdeG >> SdeG;
	atNam=text[atx][aty][ps_nam];
	deNam=text[dex][dey][ps_nam];
	//attack subjects
	if(status[atx][aty][ps_rs]==rs_adv){
		atNam="you";
		if(hit==true) attVerb="attack";
		else attVerb="miss";
	}
	else{
		atNam="the "+atNam;
		if(hit==true) attVerb="attacks";
		else attVerb="misses";
	}
	if(status[dex][dey][ps_rs]==rs_adv) deNam="you";
	else deNam="the "+deNam;
	if(hit==true){
		atP=((atL-amL)*100)/(aML-amL);
		if(atP<0){
			attDes="so badly that you laugh...";
		}
		if(atP==0){
			attDes="like a baby";
		}
		if(atP>0 && atP<=25){
			attDes="horribly";
		}
		if(atP>25 && atP<=50){
			attDes="badly";
		}
		if(atP>50 && atP<=75){
			attDes="with an average demonstration of might";
		}
		if(atP>75 && atP<=90){
			attDes="with an outstanding demonstration of valor";
		}
		if(atP>90 && atP<100){
			attDes="with undescussible perfection!";
		}
		if(atP==100){
			attDes="WITH A GODLY DISPLAY OF POWER!";
		}
		if(atP>100){
			attDes="in such a way that cannot be described by mortals...";
		}
	}
	else{
		doP=(FdeD*100)/FatS;
		if(doP<0){
			attDes="i";
		}
		if(doP==0){
			attDes="u";
		}
		if(doP>0 && doP<=25){
			attDes="uu";
		}
		if(doP>25 && doP<=50){
			attDes="uuu";
		}
		if(doP>50 && doP<=75){
			attDes="uuuu";
		}
		if(doP>75 && doP<=90){
			attDes="uuuuu";
		}
		if(doP>90 && doP<100){
			attDes="uuuuuu";
		}
		if(doP==100){
			attDes="uuuuuuu";
		}
		if(atP>100){
			attDes="uuuuuuuu";
		}
	}
	cmnts=atNam+" "+attVerb+" "+deNam+" "+attDes+"\n"+cmnts;
	Icmt++;
}

void mns_move(){
	bool advfChk=false;
	int movx,movy,ox,oy,mnx,mny,path;
	float mAtt,mAmL,mAML,mAtS,mDod,mHP,mMHP,mReg,mMov,mVel,mVis,mG,mRs;
	short mFcl,mBcl,mCpp;
	string mNam;
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			if(AIChk(x,y)==true && status[x][y][ps_mov]>=1){
				mnx=x;
				mny=y;
				//define random variable to go around obstacles (so the enemies will choose a random path, not the shortest one XD, WIP)
				path=IrandRng(0,100);
				for(;status[mnx][mny][ps_mov]>=1;status[mnx][mny][ps_mov]--){
					//*DEBUG*/mvprintw(12,40,"%.1f",status[mnx][mny][ps_mov]);
					//*DEBUG*/refresh();
					//*DEBUG*/getch();
					mNam=text[mnx][mny][ps_nam];
					mG  =status[mnx][mny][ps_G];
					mRs =status[mnx][mny][ps_rs];
					mCpp=status[mnx][mny][ps_cpp];
					mAtt=status[mnx][mny][ps_att];
					mAmL=status[mnx][mny][ps_amL];
					mAML=status[mnx][mny][ps_aML];
					mAtS=status[mnx][mny][ps_atS];
					mDod=status[mnx][mny][ps_dod];
					mHP =status[mnx][mny][ps_HP];
					mMHP=status[mnx][mny][ps_mHP];
					mReg=status[mnx][mny][ps_reg];
					mVis=status[mnx][mny][ps_vis];
					mVel=status[mnx][mny][ps_vel];
					for(y2=mny+mVis/2;(y2>=mny-mVis/2 && y2>0);y2--){
						for(x2=mnx+mVis/2;(x2>=mnx-mVis/2 && x2>0);x2--){
							if(enemyChk(mnx,mny,x2,y2)==true){
								status[mnx][mny][ps_bcl]=COLOR_BLACK;
								advfChk=true;	
								//*DEBUG*/mvprintw(14,40,"adv detcted by %c at coos: %d,%d",mG,mnx,mny);
								//*DEBUG*/mvprintw(15,40,"adv is at coos: %d,%d",x2,y2);
								//*DEBUG*/refresh();
								//*DEBUG*/getch();
								movx=x2-mnx;
								movy=y2-mny;
							}
						}
					}
					if(advfChk==false){
						status[mnx][mny][ps_bcl]=COLOR_YELLOW;
						x2=IrandRng(mnx+mVis/2,mnx-mVis/2);
						y2=IrandRng(mny+mVis/2,mny-mVis/2);
						movx=x2-mnx;
						movy=y2-mny;
					}
					advfChk=false;
					//calculation of "ox and oy" (where the monster is going to move at)
					if(movx==0) ox=mnx;
					else{
						movx=movx/abs(movx);
						ox=mnx+movx;
					}
					if(movy==0) oy=mny;
					else{
						movy=movy/abs(movy);
						oy=mny+movy;
					}
					if(doChk(mnx,mny,ox,oy)==false){
						if(path<50){
							for(j=0;((doChk(mnx,mny,ox,oy)==false) && j<8);j++){
								if(ox>mnx && oy<mny) oy++;
								else if(ox>mnx && oy==mny) oy++;
								else if(ox>mnx && oy>mny) ox--;
								else if(ox==mnx && oy>mny) ox--;
								else if(ox<mnx && oy>mny) oy--;
								else if(ox<mnx && oy==mny) oy--;
								else if(ox<mnx && oy<mny) ox++;
								else if(ox==mnx && oy<mny) ox++;
							}
						}
						else{
							for(j=0;((doChk(mnx,mny,ox,oy)==false) && j<8);j++){
								if(ox>mnx && oy<mny) ox--;
								else if(ox==mnx && oy<mny) ox--;
								else if(ox<mnx && oy<mny) oy++;
								else if(ox<mnx && oy==mny) oy++;
								else if(ox<mnx && oy>mny) ox++;
								else if(ox==mnx && oy>mny) ox++;
								else if(ox>mnx && oy>mny) oy--;
								else if(ox>mnx && oy==mny) oy--;
							}
						}
					}
					if(enemyChk(mnx,mny,ox,oy)==true){
						//*DEBUG*/mvprintw(11,40,"adv (%d,%d) attacked by %c (%d,%d)",ox,oy,mG,mnx,mny);
						//*DEBUG*/mvprintw(12,40,"adv current life: %d",status[ox][oy][ps_HP]);
						//*DEBUG*/refresh();
						//*DEBUG*/getch();
						attk(mnx,mny,ox,oy);
						ox=mnx;
						oy=mny;
					}
					mMov=status[mnx][mny][ps_mov];
					mFcl=status[mnx][mny][ps_fcl];
					mBcl=status[mnx][mny][ps_bcl];
					despawn(mnx,mny);
					Tspawn_e(ox,oy,mNam,mG,mFcl,mBcl,mCpp,mRs,mAtt,mAmL,mAML,mAtS,mDod,mHP,mMHP,mReg,mMov,mVel,mVis);
					//*DEBUG*/mvprintw(12,40,"spawned %c from %d %d to %d %d",mG,mnx,mny,ox,oy);
					//*DEBUG*/refresh();
					//*DEBUG*/getch();
					mnx=ox;
					mny=oy;
				}
				//*DEBUG*/mvprintw(12,40,"%.1f",status[mnx][mny][ps_mov]);
				//*DEBUG*/refresh();
				//*DEBUG*/getch();
			}
		}
	}
}

void adv_move(){
	aNam=text[ax][ay][ps_nam];
	aG  =status[ax][ay][ps_G];
	aRs =status[ax][ay][ps_rs];
	aFcl=status[ax][ay][ps_fcl];
	aBcl=status[ax][ay][ps_bcl];
	aCpp=status[ax][ay][ps_cpp];
	aAtt=status[ax][ay][ps_att];
	aAmL=status[ax][ay][ps_amL];
	aAML=status[ax][ay][ps_aML];
	aAtS=status[ax][ay][ps_atS];
	aDod=status[ax][ay][ps_dod];
	aHP =status[ax][ay][ps_HP];
	aMHP=status[ax][ay][ps_mHP];
	aReg=status[ax][ay][ps_reg];
	aVis=status[ax][ay][ps_vis];
	aVel=status[ax][ay][ps_vel];
	mx=ax;
	my=ay;
	if(com==c_N){
		//change adv. coos
		ay++;
		//move adv. vision & check if adv. can move
		if(moveChk(mx,my,ax,ay)==true){
			if (ay<maxy-mvy/2 && ay>mvy/2) vy++;
		}
	}
	if(com==c_NE){
		//change adv. coos
		ay++;
		ax++;
		//move adv. vision & check if adv. can move
		if(moveChk(mx,my,ax,ay)==true){
			if(ay<maxy-mvy/2 && ay>mvy/2) vy++;
			if(ax<maxx-mvx/2 && ax>mvx/2-1) vx++;
		}
	}
	if(com==c_E){
		//change adv. coos
		ax++;
		//move adv. vision & check if adv. can move
		if(moveChk(mx,my,ax,ay)==true){
			if(ax<maxx-mvx/2 && ax>mvx/2-1) vx++;
		}
	}
	if(com==c_SE){
		//change adv. coos
		ay--;
		ax++;
		//move adv. vision & check if adv. can move
		if(moveChk(mx,my,ax,ay)==true){
			if(ay>mvy/2-1 && ay<maxy-mvy/2-1) vy--;
			if(ax<maxx-mvx/2 && ax>mvx/2-1) vx++;
		}
	}
	if(com==c_S){
		//change adv. coos
		ay--;
		//move adv. vision & check if adv. can move
		if(moveChk(mx,my,ax,ay)==true){
			if(ay>mvy/2-1 && ay<maxy-mvy/2-1) vy--;
		}
	}
	if(com==c_SO){
		//change adv. coos
		ay--;
		ax--;
		//move adv. vision & check if adv. can move
		if(moveChk(mx,my,ax,ay)==true){
			if(ay>mvy/2-1 && ay<maxy-mvy/2-1) vy--;
			if(ax>mvx/2-2 && ax<maxx-mvx/2-1) vx--;
		}
	}
	if(com==c_O){
		//change adv. coos
		ax--;
		//move adv. vision & check if adv. can move
		if(moveChk(mx,my,ax,ay)==true){
			if(ax>mvx/2-2 && ax<maxx-mvx/2-1) vx--;
		}
	}
	if(com==c_NO){
		//change adv. coos
		ay++;
		ax--;
		//move adv. vision & check if adv. can move
		if(moveChk(mx,my,ax,ay)==true){
			if(ay<maxy-mvy/2 && ay>mvy/2) vy++;
			if(ax>mvx/2-2 && ax<maxx-mvx/2-1) vx--;
		}
	}
	if(com=='s'){
		i_spawn_ms();
	}
	if(com=='r'){
		aHP=aMHP;
	}
	//check if there's an hostile "life" in the place that the adv. is trying to move in and attack it
	if(enemyChk(mx,my,ax,ay)==true){
		attk(mx,my,ax,ay);
	}
	if(moveChk(mx,my,ax,ay)==false){
		ax=mx;
		ay=my;
	}
	despawn(mx,my);
	Tspawn_e(ax,ay,aNam,aG,aFcl,aBcl,aCpp,aRs,aAtt,aAmL,aAML,aAtS,aDod,aHP,aMHP,aReg,aMov,aVel,aVis);
}

bool chk_death(){
	if(M_ghost==false && status[ax][ay][ps_HP]<1) return true;
	else return false;
}

void end(){
	erase();
	move(0,0);
	printw("you lost, good luck next time :)");
	printw("\npress any key to close the program");
	getch();
}

int main()
{
	//curses initialization
	initscr();
	noecho();
	raw();
	nonl();
	keypad(stdscr, TRUE);
	//colors initialization
	start_color();
	//game initialization
	start_game();
	//*TEST*/spawn_e(6,maxy/2+5,"slime");
	i_spawn_ms();
	print_vision();
	//main loop
	while(true){
		//input
		com=getch();
		//spawn monsters
		///spawn_ms();
		//adv. stuff
		adv_move();
		//monster stuff (WIP)
		mns_move();
		//board stuff
		board_updt();
		//output
		print_vision();
		//check if the adv. is dead
		if(chk_death()==true){
			break;
		}
	}
	end();
	endwin();
	return 0;
}
