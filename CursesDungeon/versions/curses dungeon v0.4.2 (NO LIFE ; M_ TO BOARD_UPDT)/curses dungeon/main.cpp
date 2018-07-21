/*
Programmatore       : Stefano Montesi
Programma           : "Dungeon" (il nome cambiera' eventualmente)
Descrizione         : Un piccolo giochino stile "rough" fortemente in fase beta (:D)
Ultima modifica     : 6/23/2013 - 11:27 PM
*/

#include <stdio.h>
#include <stdlib.h>  //for rand() & srand()
#include <time.h>    //for srand(time(NULL))
#include <curses.h>


int Ir,j,k,x,y,x2,y2,vx,vy,ax,ay,mx,my;
float aAtt,aHP,aMHP,aReg,aMov,aVel,aVis,aG,aRs;
short aFcl,aBcl,aCpp;
float Fr;
char com;
int mvx=32; //max=78 || max=maxx //def.=32 //HAS TO BE EVEN
int mvy=17; //max=20 || max=maxy //def.=17
int maxx=100;
int maxy=100;
char ground[200][200];
char item  [200][200];
float  status[200][200][200];

//game modes (for debugging and easter eggs :D)
bool M_ghost=false;  //the player turns into a ghost and continues playing with negative HP instead of dying when his HP reach 0
bool M_zombie=true; //when the player's HP reach 0, he revives as a zombie (mHP/2;att*2;reg=0)

//status[x][y][ps_rs] - "Role Status" the role that the entity is playing
int rs_adv=1;
int rs_ene=2;
int rs_fri=3;

//status[x][y][n] - "Position Status" the position in the third dimension of the status array in which to strore what (n)
short maxst=11;
short ps_G  =0;
short ps_fcl=1;  //character (front) color
short ps_bcl=2;  //background color
short ps_cpp=3;  //color pairs pointer
short ps_rs =4;  //role status
short ps_HP =5;  //hit point (health)
short ps_mHP=6;  //maximum hit points
short ps_att=7;  //phisical attack
short ps_reg=8;  //HP regenerated each turn
short ps_vis=9;  //length of vision range
short ps_mov=10; //movement points
short ps_vel=11; //ammount of movement points gained each game-phase

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

//Graphics
char g_light='.';
char g_wall =/*177*/ '#';
char g_tree ='T';

//color pairs positions
short cpp_def=1;

//character colors
short fcl_def=COLOR_WHITE;

//background colors
short bcl_def=COLOR_BLACK;


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

void setFcl(int xx,int yy,short n){
	status[xx][yy][ps_fcl]=n;
}

void setBcl(int xx,int yy,short n){
	status[xx][yy][ps_bcl]=n;
}

void setCpp(int xx,int yy,short n){
	status[xx][yy][ps_cpp]=n;
}

void setRS (int xx,int yy,int n){
	status[xx][yy][ps_rs]=n;
}

void setAtt(int xx,int yy,float n){
	status[xx][yy][ps_att]=n;
}

void chaAtt(int xx,int yy,float n){
	status[xx][yy][ps_att]+=n;
}

void molAtt(int xx,int yy,float n){
	status[xx][yy][ps_att]*=n;
}

void setHP(int xx,int yy,float n){
	status[xx][yy][ps_HP]=n;
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

void chaMHP(int xx,int yy,float n){
	status[xx][yy][ps_mHP]+=n;
}

void molMHP(int xx,int yy,float n){
	status[xx][yy][ps_mHP]*=n;
}

void setReg(int xx,int yy,float n){
	status[xx][yy][ps_reg]=n;
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

void chaVis(int xx,int yy,int n){
	status[xx][yy][ps_vis]+=n;
}

void molVid(int xx,int yy,float n){
	status[xx][yy][ps_vis]*=n;
}

void setMov(int xx,int yy,float n){
	status[xx][yy][ps_mov]=n;
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

void chaVel(int xx,int yy,float n){
	status[xx][yy][ps_vel]+=n;
}

void molVel(int xx,int yy,float n){
	status[xx][yy][ps_vel]*=n;
}

void spawn(int xx,int yy/*char g,const char* name*/,char g,short fcl,short bcl,short cpp,int rs,float att,float hp,float mhp,float reg,float mov,float vel,int vis){
	setG(xx,yy,g);
	setFcl(xx,yy,fcl);
	setBcl(xx,yy,bcl);
	setCpp(xx,yy,cpp);
	setRS(xx,yy,rs);
	setHP(xx,yy,hp);
	setMHP(xx,yy,mhp);
	setReg(xx,yy,reg);
	setAtt(xx,yy,att);
	setMov(xx,yy,mov);
	setVel(xx,yy,vel);
	setVis(xx,yy,vis);
}

void spawn_m(int xx,int yy,const char* ent/*ENTity to spawn*/){
	float sRS,sHP,sMHP,sReg,sAtt,sVis,sMov,sVel;
	char sG;
	short sFcl,sBcl,sCpp;
	if(ent=="adv" || ent=="adventurer"){
		sG   ='@';
		sFcl =COLOR_BLACK;
		sBcl =COLOR_WHITE;
		sCpp =2;
		sRS  =rs_adv;
		sMHP =30.0;
		sHP  =sMHP;
		sReg =0.2;
		sAtt =3.0;
		sVis =t_null;
		sMov =t_null;
		sVel =t_null;
	}
	if(ent=="slime"){
		sG   ='s';
		sFcl =COLOR_CYAN;
		sBcl =COLOR_BLACK;
		sCpp =3;
		sRS  =rs_ene;
		sMHP =FrandRng(3.0,8.0);
		sHP  =sMHP;
		sReg =FrandRng(0.3,0.5);
		sAtt =IrandRng(1.0,3.0);
		sVis =IrandRng(20.0,40.0);
		sVel =1.0;
		sMov =sVel;
	}
	else if(ent=="creep" || ent=="creeper"){
		sG   ='c';
		sFcl =COLOR_YELLOW;
		sBcl =COLOR_BLACK;
		sCpp =4;
		sRS  =rs_ene;
		sMHP =FrandRng(5.0,12.0);
		sHP  =sMHP;
		sReg =FrandRng(0.0,0.2);
		sAtt =IrandRng(2.0,5.0);
		sVis =IrandRng(20.0,40.0);
		sVel =1.0;
		sMov =sVel;
	}
	else if(ent=="masti" || ent=="mastichino"){
		sG   ='m';
		sFcl =COLOR_RED;
		sBcl =COLOR_BLACK;
		sCpp =5;
		sRS  =rs_ene;
		sMHP =FrandRng(4.0,7.0);
		sHP  =sMHP;
		sReg =FrandRng(0.1,0.3);
		sAtt =IrandRng(2.0,4.0);
		sVis =IrandRng(20.0,40.0);
		sVel =FrandRng(1.2,1.5);
		sMov =sVel;
	}
	spawn(xx,yy,sG,sFcl,sBcl,sCpp,sRS,sAtt,sHP,sMHP,sReg,sMov,sVel,sVis);
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
    status[xx][yy][ps_mov]=t_null;
    status[xx][yy][ps_vel]=t_null;
    status[xx][yy][ps_vis]=t_null;
}

/*//TEST: put maxx = mxv & maxy = mxy
void generate_g(){
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			ground[x][y]=g_light;
			if(x==maxy/2 && y!=15) ground[x][y]=g_wall;
			//default
			if(x==maxx-1 || x==0 || y==maxy-1 || y==0) ground[x][y]=g_wall;
		}
	}
}/**/

void generate_g(){
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			Ir=rand()%1000;
			if(Ir<750) ground[x][y]=g_light;
			if(Ir>=750 && Ir<985) ground[x][y]=g_wall;
			if(Ir>=985) ground[x][y]=g_tree;
			//default
			if(x==maxx-1 || x==0 || y==maxy-1 || y==0) ground[x][y]=g_wall;
		}
	}
}

void start_game(){
	//start random seed
	srand(time(NULL));
	//reset
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			ground[x][y]=t_null;
			item  [x][y]=t_null;
			//reset status
			for(j=0;j<maxst;j++) status[x][y][j]=t_null;
		}
	}
	generate_g();
	//spawn character
	j=0;
	for(y=maxy/2;y>=0;y--){
		for(x=maxx/2;x>=0;x--){
			if(ground[x][y]==g_light){
				spawn_m(x,y,"adv");
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
			if(ground[x][y]==g_light && status[x][y][ps_rs]==t_null){
				if(Ir==0){
					Ir=rand()%100;
                	if(Ir<=45) spawn_m(x,y,"slime");
                	if(Ir>45 && Ir<=75) spawn_m(x,y,"masti");
					if(Ir> 75) spawn_m(x,y,"creeper");
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
			if(ground[x][y]==g_light && status[x][y][ps_rs]==t_null){
				if(Fr==0.00000){
					Ir=rand()%100;
                	if(Ir<=40) spawn_m(x,y,"slime");
                	if(Ir>40 && Ir<=75) spawn_m(x,y,"masti");
					if(Ir> 75) spawn_m(x,y,"creeper");
				}
			}
		}
	}
}

void board_updt(){
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			if(lifeChk(x,y)==true){
				//**ADVENTURER AND MONSTERS**//
				//regenerate HP
				status[x][y][ps_HP]+=status[x][y][ps_reg];
				//keep HP lower than mHP
				if(status[x][y][ps_HP]>status[x][y][ps_mHP]) status[x][y][ps_HP]=status[x][y][ps_mHP];
				//**MONSTERS ONLY**//
				if(status[x][y][ps_rs]!=rs_adv){
					//regenerate mov
					status[x][y][ps_mov]+=status[x][y][ps_vel];
					//despawn monsters when their HP reahces 0
					if(status[x][y][ps_HP]<1 && status[x][y][ps_rs]!=rs_adv) despawn(x,y);
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
	int mnsN=0;
	move(0,0);
	//erase side comments
	for(y=17;y>6;y--){
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
			else if(item[vx-x][vy-y]!=t_null) mvaddch(y,x,item[vx-x][vy-y]);
			else mvaddch(y,x,ground[vx-x][vy-y]);
			//reset default color
			init_pair(cpp_def,fcl_def,bcl_def);
			attron(COLOR_PAIR(cpp_def));
			//print monsters and items inside vision range(WIP)
			if(status[vx-x][vy-y][ps_rs]==rs_ene){
				attron(COLOR_PAIR(status[vx-x][vy-y][ps_cpp]));
				mvprintw(7+mnsN,38,"%c - ATT:%.1f HP :%.1f VEL:%.1f REG:%.1f\t",(char)status[vx-x][vy-y][ps_G],status[vx-x][vy-y][ps_att],status[vx-x][vy-y][ps_HP],status[vx-x][vy-y][ps_vel],status[vx-x][vy-y][ps_reg]);
				attron(COLOR_PAIR(cpp_def));
				mnsN++;
			}
		}
	}
	//print adv attributes
	print_attr("ATT",status[ax][ay][ps_att],1,0);
	print_attr("HP ",status[ax][ay][ps_HP],2,0);
	print_attr("REG",status[ax][ay][ps_reg],3,0);
	print_attr("mHP",status[ax][ay][ps_mHP],2,1);
	//*--DEBUG--*/print_attr("xAT",status[ax][ay+1][ps_att],1,1);
	//*--DEBUG--*/print_attr("xHP",status[ax][ay+1][ps_HP],2,1);
	//print monsters and items on the screen
	//print comments (WIP)
	mvprintw(19,0,"coos:  %d;%d\n\nlast com: %c (%d)",ax,ay,com,com);
	refresh();
}

void attk(int atx,int aty,int dex,int dey){
	//WIP
	chaHP(dex,dey,-status[atx][aty][ps_att]);
}

void mns_move(){
	bool advfChk=false;
	int movx,movy,ox,oy,mnx,mny;
	float mAtt,mHP,mMHP,mReg,mMov,mVel,mVis,mG,mRs;
	short mFcl,mBcl,mCpp;
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			if(lifeChk(x,y)==true && status[x][y][ps_rs]==rs_ene && status[x][y][ps_mov]>=1){
				mnx=x;
				mny=y;
				for(;status[mnx][mny][ps_mov]>=1;status[mnx][mny][ps_mov]--){
					//*DEBUG*/mvprintw(12,40,"%.1f",status[mnx][mny][ps_mov]);
					//*DEBUG*/refresh();
					//*DEBUG*/getch();
					mG  =status[mnx][mny][ps_G];
					mRs =status[mnx][mny][ps_rs];
					mFcl=status[mnx][mny][ps_fcl];
					mBcl=status[mnx][mny][ps_bcl];
					mCpp=status[mnx][mny][ps_cpp];
					mAtt=status[mnx][mny][ps_att];
					mHP =status[mnx][mny][ps_HP];
					mMHP=status[mnx][mny][ps_mHP];
					mReg=status[mnx][mny][ps_reg];
					mVis=status[mnx][mny][ps_vis];
					mVel=status[mnx][mny][ps_vel];
					for(y2=mny+mVis/2;(y2>=mny-mVis/2 && y2>0);y2--){
						for(x2=mnx+mVis/2;(x2>=mnx-mVis/2 && x2>0);x2--){
							if(status[x2][y2][ps_rs]==rs_adv || status[x2][y2][ps_rs]==rs_fri){
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
					if(ground[ox][oy]!=g_light || status[ox][oy][ps_rs]==rs_ene){
						for(j=0;((ground[ox][oy]!=g_light || status[ox][oy][ps_rs]==rs_ene) && j<8);j++){
							if(ox<mnx) oy--;
							if(ox>mnx) oy++;
							if(oy<mny) ox++;
							if(oy>mny) ox--;
						}
					}
					if(status[ox][oy][ps_rs]==rs_adv || status[ox][oy][ps_rs]==rs_fri){
						//*DEBUG*/mvprintw(11,40,"adv (%d,%d) attacked by %c (%d,%d)",ox,oy,mG,mnx,mny);
						//*DEBUG*/mvprintw(12,40,"adv current life: %d",status[ox][oy][ps_HP]);
						//*DEBUG*/refresh();
						//*DEBUG*/getch();
						attk(mnx,mny,ox,oy);
						ox=mnx;
						oy=mny;
					}
					mMov=status[mnx][mny][ps_mov];
					despawn(mnx,mny);
					spawn(ox,oy,mG,mFcl,mBcl,mCpp,rs_ene,mAtt,mHP,mMHP,mReg,mMov,mVel,mVis);
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
	aG  =status[ax][ay][ps_G];
	aRs =status[ax][ay][ps_rs];
	aFcl=status[ax][ay][ps_fcl];
	aBcl=status[ax][ay][ps_bcl];
	aCpp=status[ax][ay][ps_cpp];
	aAtt=status[ax][ay][ps_att];
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
		if((ground[ax][ay]==g_light && lifeChk(ax,ay)==false) || (lifeChk(ax,ay)==true && status[ax][ay][ps_rs]==rs_fri)){
			if (ay<maxy-mvy/2 && ay>mvy/2) vy++;
		}
	}
	if(com==c_NE){
		//change adv. coos
		ay++;
		ax++;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && lifeChk(ax,ay)==false) || (lifeChk(ax,ay)==true && status[ax][ay][ps_rs]==rs_fri)){
			if(ay<maxy-mvy/2 && ay>mvy/2) vy++;
			if(ax<maxx-mvx/2 && ax>mvx/2-1) vx++;
		}
	}
	if(com==c_E){
		//change adv. coos
		ax++;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && lifeChk(ax,ay)==false) || (lifeChk(ax,ay)==true && status[ax][ay][ps_rs]==rs_fri)){
			if(ax<maxx-mvx/2 && ax>mvx/2-1) vx++;
		}
	}
	if(com==c_SE){
		//change adv. coos
		ay--;
		ax++;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && lifeChk(ax,ay)==false) || (lifeChk(ax,ay)==true && status[ax][ay][ps_rs]==rs_fri)){
			if(ay>mvy/2-1 && ay<maxy-mvy/2-1) vy--;
			if(ax<maxx-mvx/2 && ax>mvx/2-1) vx++;
		}
	}
	if(com==c_S){
		//change adv. coos
		ay--;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && lifeChk(ax,ay)==false) || (lifeChk(ax,ay)==true && status[ax][ay][ps_rs]==rs_fri)){
			if(ay>mvy/2-1 && ay<maxy-mvy/2-1) vy--;
		}
	}
	if(com==c_SO){
		//change adv. coos
		ay--;
		ax--;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && lifeChk(ax,ay)==false) || (lifeChk(ax,ay)==true && status[ax][ay][ps_rs]==rs_fri)){
			if(ay>mvy/2-1 && ay<maxy-mvy/2-1) vy--;
			if(ax>mvx/2-2 && ax<maxx-mvx/2-1) vx--;
		}
	}
	if(com==c_O){
		//change adv. coos
		ax--;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && lifeChk(ax,ay)==false) || (lifeChk(ax,ay)==true && status[ax][ay][ps_rs]==rs_fri)){
			if(ax>mvx/2-2 && ax<maxx-mvx/2-1) vx--;
		}
	}
	if(com==c_NO){
		//change adv. coos
		ay++;
		ax--;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && lifeChk(ax,ay)==false) || (lifeChk(ax,ay)==true && status[ax][ay][ps_rs]==rs_fri)){
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
	if(lifeChk(ax,ay)==true && status[ax][ay][ps_rs]==rs_ene){
		attk(mx,my,ax,ay);
	}
	if(ground[ax][ay]!=g_light || lifeChk(ax,ay)==true){
		ax=mx;
		ay=my;
	}
	despawn(mx,my);
	spawn(ax,ay,aG,aFcl,aBcl,aCpp,aRs,aAtt,aHP,aMHP,aReg,aMov,aVel,aVis);
}

bool chk_death(){
	if(M_ghost==false && status[ax][ay][ps_HP]<1) return true;
	else return false;
}

void end(){
	system("cls");
	mvprintw(0,0,"you lost, good luck next time :)\npress any key to close the program");
	getch();
}

int main()
{
	//curses initialization
	initscr();
	noecho();
	raw();
	keypad(stdscr, TRUE);
	//colors initialization
	start_color();
	//game initialization
	start_game();
	//*TEST*/spawn_m(8,maxy/2,"masti");
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
			getch();
			break;
		}
	}
	end();
	endwin();
	return 0;
}
