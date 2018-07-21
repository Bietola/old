/*
Programmatore       : Stefano Montesi
Programma           : "Dungeon" (il nome cambiera' eventualmente)
Descrizione         : Un piccolo giochino stile "rough" fortemente in fase beta (:D)
Ultima modifica     : 6/8/2013 - 3:26 PM
*/

#include <stdio.h>
#include <stdarg.h>  //for functions with dynamic arguments
#include <stdlib.h>  //for rand() & srand()
#include <time.h>    //for srand(time(NULL))
#include <curses.h>


int j,k,x,y,vx,vy,ax,ay,mx,my,xmill;
int aatt,ahp;
char com;
int mvx=32; //max=78 || max=maxx //def.=32 //HAS TO BE EVEN
int mvy=17; //max=20 || max=maxy //def.=17
int maxx=100;
int maxy=100;
char ground[100][100];
char item  [100][100];
char life  [100][100];
int  status[100][100][3];

//status[x][y][ps_rs] - "Role Status" the role that the entity is playing
int rs_adv=1;
int rs_ene=2;
int rs_fri=3;

//status[x][y][n] - "Position Status" the position in the third dimension of the status array in which to strore what (n)
char ps_rs =0;
char ps_HP =1;
char ps_att=2;

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

void print_attr(const char* name,int n,int row){
	//a f. that prints an ATTRibute
	if(y==row){
		printw("\t");
		addstr(name);
		printw(" =");
		if(n<10) printw("   ");
		if(n>=10 && n<100) printw("  ");
		if(n>=100 && n<1000) printw(" ");
		printw("%d",n);
	}
}

void setRS (int xx,int yy,int n){
	status[xx][yy][ps_rs]=n;
}

void setAtt(int xx,int yy,int n){
	status[xx][yy][ps_att]=n;
}

void chaAtt(int xx,int yy,int n){
	status[xx][yy][ps_att]=status[xx][yy][ps_att]+n;
}

void setHP(int xx,int yy,int n){
	status[xx][yy][ps_HP]=n;
}

void chaHP(int xx,int yy,int n){
	status[xx][yy][ps_HP]=status[xx][yy][ps_HP]+n;
}

void spawn(char g,/*const char* name,*/int xx,int yy,int rs,int att,int hp){
	life[xx][yy]=g;
	setRS(xx,yy,rs);
	setHP(xx,yy,hp);
	setAtt(xx,yy,att);
}

void spawn_m(int xx,int yy,const char* ent/*ENTity to spawn*/){
	int sRS,sHP,sATT;
	char sG;
	if(ent=="slime"){
		sRS  =rs_ene;
		sHP  =rand()%10+3;
		sATT =rand()%3+1;
		sG   ='s';
	}
	else if(ent=="creeper"){
		sRS  =rs_ene;
		sHP  =rand()%12+5;
		sATT =rand()%5+2;
		sG   ='c';
	}
	spawn(sG,xx,yy,sRS,sATT,sHP);
}

void despawn(int xx,int yy){
	life[xx][yy]=t_null;
    for (j=0;j<3;j++) status[xx][yy][j]=t_null;
}

bool check_par(const char* spar,const char* par[],int max){
	bool chk=false;
	for(j=0;j<max;j++){
		if(par[j]==spar){
			chk=true;
			return true;
			break;
		}
	}
	if(chk==false) return false;
}

void generate_g(int i,...){
	const char* n[i];
	va_list argptr;
	va_start(argptr,i);
	
	
	
	for(j=0;j++;j<i){
		n[j]=va_arg(argptr,const char*);
		move(30,110);
		printw("hello");
	}
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			xmill=rand()%1000;
			if(xmill<750) ground[x][y]=g_light;
			if(check_par("wall",n,i)==true){
				if(xmill>=750 /*&& xmill<985*/) ground[x][y]=g_wall;
			}
			else{
				/*if(xmill>=985)*/if(xmill>=750) ground[x][y]=g_tree;
			}
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
			for(j=0;j<3;j++) status[x][y][j]=t_null;
		}
	}
	generate_g(1,"wall");
	//spawn character
	j=0;
	for(y=maxy/2;y>=0;y--){
		for(x=maxx/2;x>=0;x--){
			if(ground[x][y]==g_light){
				aatt=1;
				ahp=10;
				spawn(l_adv,x,y,rs_adv,aatt,ahp);
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
			xmill=rand()%300;
			if(ground[x][y]==g_light){
				if(xmill==0){
					xmill=rand()%100;
                	if(xmill<=75) spawn_m(x,y,"slime");
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
                	if(xmill<=75) spawn_m(x,y,"slime");
					if(xmill> 75) spawn_m(x,y,"creeper");
				}
			}
		}
	}
}

void board_updt(){
	for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			if(status[x][y][ps_HP]<=0) despawn(x,y);
		}
	}
}

void print_vision(){
	//print board
	move(0,0);
   	for(y=0;y<mvy;y++){
        printw("\n");
		for(x=0;x<mvx;x++){
			//game-screen
			if(life[vx-x][vy-y]!=t_null) addch(life[vx-x][vy-y]);
			else if(item[vx-x][vy-y]!=t_null) addch(item[vx-x][vy-y]);
			else addch(ground[vx-x][vy-y]);
			//attributes (WIP)
			if(x==mvx-1){
				print_attr("ATT",status[ax][ay][ps_att],0);
				print_attr("xAT",status[ax][ay+1][ps_att],0);
				print_attr("HP ",status[ax][ay][ps_HP],1);
				print_attr("xHP",status[ax][ay+1][ps_HP],1);
			}
		}
	}
	//print comments (WIP)
	printw("\n\ncoos:  %d;%d\n\nlast com: %c (%d)",ax,ay,com,com);
	refresh();
}

void adv_updt(){
	aatt=status[ax][ay][ps_att];
	ahp =status[ax][ay][ps_HP];
}

void attk(int atx,int aty,int dex,int dey){
	//WIP
	chaHP(dex,dey,-status[atx][aty][ps_att]);
}

void adv_move(){
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
	spawn(l_adv,ax,ay,rs_adv,aatt,ahp);
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
	i_spawn_ms();
	print_vision();
	//input
	while(ahp>0){
		com=getch();
		//move or attack with all entities in board
		//adv.
		spawn_ms();
		adv_move();
		board_updt();
		adv_updt();
		print_vision();
	}
	end();
	endwin();
	return 0;
}
