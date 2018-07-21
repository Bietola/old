/*
Programmatore       : Stefano Montesi
Programma           : "Dungeon" (il nome cambiera' eventualmente)
Descrizione         : Un piccolo giochino stile "rogue" fortemente in fase beta (:3)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<conio.h>
#include <iostream>
#include <string>
using namespace std;


int j,k,x,y,vx,vy,ax,ay,mx,my,xmill;
int aatt,ahp;
char com;
int mvx=32; //max=78
int mvy=17; //max=20
int maxx=100;
int maxy=100;
char ground[100][100];
char item  [100][100];
char life  [100][100];
int  status[100][100][3];

//status[x][y][ps_rs] - "Role Status" the role that the entity is playing
char rs_adv=1;
char rs_ene=2;
char rs_fri=3;

//status[x][y][n] - "Position Status" the position in the third dimension of the status array in which to store what (n)
char ps_rs =0;
char ps_HP =1;
char ps_att=2;

//Controlls:
char c_N =72;
char c_NE=71;
char c_E =75;
char c_SE=79;
char c_S =80;
char c_SO=81;
char c_O =77;
char c_NO=73;

//Technical stuff
char t_null=0;

//Graphics
char g_light='.';
char g_wall =177;
char g_tree =  5;

//Life (entities)
char l_adv='@';
char l_slime='s';
char l_creeper='c';

void intro(){
		cout<<"welcome to 'dungeon' (the name will change eventually...):  A 'roguelike' game\nstrongly in beta version... :)\n\n**THINGS TO DO**\n-)remake the game using only the 'curses' lybrary and not the 'conio' lybrary (to stop all the 'blinking' with every movement and to make the user interface much, much better\n-)combine all the arrays (life, ground, status, etc...) into a single array or vector (to preserve memory and to make things less messy...)\n-)add colors...\n-)REWRITE THE WHOLE THING USING OOP (otherwise, the more complicated the program gets, the more it's going to give me headaches :) )\n\n**INSTRUCTIONS**\n-)you can either use the directional arrows or the numberpad to move your character (which is the '@' at the center of the screen) if you use the numpad, you can even move verticaly using the numbers 7,9,1,3\n-)to kill monsters (which are rappresented by letters) just walk into them: they will not attack you and they don't even move (:D), but this will obviously change in the future (a very distant future...)\n-)if you walk in front of a monster (so the monster has to be above your character) the console will tell you the monster's ATT (useless for now) and HPs through xAT and xHP\n\nthat's all for now, press any key to play...";
		while(true) if(kbhit()) break;
}

void print_attr(string name,int n,int row){
	//a f. that prints an ATTRibute
	if(y==row){
		cout<<"\t";
		cout<<name<<" =";
		if(n<10) cout<<"   ";
		if(n>=10 && n<100) cout<<"  ";
		if(n>=100 && n<1000) cout<<" ";
		cout<<n;
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

void spawn(char g,/*string name,*/int xx,int yy,int rs,int att,int hp){
	life[xx][yy]=g;
	setRS(xx,yy,rs);
	setHP(xx,yy,hp);
	setAtt(xx,yy,att);
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
	generate_g();
	//spawn character
	j=0;
	for(y=maxy/2;y>=0;y--){
		for(x=maxx/2;x>=0;x--){
			if(ground[x][y]==g_light){
				aatt=1;
				ahp=10;
				spawn(l_adv,x,y,rs_adv,aatt,ahp);
				//ini. ax & ay (adventurer x & adventurer y)
				ax=x;
				ay=y;
				//ini. vx & vy (vision x & vision y)
				vx=mvx/2+x;
				vy=mvy/2+y;
				j=1;
			}
			if(j==1) break;
		}
		if(j==1) break;
	}
}

void i_spawn_m(){
	//spawns monsters (made to be used one time only)
    for(y=maxy-1;y>=0;y--){
		for(x=maxx-1;x>=0;x--){
			xmill=rand()%300;
			if(ground[x][y]==g_light){
				if(xmill==0){
					xmill=rand()%100;
                	if(xmill<=75) spawn(l_slime,x,y,rs_ene,1,5);
					if(xmill> 75) spawn(l_creeper,x,y,rs_ene,3,12);
				}
			}
		}
	}
}

void spawn_m(){
	//spawns monsters (made to be used continusly) **NOT USED IN PROGRAM**
   	for(y=0;y<mvy;y++){
		for(x=0;x<mvx;x++){
			xmill=rand()%1000000000000000;
			if(ground[vx-x][vy-y]==g_light){
				if(xmill==0){
					xmill=rand()%100;
                	if(xmill<=75) spawn(l_slime,x,y,rs_ene,1,5);
					if(xmill> 75) spawn(l_creeper,x,y,rs_ene,3,12);
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
	system("cls");
	//print board
   	for(y=0;y<mvy;y++){
        cout<<"\n ";
		for(x=0;x<mvx;x++){
			//game-screen
			if(life[vx-x][vy-y]!=t_null) cout<<life[vx-x][vy-y];
			else if(item[vx-x][vy-y]!=t_null) cout<<item[vx-x][vy-y];
			else cout<<ground[vx-x][vy-y];
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
	cout<<"\n\ncoos:  "<<ax<<";"<<ay<<"\n\nlast com:  ";
	printf("%c (%d)",com,com);
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
	cout<<"\npress any key to close the program";
	while(true) if(kbhit()) break;
}

int main()
{
	intro();
	start_game();
	i_spawn_m();
	print_vision();
	//input
	while(ahp>0){
		while(true) if(kbhit()){
			com=getch();
			break;
		}
		//move or attack with all entities in board
		
		//WIP (this is why everything stays where it is)
		
		//adv.
		adv_move();
		board_updt();
		adv_updt();
		print_vision();
	}
	system("cls");
	cout<<"you lost, good luck next time :)";
	end();
	return 0;
}
