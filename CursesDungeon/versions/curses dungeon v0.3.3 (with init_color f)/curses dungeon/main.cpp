/*
Programmatore       : Stefano Montesi
Programma           : "Dungeon" (il nome cambiera' eventualmente)
Descrizione         : Un piccolo giochino stile "rough" fortemente in fase beta (:D)
Ultima modifica     : 6/16/2013 - 9:43 PM
*/

#include <stdio.h>
#include <stdarg.h>  //for functions with dynamic arguments
#include <stdlib.h>  //for rand() & srand()
#include <time.h>    //for srand(time(NULL))
#include <curses.h>


int Ir,j,k,x,y,x2,y2,vx,vy,ax,ay,mx,my;
int aAtt,aHP,aReg;
float Fr;
char com;
int mvx=32; //max=78 || max=maxx //def.=32 //HAS TO BE EVEN
int mvy=17; //max=20 || max=maxy //def.=17
int maxx=100;
int maxy=100;
char ground[100][100];
char item  [100][100];
char life  [100][100];
short color[100][100][4];
float  status[100][100][100];

//status[x][y][ps_rs] - "Role Status" the role that the entity is playing
int rs_adv=1;
int rs_ene=2;
int rs_fri=3;

//status[x][y][n] - "Position Status" the position in the third dimension of the status array in which to strore what (n)
short maxst=6;
short ps_rs =0; //role status
short ps_HP =1; //hit point (health)
short ps_att=2; //phisical attack
short ps_reg=3;
short ps_vis=4; //length of vision range
short ps_mov=5; //movement points
short ps_vel=6; //ammount of movement points gained each game-phase

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

//colors
short cl_def=1;
short cl_adv[4]={11,255,255,255};
short cl_slime[4]={12,50,205,50};
short cl_creep[4]={13,0,205,102};
short cl_tree[4]={14,0,255,0};
short cl_masti[4]={15,142,35,35};

void print_attr(const char* name,int n,int row,int col){
	//a f. that prints an ATTRibute
	move(row,40+(col*15));
	addstr(name);
	printw(" =");
	if(n<10) printw("   ");
	if(n>=10 && n<100) printw("  ");
	if(n>=100 && n<1000) printw(" ");
	printw("%d",n);
}

float FrandRng(float min, float max){
	float r = min + (float)rand()/((float)RAND_MAX/(max-min));
	return r;
}

int IrandRng(int min,int max){
	int r = min + (int)rand()/((int)RAND_MAX/(max-min));
	return r;
}

void setClr(int xx,int yy,short n[4]){
	for(j=0;j<4;j++){
		color[xx][yy][j]=n[j];
	}
}

void setRS (int xx,int yy,int n){
	status[xx][yy][ps_rs]=n;
}

void setAtt(int xx,int yy,float n){
	status[xx][yy][ps_att]=n;
}

void chaAtt(int xx,int yy,float n){
	status[xx][yy][ps_att]=status[xx][yy][ps_att]+n;
}

void setHP(int xx,int yy,float n){
	status[xx][yy][ps_HP]=n;
}

void chaHP(int xx,int yy,float n){
	status[xx][yy][ps_HP]=status[xx][yy][ps_HP]+n;
}

void setReg(int xx,int yy,float n){
	status[xx][yy][ps_reg]=n;
}

void chaReg(int xx,int yy,float n){
	status[xx][yy][ps_reg]=status[xx][yy][ps_reg]+n;
}

void setVis(int xx,int yy,int n){
	status[xx][yy][ps_vis]=n;
}

void chaVis(int xx,int yy,int n){
	status[xx][yy][ps_vis]=status[xx][yy][ps_vis]+n;
}

void setMov(int xx,int yy,float n){
	status[xx][yy][ps_mov]=n;
}

void chaMov(int xx,int yy,float n){
	status[xx][yy][ps_mov]=status[xx][yy][ps_mov]+n;
}

void setVel(int xx,int yy,float n){
	status[xx][yy][ps_vel]=n;
}

void chaVel(int xx,int yy,float n){
	status[xx][yy][ps_vel]=status[xx][yy][ps_vel]+n;
}

void spawn(int xx,int yy/*char g,const char* name*/,char g,short clr[4],int rs,float att,float hp,float reg,float mov,float vel,int vis){
	life[xx][yy]=g;
	setClr(xx,yy,clr);
	setRS(xx,yy,rs);
	setHP(xx,yy,hp);
	setReg(xx,yy,reg);
	setAtt(xx,yy,att);
	setMov(xx,yy,mov);
	setVel(xx,yy,vel);
	setVis(xx,yy,vis);
}

void spawn_m(int xx,int yy,const char* ent/*ENTity to spawn*/){
	float sRS,sHP,sReg,sAtt,sVis,sMov,sVel;
	char sG;
	short sC[4];
	if(ent=="adv" || ent=="adventurer"){
		sRS  =rs_adv;
		sHP  =aHP;
		sReg =aReg;
		sAtt =aAtt;
		sVis =t_null;
		sMov =t_null;
		sVel =t_null;
		sG   =l_adv;
		for(j=0;j<4;j++){
			sC[j]=cl_adv[j];
		}
	}
	if(ent=="slime"){
		sRS  =rs_ene;
		sHP  =FrandRng(3.0,8.0);
		sReg =FrandRng(0.3,0.5);
		sAtt =IrandRng(1.0,3.0);
		sVis =IrandRng(20.0,40.0);
		sMov =1.0;
		sVel =1.0;
		sG   =l_slime;
		for(j=0;j<4;j++){
			sC[j]=cl_slime[j];
		}
	}
	else if(ent=="creep" || ent=="creeper"){
		sRS  =rs_ene;
		sHP  =FrandRng(5.0,12.0);
		sReg =FrandRng(0.0,0.2);
		sAtt =IrandRng(2.0,5.0);
		sVis =IrandRng(20.0,40.0);
		sMov =1.0;
		sVel =1.0;
		sG   =l_creep;
		for(j=0;j<4;j++){
			sC[j]=cl_creep[j];
		}
	}
	else if(ent=="masti" || ent=="mastichino"){
		sRS  =rs_ene;
		sHP  =FrandRng(4.0,7.0);
		sReg =FrandRng(0.1,0.3);
		sAtt =IrandRng(2.0,4.0);
		sVis =IrandRng(20.0,40.0);
		sVel =FrandRng(1.2,1.5);
		sMov =sVel;
		sG   =l_masti;
		for(j=0;j<4;j++){
			sC[j]=cl_masti[j];
		}
	}
	spawn(xx,yy,sG,sC,sRS,sAtt,sHP,sReg,sMov,sVel,sVis);
}

/*void despawn(int xx,int yy){
	life[xx][yy]=t_null;
    for (j=0;j<maxst;j++) status[xx][yy][j]=t_null;
}*/

void despawn(int xx,int yy){
	life[xx][yy]=t_null;
	for(j=0;j<4;j++){
		color[xx][yy][j]=cl_def;
	}
    status[xx][yy][ps_rs]=t_null;
    status[xx][yy][ps_HP]=t_null;
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
			if(ground[x][y]==g_light){
				if(Ir==0){
					Ir=rand()%100;
                	if(Ir<=40) spawn_m(x,y,"slime");
                	if(Ir>40 && Ir<=75) spawn_m(x,y,"masti");
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
	//despawn life forms with HP below or equal to 0
	//recharge MP(move points) to all lifeforms on board
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			if(life[x][y]!=t_null){
				if(status[x][y][ps_rs]!=rs_adv){
					status[x][y][ps_mov]+=status[x][y][ps_vel];
				}
				if(status[x][y][ps_HP]<1) despawn(x,y);
			}
		}
	}
}

void print_vision(){
	int mnsN=0;
	move(0,0);
	//erase side comments
	for(y=17;y>7;y--){
		for(x=38;x<100;x++){
			mvprintw(y,x," ");
		}
	}
	//print board
   	for(y=0;y<mvy;y++){
		for(x=0;x<mvx;x++){
			//set color
			init_color(color[vx-x][vy-y][0],color[vx-x][vy-y][1],color[vx-x][vy-y][2],color[vx-x][vy-y][3]);
			init_pair(color[vx-x][vy-y][0],color[vx-x][vy-y][0],COLOR_BLACK);
			attron(COLOR_PAIR(color[vx-x][vy-y][0]));
			//print game-screen
			if(life[vx-x][vy-y]!=t_null) mvaddch(y,x,life[vx-x][vy-y]);
			else if(item[vx-x][vy-y]!=t_null) mvaddch(y,x,item[vx-x][vy-y]);
			else mvaddch(y,x,ground[vx-x][vy-y]);
			//reset default color
			init_pair(cl_def,COLOR_WHITE,COLOR_BLACK);
			attron(COLOR_PAIR(cl_def));
			//print monsters and items inside vision range(WIP)
			if(status[vx-x][vy-y][ps_rs]==rs_ene){
				mvprintw(7+mnsN,38,"%c - ATT:%.1f HP :%.1f VIS:%.1f MOV:%.1f\t",life[vx-x][vy-y],status[vx-x][vy-y][ps_att],status[vx-x][vy-y][ps_HP],status[vx-x][vy-y][ps_vis],status[vx-x][vy-y][ps_mov]);
				mnsN++;
			}
		}
	}
	//print adv attributes
	print_attr("ATT",status[ax][ay][ps_att],1,0);
	print_attr("xAT",status[ax][ay+1][ps_att],1,1);
	print_attr("HP ",status[ax][ay][ps_HP],2,0);
	print_attr("xHP",status[ax][ay+1][ps_HP],2,1);
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
	float mAtt,mHP,mReg,mMov,mVel,mVis,mG;
	short mC[4];
	for(y=0;y<mvy;y++){
		for(x=0;x<mvx;x++){
			if(life[vx-x][vy-y]!=t_null && status[vx-x][vy-y][ps_rs]==rs_ene && status[vx-x][vy-y][ps_mov]>=1){
				mnx=vx-x;
				mny=vy-y;
				for(;status[mnx][mny][ps_mov]>=1.0;status[mnx][mny][ps_mov]-=1.0){
					//*DEBUG*/mvprintw(12,40,"%.1f",status[mnx][mny][ps_mov]);
					//*DEBUG*/refresh();
					//*DEBUG*/getch();
					for(j=0;j<4;j++){
						mC[j]=color[mnx][mny][j];
					}
					mAtt=status[mnx][mny][ps_att];
					mHP =status[mnx][mny][ps_HP];
					mReg=status[mnx][mny][ps_reg];
					mVis=status[mnx][mny][ps_vis];
					mVel=status[mnx][mny][ps_vel];
					mG  =life[mnx][mny];
					for(y2=mny+mVis/2;(y2>=mny-mVis/2 && y2>0);y2--){
						for(x2=mnx+mVis/2;(x2>=mnx-mVis/2 && x2>0);x2--){
							if(status[x2][y2][ps_rs]==rs_adv || status[x2][y2][ps_rs]==rs_fri){
								advfChk=true;	
								//*DEBUG*/mvprintw(13,40,"adv detcted by %c at coos: %d,%d",mG,mnx,mny);
								//*DEBUG*/mvprintw(14,40,"adv is at coos: %d,%d",x2,y2);
								//*DEBUG*/refresh();
								//*DEBUG*/getch();
								movx=x2-mnx;
								movy=y2-mny;
							}
						}
					}
					if(advfChk==false){
						movx=IrandRng(mnx+mVis/2,mnx-mVis/2);
						movy=IrandRng(mny+mVis/2,mny-mVis/2);
					}
					advfChk=false;
					//calcolation of "ox and oy" (where the monster is going to move at)
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
					spawn(ox,oy,mG,mC,rs_ene,mAtt,mHP,mReg,mMov,mVel,mVis);
					mnx=ox;
					mny=oy;
					//*DEBUG*/move(15,40);
					//*DEBUG*/printw("spawned %c from %d,%d to %d,%d",mG,mnx,mny,ox,oy);
					//*DEBUG*/refresh();
					//*DEBUG*/getch();
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
	//check if there's an hostile "life" in the place that the adv. is trying to move in and attack it
	if(life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_ene){
		attk(mx,my,ax,ay);
	}
	if(ground[ax][ay]!=g_light || life[ax][ay]!=t_null){
		ax=mx;
		ay=my;
	}
	despawn(mx,my);
	spawn(ax,ay,l_adv,cl_adv,rs_adv,aAtt,aHP,aReg,t_null,t_null,t_null);
}

bool chk_death(){
	bool d=false;
	if(status[ax][ay][ps_HP]<1) d=true;
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
	/*init_color(1,500,0,500);
	init_pair(1,1,COLOR_BLACK);
	attron(COLOR_PAIR(1));*/
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
		spawn_ms();
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
