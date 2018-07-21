/*
Programmatore       : Stefano Montesi
Programma           : "Dungeon" (il nome cambiera' eventualmente)
Descrizione         : Un piccolo giochino stile "rough" fortemente in fase beta (:D)
Ultima modifica     : 6/16/2013 - 9:43 PM
*/

#include <stdio.h>
#include <stdlib.h>  //for rand() & srand()
#include <time.h>    //for srand(time(NULL))
#include <curses.h>


int Ir,j,k,x,y,x2,y2,vx,vy,ax,ay,mx,my;
float aAtt,aHP,aMHP,aReg;
float Fr;
char com;
int mvx=32; //max=78 || max=maxx //def.=32 //HAS TO BE EVEN
int mvy=17; //max=20 || max=maxy //def.=17
int maxx=100;
int maxy=100;
char ground[200][200];
char item  [200][200];
char life  [200][200];
float  status[200][200][200];

//game modes (for debugging and easter eggs :D)
bool M_ghost=false;  //the player turns into a ghost and continues playing with negative HP instead of dying when his HP reach 0
bool M_zombie=false; //when the player's HP reach 0, he revives as a zombie (mHP/2;att*2;reg=0)

//status[x][y][ps_rs] - "Role Status" the role that the entity is playing
int rs_adv=1;
int rs_ene=2;
int rs_fri=3;

//status[x][y][n] - "Position Status" the position in the third dimension of the status array in which to strore what (n)
short maxst=10;
short ps_clr=0;
short ps_cpp=1;
short ps_rs =2; //role status
short ps_HP =3; //hit point (health)
short ps_mHP=4; //maximum hit points
short ps_att=5; //phisical attack
short ps_reg=6;
short ps_vis=7; //length of vision range
short ps_mov=8; //movement points
short ps_vel=9; //ammount of movement points gained each game-phase

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

//Life (entities)
char l_adv='@';
char l_slime='s';
char l_creep='c';
char l_masti='m';

char l_zmbAdv='Z';

//color pairs positions
short pp_def=1;
short pp_adv=2;
short pp_slime=3;
short pp_creep=4;
short pp_masti=5;
short pp_tree=6;

//colors
short cl_def=COLOR_WHITE;
short cl_adv=COLOR_WHITE;
short cl_slime=COLOR_CYAN;
short cl_creep=COLOR_YELLOW;
short cl_tree=COLOR_GREEN;
short cl_masti=COLOR_RED;

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

float FrandRng(float min, float max){
	float r = min + (float)rand()/((float)RAND_MAX/(max-min));
	return r;
}

int IrandRng(int min,int max){
	int r = min + (int)rand()/((int)RAND_MAX/(max-min));
	return r;
}

void setClr(int xx,int yy,short n){
	status[xx][yy][ps_clr]=n;
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

void spawn(int xx,int yy/*char g,const char* name*/,char g,short clr,short cpp,int rs,float att,float hp,float mhp,float reg,float mov,float vel,int vis){
	life[xx][yy]=g;
	setClr(xx,yy,clr);
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
	short sC,sCpp;
	if(ent=="adv" || ent=="adventurer"){
		sRS  =rs_adv;
		sMHP =aMHP;
		sHP  =aHP;
		sReg =aReg;
		sAtt =aAtt;
		sVis =t_null;
		sMov =t_null;
		sVel =t_null;
		sG   =l_adv;
		sC   =cl_adv;
		sCpp =pp_adv;
	}
	if(ent=="slime"){
		sRS  =rs_ene;
		sMHP =FrandRng(3.0,8.0);
		sHP  =sMHP;
		sReg =FrandRng(0.3,0.5);
		sAtt =IrandRng(1.0,3.0);
		sVis =IrandRng(20.0,40.0);
		sVel =1.0;
		sMov =sVel;
		sG   =l_slime;
		sC   =cl_slime;
		sCpp =pp_slime;
	}
	else if(ent=="creep" || ent=="creeper"){
		sRS  =rs_ene;
		sMHP  =FrandRng(5.0,12.0);
		sHP  =sMHP;
		sReg =FrandRng(0.0,0.2);
		sAtt =IrandRng(2.0,5.0);
		sVis =IrandRng(20.0,40.0);
		sVel =1.0;
		sMov =sVel;
		sG   =l_creep;
		sC   =cl_creep;
		sCpp =pp_creep;
	}
	else if(ent=="masti" || ent=="mastichino"){
		sRS  =rs_ene;
		sMHP  =FrandRng(4.0,7.0);
		sHP  =sMHP;
		sReg =FrandRng(0.1,0.3);
		sAtt =IrandRng(2.0,4.0);
		sVis =IrandRng(20.0,40.0);
		sVel =FrandRng(1.2,1.5);
		sMov =sVel;
		sG   =l_masti;
		sC   =cl_masti;
		sCpp =pp_masti;
	}
	spawn(xx,yy,sG,sC,sCpp,sRS,sAtt,sHP,sMHP,sReg,sMov,sVel,sVis);
}

/*void despawn(int xx,int yy){
	life[xx][yy]=t_null;
    for (j=0;j<maxst;j++) status[xx][yy][j]=t_null;
}*/

void despawn(int xx,int yy){
	life[xx][yy]=t_null;
	status[xx][yy][ps_clr]=t_null;
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
			life  [x][y]=t_null;
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
				aAtt=3.0;
				aHP =30.0;
				aMHP=30.0;
				aReg=0.2;
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
			if(ground[x][y]==g_light && life[x][y]==t_null){
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
			if(ground[x][y]==g_light && life[x][y]==t_null){
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
	//regenerate hit points
	//despawn life forms with HP below or equal to 0
	//recharge MP(move points) to all lifeforms on board
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			if(life[x][y]!=t_null){
				status[x][y][ps_HP]+=status[x][y][ps_reg];
				if(status[x][y][ps_HP]>status[x][y][ps_mHP]) status[x][y][ps_HP]=status[x][y][ps_mHP];
				if(status[x][y][ps_rs]!=rs_adv){
					status[x][y][ps_mov]+=status[x][y][ps_vel];
				}
				if(status[x][y][ps_HP]<1 && status[x][y][ps_rs]!=rs_adv) despawn(x,y);
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
			//set color
			init_pair(status[vx-x][vy-y][ps_cpp],status[vx-x][vy-y][ps_clr],COLOR_BLACK);
			attron(COLOR_PAIR(status[vx-x][vy-y][ps_cpp]));
			//print game-screen
			if(life[vx-x][vy-y]!=t_null) mvaddch(y,x,life[vx-x][vy-y]);
			else if(item[vx-x][vy-y]!=t_null) mvaddch(y,x,item[vx-x][vy-y]);
			else mvaddch(y,x,ground[vx-x][vy-y]);
			//reset default color
			init_pair(cl_def,COLOR_WHITE,COLOR_BLACK);
			attron(COLOR_PAIR(cl_def));
			//print monsters and items inside vision range(WIP)
			if(status[vx-x][vy-y][ps_rs]==rs_ene){
				attron(COLOR_PAIR(status[vx-x][vy-y][ps_cpp]));
				mvprintw(7+mnsN,38,"%c - ATT:%.1f HP :%.1f VEL:%.1f REG:%.1f\t",life[vx-x][vy-y],status[vx-x][vy-y][ps_att],status[vx-x][vy-y][ps_HP],status[vx-x][vy-y][ps_vel],status[vx-x][vy-y][ps_reg]);
				attron(COLOR_PAIR(cl_def));
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
	float mAtt,mHP,mMHP,mReg,mMov,mVel,mVis,mG;
	short mC,mCpp;
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			if(life[x][y]!=t_null && status[x][y][ps_rs]==rs_ene && status[x][y][ps_mov]>=1){
				mnx=x;
				mny=y;
				for(;status[mnx][mny][ps_mov]>=1;status[mnx][mny][ps_mov]--){
					//*DEBUG*/mvprintw(12,40,"%.1f",status[mnx][mny][ps_mov]);
					//*DEBUG*/refresh();
					//*DEBUG*/getch();
					mG  =life[mnx][mny];
					mC  =status[mnx][mny][ps_clr];
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
					spawn(ox,oy,mG,mC,mCpp,rs_ene,mAtt,mHP,mMHP,mReg,mMov,mVel,mVis);
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
	aAtt=status[ax][ay][ps_att];
	aHP =status[ax][ay][ps_HP];
	aMHP=status[ax][ay][ps_mHP];
	aReg=status[ax][ay][ps_reg];
	mx=ax;
	my=ay;
	if(com==c_N){
		//change adv. coos
		ay++;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && life[ax][ay]==t_null) || (life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_fri)){
			if (ay<maxy-mvy/2 && ay>mvy/2) vy++;
		}
	}
	if(com==c_NE){
		//change adv. coos
		ay++;
		ax++;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && life[ax][ay]==t_null) || (life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_fri)){
			if(ay<maxy-mvy/2 && ay>mvy/2) vy++;
			if(ax<maxx-mvx/2 && ax>mvx/2-1) vx++;
		}
	}
	if(com==c_E){
		//change adv. coos
		ax++;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && life[ax][ay]==t_null) || (life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_fri)){
			if(ax<maxx-mvx/2 && ax>mvx/2-1) vx++;
		}
	}
	if(com==c_SE){
		//change adv. coos
		ay--;
		ax++;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && life[ax][ay]==t_null) || (life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_fri)){
			if(ay>mvy/2-1 && ay<maxy-mvy/2-1) vy--;
			if(ax<maxx-mvx/2 && ax>mvx/2-1) vx++;
		}
	}
	if(com==c_S){
		//change adv. coos
		ay--;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && life[ax][ay]==t_null) || (life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_fri)){
			if(ay>mvy/2-1 && ay<maxy-mvy/2-1) vy--;
		}
	}
	if(com==c_SO){
		//change adv. coos
		ay--;
		ax--;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && life[ax][ay]==t_null) || (life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_fri)){
			if(ay>mvy/2-1 && ay<maxy-mvy/2-1) vy--;
			if(ax>mvx/2-2 && ax<maxx-mvx/2-1) vx--;
		}
	}
	if(com==c_O){
		//change adv. coos
		ax--;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && life[ax][ay]==t_null) || (life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_fri)){
			if(ax>mvx/2-2 && ax<maxx-mvx/2-1) vx--;
		}
	}
	if(com==c_NO){
		//change adv. coos
		ay++;
		ax--;
		//move adv. vision & check if adv. can move
		if((ground[ax][ay]==g_light && life[ax][ay]==t_null) || (life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_fri)){
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
	if(life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_ene){
		attk(mx,my,ax,ay);
	}
	if(ground[ax][ay]!=g_light || life[ax][ay]!=t_null){
		ax=mx;
		ay=my;
	}
	despawn(mx,my);
	spawn_m(ax,ay,"adv");
}

bool chk_death(){
	bool d=false;
	//ghost mode
	if(M_ghost==true){
		if(status[ax][ay][ps_HP]<1) l_adv='G';
		if(status[ax][ay][ps_HP]>=1) l_adv='@';
	}
	//zombie mode
	else if(M_zombie==true){
		if(status[ax][ay][ps_HP]<1){
			if(l_adv==l_zmbAdv) d=true;
			if(l_adv!=l_zmbAdv){
				l_adv=l_zmbAdv;
				molMHP(ax,ay,0.5);
				setHP(ax,ay,status[ax][ay][ps_mHP]);
				molAtt(ax,ay,2);
				setReg(ax,ay,0);
			}
		}
	}
	//default mode
	else{
		if(status[ax][ay][ps_HP]<1) d=true;
	}
	//return statement
	return d;
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
		if(chk_death()==true) break;
	}
	end();
	endwin();
	return 0;
}
