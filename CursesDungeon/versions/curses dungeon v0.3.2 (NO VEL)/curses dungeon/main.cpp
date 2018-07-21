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


int j,k,x,y,x2,y2,vx,vy,ax,ay,mx,my,xmill;
int aatt,ahp,areg;
char com;
int mvx=32; //max=78 || max=maxx //def.=32 //HAS TO BE EVEN
int mvy=17; //max=20 || max=maxy //def.=17
int maxx=100;
int maxy=100;
char ground[100][100];
char item  [100][100];
char life  [100][100];
int  status[100][100][5];

//status[x][y][ps_rs] - "Role Status" the role that the entity is playing
int rs_adv=1;
int rs_ene=2;
int rs_fri=3;

//status[x][y][n] - "Position Status" the position in the third dimension of the status array in which to strore what (n)
int maxst=6;
char ps_rs =0; //role status
char ps_HP =1; //hit point (health)
char ps_att=2; //phisical attack
char ps_vis=3; //length of vision range
char ps_mov=4; //movement points

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

//TEMPORARY CONTROLLS - or at least until I discover how to make curses read the default ones XD
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
char l_creeper='c';
char l_mastichino='m';

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

void spawn(int xx,int yy/*char g,const char* name*/,char g,int rs,float att,float hp,float mov,int vis){
	life[xx][yy]=g;
	setRS(xx,yy,rs);
	setHP(xx,yy,hp);
	setAtt(xx,yy,att);
	setMov(xx,yy,mov);
	setVis(xx,yy,vis);
}

void spawn_m(int xx,int yy,const char* ent/*ENTity to spawn*/){
	float sRS,sHP,sATT,sVIS,sMOV;
	char sG;
	if(ent=="adv" || ent=="adventurer"){
		sRS  =rs_adv;
		sHP  =ahp;
		sATT =aatt;
		sVIS =t_null;
		sMOV =t_null;
		sG   ='@';
	}
	if(ent=="slime"){
		sRS  =rs_ene;
		sHP  =FrandRng(3.0,8.0);
		sATT =FrandRng(1.0,3.0);
		sVIS =FrandRng(20.0,40.0);
		sMOV =1.0;
		sG   ='s';
	}
	else if(ent=="creeper"){
		sRS  =rs_ene;
		sHP  =FrandRng(5.0,12.0);
		sATT =FrandRng(2.0,5.0);
		sVIS =FrandRng(20.0,40.0);
		sMOV =1.0;
		sG   ='c';
	}
	else if(ent=="masti" || ent=="mastichino"){
		sRS  =rs_ene;
		sHP  =FrandRng(4.0,7.0);
		sATT =FrandRng(2.0,4.0);
		sVIS =FrandRng(20.0,40.0);
		sMOV =1.0;
		sG   ='m';
	}
	spawn(xx,yy,sG,sRS,sATT,sHP,sMOV,sVIS);
}

/*void despawn(int xx,int yy){
	life[xx][yy]=t_null;
    for (j=0;j<maxst;j++) status[xx][yy][j]=t_null;
}*/

void despawn(int xx,int yy){
	life[xx][yy]=t_null;
    status[xx][yy][ps_rs]=t_null;
    status[xx][yy][ps_HP]=t_null;
    status[xx][yy][ps_att]=t_null;
    status[xx][yy][ps_mov]=t_null;
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
			xmill=rand()%1000;
			if(xmill<750) ground[x][y]=g_light;
			if(xmill>=750 && xmill<985) ground[x][y]=g_wall;
			if(xmill>=985) ground[x][y]=g_tree;
			//default
			if(x==maxx-1 || x==0 || y==maxy-1 || y==0) ground[x][y]=g_wall;
		}
	}
}/**/

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
				aatt=3;
				ahp=20;
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
			xmill=rand()%250;
			if(ground[x][y]==g_light){
				if(xmill==0){
					xmill=rand()%100;
                	if(xmill<=40) spawn_m(x,y,"slime");
                	if(xmill>40 && xmill<=75) spawn_m(x,y,"masti");
					if(xmill> 75) spawn_m(x,y,"creeper");
				}
			}
		}
	}
}

void spawn_ms(){
	//spawns monsters (made to be used continusly)
   	for(y=0;y<mvy;y++){
		for(x=0;x<mvx;x++){
			xmill=rand()%1000000000000000;
			if(ground[vx-x][vy-y]==g_light){
				if(xmill==0){
					xmill=rand()%100;
                	if(xmill<=40) spawn_m(x,y,"slime");
                	if(xmill>40 && xmill<=75) spawn_m(x,y,"masti");
					if(xmill> 75) spawn_m(x,y,"creeper");
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
					status[x][y][ps_mov]++;
				}
				if(status[x][y][ps_HP]<=0) despawn(x,y);
			}
		}
	}
}

void print_vision(){
	int mnsN=0;
	//print board
	move(0,0);
	//erase side comments
   	for(y=0;y<mvy;y++){
		for(x=0;x<mvx;x++){
			//game-screen
			if(life[vx-x][vy-y]!=t_null) mvaddch(y,x,life[vx-x][vy-y]);
			else if(item[vx-x][vy-y]!=t_null) mvaddch(y,x,item[vx-x][vy-y]);
			else mvaddch(y,x,ground[vx-x][vy-y]);
			//print monsters and items inside vision range(WIP)
			if(status[vx-x][vy-y][ps_rs]==rs_ene){
				mvprintw(7+mnsN,38,"%c - ATT:%d HP :%d VIS:%d MOV:%d\t",life[vx-x][vy-y],status[vx-x][vy-y][ps_att],status[vx-x][vy-y][ps_HP],status[vx-x][vy-y][ps_vis],status[vx-x][vy-y][ps_mov]);
				mnsN++;
			}
		}
	}
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
	float mAtt,mHP,mMov,mVis,mG;
	for(y=0;y<mvy;y++){
		for(x=0;x<mvx;x++){
			if(life[vx-x][vy-y]!=t_null && status[vx-x][vy-y][ps_rs]==rs_ene && status[vx-x][vy-y][ps_mov]>=1){
				mnx=vx-x;
				mny=vy-y;
				///status[mnx][mny][ps_mvc]=true;
				//*DEBUG*/mvprintw(12,40,"%d",status[mnx][mny][ps_mov]);
				//*DEBUG*/refresh();
				status[mnx][mny][ps_mov]--;
				mAtt=status[mnx][mny][ps_att];
				mHP =status[mnx][mny][ps_HP];
				mVis=status[mnx][mny][ps_vis];
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
				spawn(ox,oy,mG,rs_ene,mAtt,mHP,mMov,mVis);
				//*DEBUG*/move(15,40);
				//*DEBUG*/printw("spawned %c from %d,%d to %d,%d",mG,mnx,mny,ox,oy);
				//*DEBUG*/refresh();
				//*DEBUG*/getch();
			}
		}
	}
}

void adv_move(){
	aatt=status[ax][ay][ps_att];
	ahp =status[ax][ay][ps_HP];
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
	spawn(ax,ay,l_adv,rs_adv,aatt,ahp,t_null,t_null);
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
	//game initialization
	start_game();
	///spawn_m(8,maxy/2,"slime");
	i_spawn_ms();
	print_vision();
	//main loop
	while(ahp>0){
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
	}
	end();
	endwin();
	return 0;
}
