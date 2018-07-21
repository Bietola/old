/*
Programmatore       : Stefano Montesi
Programma           : "Dungeon" (il nome cambiera' eventualmente)
Descrizione         : Un piccolo giochino stile "rough" fortemente in fase beta (:D)
Ultima modifica     : 6/8/2013 - 3:26 PM
*/

#include <stdio.h>
#include <stdlib.h>  //for rand() & srand()
#include <time.h>    //for srand(time(NULL))
#include <curses.h>


int j,k,x,y,x2,y2,vx,vy,ax,ay,mx,my,xmill;
int aatt,ahp;
char com;
int mvx=32; //max=78
int mvy=17; //max=20
int maxx=100;
int maxy=100;
char ground[100][100];
char item  [100][100];
char life  [100][100];
int  status[100][100][4];

//status[x][y][ps_rs] - "Role Status" the role that the entity is playing
int rs_adv=1;
int rs_ene=2;
int rs_fri=3;

//status[x][y][n] - "Position Status" the position in the third dimension of the status array in which to strore what (n)
char ps_rs =0;
char ps_HP =1;
char ps_att=2;
char ps_vis=3;

//Controlls:
/*
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

void setVis(int xx,int yy,int n){
	status[xx][yy][ps_vis]=n;
}

void chaVis(int xx,int yy,int n){
	status[xx][yy][ps_vis]=status[xx][yy][ps_vis]+n;
}

void spawn(char g,/*const char* name,*/int xx,int yy,int rs,int att,int hp,int vis){
	life[xx][yy]=g;
	setRS(xx,yy,rs);
	setHP(xx,yy,hp);
	setAtt(xx,yy,att);
	setVis(xx,yy,vis);
}

void spawn_m(int xx,int yy,const char* ent/*ENTity to spawn*/){
	int sRS,sHP,sATT,sVIS;
	char sG;
	if(ent=="adv" || ent=="adventurer"){
		sRS  =rs_adv;
		sHP  =ahp;
		sATT =aatt;
		sVIS =t_null;
		sG   ='@';
	}
	else if(ent=="slime"){
		sRS  =rs_ene;
		sHP  =rand()%10+3;
		sATT =rand()%3+1;
		sVIS =rand()%20+10;
		sG   ='s';
	}
	else if(ent=="creeper"){
		sRS  =rs_ene;
		sHP  =rand()%12+5;
		sATT =rand()%5+2;
		sVIS =rand()%20+10;
		sG   ='c';
	}
	spawn(sG,xx,yy,sRS,sATT,sHP,sVIS);
}

void despawn(int xx,int yy){
	life[xx][yy]=t_null;
    for (j=0;j<3;j++) status[xx][yy][j]=t_null;
}

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
			for(j=0;j<4;j++) status[x][y][j]=t_null;
		}
	}
	generate_g();
	//spawn character
	j=0;
	for(y=maxy/2;y>=0;y--){
		for(x=maxx/2;x>=0;x--){
			if(ground[x][y]==g_light){
				aatt=1;
				ahp=10;
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

/*void mns_move(){
	for(y=0;y<mvy;y++){
		for(x=0;x<mvx;x++){6
			if(life[x][y]!=t_null && status[x][y][ps_rs]==rs_ene){
				for
			}
		}
	}
}*/

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

void move(int xx,int yy,int ox,int oy){
	int mg  =life[xx][yy];
	int mrs =status[xx][yy][ps_rs];
	int matt=status[xx][yy][ps_att];
	int mhp =status[xx][yy][ps_HP];
	int mvis=status[xx][yy][ps_vis];
		printw("ok - %d;%d - %d;%d",xx,yy,ox,oy);
		getch();
	//if(life[ox][oy]!=t_null && (((status[xx][yy][ps_rs]==rs_adv || status[xx][yy][ps_rs]==rs_fri) && status[ox][oy][ps_rs]==rs_ene) || (status[xx][yy][ps_rs]==rs_ene && (status[ox][oy][ps_rs]==rs_adv || status[ox][oy][ps_rs]==rs_fri)))){
	if(life[ox][oy]!=t_null && status[ox][oy][ps_rs]==rs_ene){
			attk(xx,yy,ox,oy);
	}
	if(ground[ox][oy]!=g_light || life[ox][oy]!=t_null){
		ox=xx;	
		oy=yy;
		printw("ok2 - %d;%d - %d;%d",xx,yy,ox,oy);
		getch();
	}
	despawn(xx,yy);
	spawn(mg,ox,oy,mrs,matt,mhp,mvis);
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
	/*if(life[ax][ay]!=t_null && status[ax][ay][ps_rs]==rs_ene){
		attk(mx,my,ax,ay);
	}
	if(ground[ax][ay]!=g_light || life[ax][ay]!=t_null){
		ax=mx;
		ay=my;
	}
	despawn(mx,my);
	spawn(l_adv,ax,ay,rs_adv,aatt,ahp,avis);*/
	move(mx,my,ax,ay);
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
	//main loop
	while(ahp>0){
		//input
		com=getch();
		//spawn monsters
		spawn_ms();
		//monster stuff (WIP)
		/*mns_move();*/
		//adv. stuff
		adv_move();
		adv_updt();
		//board stuff
		board_updt();
		//output
		print_vision();
	}
	end();
	endwin();
	return 0;
}
