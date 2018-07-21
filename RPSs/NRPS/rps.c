#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

///CONSTANTS
#define BUFLEN 100

#define DB_BRAINDES 1
#define DB_BRAIN 0

#define MAX_FANS 500
#define CROWD_ROAR_CHANCE 50
#define CROWD_ROAR_MULT 4
#define FAN_MULT 2
#define WIN_FAN_MULT 5

#define TIE_MONSTER_CHANCE 80
#define TIE_MONSTER_LIFE_MULT 0.5
#define TIE_MONSTER_REWARD_LIFE_MULT 0.25
#define TIE_MONSTER_REVIVE_MULT 0.25
#define TIE_MONSTER_HEROS_MULT 0.5

#define HITLER_CHANCE 50
#define HITLER_LIFE_MULT 0.5
#define HITLER_MINION_MULT 0.5
#define HITLER_REWARD_LIFE_MULT 0.25
#define HITLER_REVIVE_MULT 0.25
#define HITLER_HEROES_MULT 0.25

///TYPES
//move type
enum move_t{
    MOVE_NULL,
    ROCK,
    PAPER,
    SCISSORS,
};
//state type
enum state_t{
    FREE,
    INBATTLE,
    WINNER,
    LOSER,
    BOSSWINNER,
};
//role type (how a player plays the game)
enum role_t{
    USER,
    CPU,
};
//gender type
enum gender_t{
    MALE,
    FEMALE,
    NEUTRAL,
};
//boss type
enum boss_t{
    BOSS_SCISSORS,
    BOSS_TIE_MONSTER,
};

///STRUCTURES
//player structure
struct Player{
    //strings
    char* name;
    char* boss;
    //technical characteristics
    enum role_t role;
    enum state_t state;
    //gender
    enum gender_t gender;
    //brain
    int offensive,defensive,random;
    //win stats
    int sWins,rWins,pWins,ties,lost;
    //other stats
    int fans,shit,honor,life,memory;
    //special tags
    int god,master;
};

///GLOBAL DATA
//players array
struct Player* players;
//number of players currently in the players array
int plNum=0;
//number of battles and rounds that have been played
int battles=0;
int rounds=0;
//number of ties
int ties=0;
//shame variable
int shame=1;
//show info
int showInfo=1;

//spawn checks
int spawn_boss=0;
int spawn_tie_monster=0;
int spawn_hitler=0;

///FUNCTIONS
///utilities
//add a player to the game
void add_player(char* name,char* boss,enum role_t role,enum gender_t gender,int god,int master,int fans,int shit,int honor,int life,int memory,int rWins,int pWins,int sWins,int ties,int lost,int offensive,int defensive,int random){
    //reallocate players
    players=(struct Player*)realloc(players,sizeof(struct Player)*(plNum+1));
    //add player to players
    players[plNum].name=(char*)malloc(sizeof(char)*BUFLEN);
    strcpy(players[plNum].name,name);
    players[plNum].boss=(char*)malloc(sizeof(char)*BUFLEN);
    strcpy(players[plNum].boss,boss);
    players[plNum].role=role;
    players[plNum].gender=gender;
    players[plNum].god=god;
    players[plNum].master=master;
    players[plNum].fans=fans;
    players[plNum].shit=shit;
    players[plNum].honor=honor;
    players[plNum].life=life;
    players[plNum].memory=memory;
    players[plNum].state=FREE;
    players[plNum].rWins=rWins;
    players[plNum].pWins=pWins;
    players[plNum].sWins=sWins;
    players[plNum].ties=ties;
    players[plNum].lost=lost;
    players[plNum].offensive=offensive;
    players[plNum].defensive=defensive;
    players[plNum].random=random;
    //increment players number
    plNum++;
}
//add normal player
void add_normal_player(char* name,enum role_t role,enum gender_t gender,int fans,int shit,int honor){
    int offensive=(rand()%100)+1;
    int defensive=offensive;
    int random=(rand()%100)+1;
    add_player(name,"NONE",role,gender,0,0,fans,shit,honor,1,1,0,0,0,0,0,offensive,defensive,random);
}
//add a boss to the game
void add_boss(char* name,int life,enum role_t role,enum gender_t gender){
    int offensive=(rand()%100)+1;
    int defensive=offensive;
    int random=(rand()%100)+1;
    add_player(name,"NONE",role,gender,0,0,0,0,0,life,0,0,0,0,0,0,offensive,defensive,random);
}
//add a minion to the game
void add_minion(char* name,char* boss,int life,enum role_t role,enum gender_t gender){
    int offensive=(rand()%100)+1;
    int defensive=offensive;
    int random=(rand()%100)+1;
    add_player(name,boss,role,gender,0,0,0,0,0,life,0,0,0,0,0,0,offensive,defensive,random);
}
//transfer players from the players file to the players array
void get_players(){
    FILE* f; //pointer to file
    char inStr[BUFLEN]; //temporary holding string
    //initiate players array
    players=(struct Player*)malloc(sizeof(struct Player));
    //open the players file (no err return)
    f=fopen("players.txt","r");
    //get to the divider symbol
    while(fgetc(f)!=':');
    //get player information
    while(fscanf(f,"%s",inStr)==1){
        //reallocate players
        players=(struct Player*)realloc(players,sizeof(struct Player)*(plNum+1));
        //get name
        players[plNum].name=(char*)malloc(sizeof(char)*(strlen(inStr)+1));
        strcpy(players[plNum].name,inStr);
        //get gender
        fscanf(f,"%s",inStr);
        if(strcmp(inStr,"MALE")==0) players[plNum].gender=MALE;
        else if(strcmp(inStr,"FEMALE")==0) players[plNum].gender=FEMALE;
        else players[plNum].gender=NEUTRAL;
        //get role
        fscanf(f,"%s",inStr);
        if(strcmp(inStr,"USER")==0) players[plNum].role=USER;
        else players[plNum].role=CPU;
        //get fans, shit and honor
        fscanf(f,"%d",&players[plNum].fans);
        fscanf(f,"%d",&players[plNum].shit);
        fscanf(f,"%d",&players[plNum].honor);
        //get god
        fscanf(f,"%d",&players[plNum].god);
        //set initial state
        players[plNum].state=FREE;
        //set player to be memorized after the battle
        players[plNum].memory=1;
        //set life (only 1)
        players[plNum].life=1;
        //set boss (no one)
        players[plNum].boss=(char*)malloc(sizeof(char)*BUFLEN);
        strcpy(players[plNum].boss,"NONE");
        //set win stats
        players[plNum].rWins=0;
        players[plNum].pWins=0;
        players[plNum].sWins=0;
        players[plNum].ties=0;
        players[plNum].lost=0;
        //set brain
        players[plNum].offensive=(rand()%100)+1;
        players[plNum].defensive=100-players[plNum].offensive;
        players[plNum].random=(rand()%100)+1;
        //set special tags
        players[plNum].master=0;
        //increment player number
        plNum++;
    }
    //close file
    fclose(f);
}
//print players information on the terminal
void show_players(){
    int j; //counter
    //print players
    for(j=0;j<plNum;j++){
        //name
        printf("%s",players[j].name);
        //brain
        if(DB_BRAINDES) printf("\t\t{%d,%d,%d}",players[j].offensive,players[j].defensive,players[j].random);

        printf("\n");
    }
}
//print introduction
void intro(){
    //start random seed
    srand(time(NULL));
    //print intro phrase
    printf("the players:\n");
    show_players();
    printf("press any key to start the fight!\n");
    getch();
    printf("\n");
}
//converts a move type to a string
char* mtoa(enum move_t move){
    if(move==ROCK) return "rock";
    if(move==PAPER) return "paper";
    if(move==SCISSORS) return "scissors";
    else return NULL; //error
}
//converts a string to a move type
enum move_t atom(char* move){
    if(strcmp(move,"rock")==0) return ROCK;
    if(strcmp(move,"paper")==0) return PAPER;
    if(strcmp(move,"scissors")==0) return SCISSORS;
    else return -1; //error
}
//returns the personal pronoun bound to the specified gender
char* pronoun(enum gender_t gender){
    if(gender==MALE) return "he";
    if(gender==FEMALE) return "she";
    if(gender==NEUTRAL) return "it";
}
//returns the possessive pronoun bound to the specified gender
char* possessive(enum gender_t gender){
    if(gender==MALE) return "his";
    if(gender==FEMALE) return "her";
    if(gender==NEUTRAL) return "its";
}
//add fans
void add_fans(int pl,int fans){
    players[pl].fans+=fans;
    if(players[pl].shit>0) players[pl].shit-=fans;
    if(players[pl].shit<0) players[pl].shit=0;
    if(players[pl].fans<0){
        players[pl].shit-=players[pl].fans;
        players[pl].fans=0;
    }
    else if(players[pl].fans>MAX_FANS){
        players[pl].honor++;
        players[pl].fans=MAX_FANS;
    }
}
//check if a player can go against another player
int check_battle(int pl1,int pl2){
    if(players[pl2].state!=FREE || pl2==pl1 || strcmp(players[pl1].boss,players[pl2].name)==0 || strcmp(players[pl2].boss,players[pl1].name)==0 || (strcmp(players[pl2].boss,players[pl1].boss)==0 && strcmp(players[pl1].boss,"NONE")!=0 && strcmp(players[pl2].boss,"NONE")!=0)) return 0; //enemies are not compatible
    else return 1; //players are compatible
}
//checks if a round, or the game itself, has ended
int check_end(){
    int j,k; //counters
    //check if the round has finished
    k=0;
    for(j=0;j<plNum;j++){
        if(players[j].state==FREE) k++;
    }
    if(k<=1){
        //reset the players' states
        k=0;
        for(j=0;j<plNum;j++){
            if(players[j].state==WINNER || players[j].state==FREE || players[j].state==BOSSWINNER){
                if(players[j].state!=BOSSWINNER) k++;
                players[j].state=FREE;
            }
        }
        if(k<=1) return 1; //game is over
        //reset counters
        rounds++;
        battles=0;
        //announce that round has finished
        printf("prepare for round %d.\n\n",rounds+1);
    }
    return 0; //game is not over
}
//revive players for a boss battle
void boss_upgrade(int upgradeNum,int upgradeLife,int revive,int pl){
    int j,i,revivable=0;
	//upgrade summoner (if present)
    if(pl>=0){
        printf("%s has been upgraded! (%d)\n",players[pl].name,upgradeLife*2);
        players[pl].life=upgradeLife;
    }
    //augment players life
    for(i=0;i<upgradeNum;i++){
		j=rand()%plNum;
        if(j!=pl && (players[j].state==WINNER || players[j].state==FREE)){
			printf("%s has been upgraded! (%d)\n",players[j].name,upgradeLife);
            players[j].life=upgradeLife;
            players[j].state=WINNER;
        }
    }
    //revive fallen players
    if(revive>revivable) revive=revivable;
    while(revive>0){
        j=rand()%plNum;
        if(players[j].state==LOSER){
            printf("%s has been revived!\n",players[j].name);
            players[j].state=WINNER;
            revive--;
        }
    }
}
//spawn requested boss
void boss_spawn(int upgrade,enum boss_t boss,int pl1,int pl2){
    //tie monster (tie boss) (non summon)
    if(boss==BOSS_TIE_MONSTER){
        int mult=(rounds*2+ties)/2;
        if(rand()%TIE_MONSTER_CHANCE<=mult && spawn_boss==0){
            spawn_boss=1;
            printf("your evil actions have summoned a tie monster! (%d)\n",(int)(ties*TIE_MONSTER_LIFE_MULT)+1);
            if(upgrade) boss_upgrade((int)(ties*TIE_MONSTER_HEROS_MULT), (int)(TIE_MONSTER_REWARD_LIFE_MULT*mult)+1,(int)(TIE_MONSTER_REVIVE_MULT*mult)+1,-1);
            add_boss("tie monster",(int)(ties*TIE_MONSTER_LIFE_MULT)+1,CPU,NEUTRAL);
        }
    }
    //scissors bosses/protectors/master
    else if(boss==BOSS_SCISSORS && players[pl1].sWins>=3){
        int mult=rounds;
        if(players[pl1].sWins==3){
            printf("%s would you like to call Hitler? (%d,%d) ",players[pl1].name,(int)(mult*HITLER_LIFE_MULT)+1,(int)(mult*HITLER_MINION_MULT)+1);
            char s[BUFLEN];
            scanf("%s",&s);
            if(!strcmp(s,"yes")){
                spawn_boss=1;
                printf("\nHitler has entered the battlefield! (%d,%d)\n",(int)(mult*HITLER_LIFE_MULT)+1,(int)(mult*HITLER_MINION_MULT)+1);
                if(upgrade) boss_upgrade((int)(HITLER_HEROES_MULT*mult), (int)(HITLER_REWARD_LIFE_MULT*mult)+1,(int)(HITLER_REVIVE_MULT*mult)+1,pl1);
                add_boss("Hitler",(int)(mult*HITLER_LIFE_MULT)+1,CPU,MALE);
                int minions=(int)(mult*HITLER_MINION_MULT)+1;
                int j;
                for(j=0;j<minions;j++){
                    add_minion("soldier","Hitler",1,CPU,MALE);
                }
                players[pl1].sWins=0;
            }
            else printf("\nOk then...\n");
        }
        else if(players[pl1].sWins==4){
            printf("%s would you like to summon an ermenegildo? ",players[pl1].name);
            char s[BUFLEN];
            scanf("%s",&s);
            if(!strcmp(s,"yes")){
                printf("\n%s is now protected by an ermenegildo!\n",players[pl1].name);
                add_minion("ermenegildo",players[pl1].name,3,CPU,NEUTRAL);
                players[pl1].sWins=0;
            }
            else printf("\nOk then...\n");
        }
        else if(players[pl1].sWins>=5){
            printf("%s would you like to become a scissors master? (%d) ",players[pl1].name,players[pl1].sWins);
            char s[BUFLEN];
            scanf("%s",&s);
            if(!strcmp(s,"yes")){
                printf("\n%s has become a scissors master...\n",players[pl1].name);
                players[pl1].master=1;
                players[pl1].life=players[pl1].sWins;
                int minions=(int)(players[pl1].sWins);
                int j;
                for(j=0;j<minions;j++){
                    add_minion("ermenegildino",players[pl1].name,1,CPU,NEUTRAL);
                }
                players[pl1].sWins=0;
            }
            else printf("\nOk then...\n");
        }
    }
}
//randomly picks two players
int pick_players(int* pl1,int* pl2){
    int c;
    do{
        c=0;
        //find available player
        do{
            *pl1=rand()%plNum;
        }while(players[*pl1].state!=FREE);
        //check if there is a possible contender to the chosen player
        int j;
        for(j=0;j<plNum;j++){
            if(check_battle(*pl1,j)) break;
        }
        //if there's one (or more), choose it at random. If not mark the player as a winner
        if(j<plNum){
            do{
                *pl2=rand()%plNum;
            }while(!check_battle(*pl1,*pl2));
            return 1; //players pick-able
        }
        else{
            players[*pl1].state=BOSSWINNER;
            if(check_end()) return 0; //players not pick-able (game is over)
            c=1;
        }
    }while(c==1);
}
//end the game
void end(){
    int j;//counter
    FILE* f; //pointer to file
    char inStr[BUFLEN]; //string junk container
    int n; //integer junk container
    //save fans
    f=fopen("players.txt","w");
    fprintf(f,"name\t\tgender\trole\tfans\tshit\thonor\n");
    fprintf(f,"----------------------------------------------------:\n");
    //find the winner and reward him fans
    int win;
    for(win=0;win<plNum;win++){
        if(players[win].state==FREE) break;
    }
    //if the winner has a boss, then the boss wins
    if(strcmp(players[win].boss,"NONE")!=0){
        for(j=0;j<plNum;j++){
            if(strcmp(players[j].name,players[win].boss)==0) break;
        }
        win=j;
    }
    add_fans(win,rounds*WIN_FAN_MULT);
    //rewrite file adding fans
    for(j=0;j<plNum;j++){
        //check if player can be saved
        if(players[j].memory){
            //rewrite name
            fprintf(f,"%s\t\t",players[j].name);
            //rewrite gender
            if(players[j].gender==MALE) fprintf(f,"MALE\t");
            else if(players[j].gender==FEMALE) fprintf(f,"FEMALE\t");
            else if(players[j].gender==NEUTRAL) fprintf(f,"NEUTRAL\t");
            //rewrite role
            if(players[j].role==USER) fprintf(f,"USER\t");
            else if(players[j].role==CPU) fprintf(f,"CPU\t");
            //update fans, shit and honor and god
            fprintf(f,"%d\t%d\t%d\t%d\n",players[j].fans,players[j].shit,players[j].honor,players[j].god);
        }
    }
    fclose(f);
    //print the winner
    printf("the champion of this rps tournament is ");
    if(players[win].master) printf("master ");
    printf("%s! (%d)\n",players[win].name,WIN_FAN_MULT*rounds);
    //add it to the hall of fame
    f=fopen("hall of fame.txt","a");
    if(players[win].master) fprintf(f,"master ");
    fprintf(f,"%s\n",players[win].name);
    fclose(f);
    getch();
}
///battles
//calculates a move based on the role
enum move_t pick_move(struct Player player,struct Player enemy){
    //user controlled (input)
    if(player.role==USER){
        char inStr[BUFLEN]; //input string
        while(1){
            printf("%s pick a move: ",player.name);
            scanf("%s",inStr);
            if(atom(inStr)==-1){
                printf("invalid move\n");
            }
            else return atom(inStr);
        }
    }
    //normal computer controlled player
    if(player.role==CPU){
        enum move_t move=MOVE_NULL;
        int pTotWins=player.rWins+player.pWins+player.sWins;
        int eTotWins=enemy.rWins+enemy.pWins+enemy.sWins;
        ///offensive strategy
        void attack(){
            //choose which move to attack with
            int totWins=pTotWins;
            int rChance=player.rWins*100/totWins;
            int pChance=rChance+player.pWins*100/totWins;
            int sChance=pChance+player.sWins*100/totWins;
            if(DB_BRAIN) printf("%s attacks! (%d %d %d) ",player.name,rChance,pChance,sChance);
            int r=rand()%100;
            if(r<rChance) move=ROCK;
            else if(r<pChance) move=PAPER;
            else if(r<sChance) move=SCISSORS;
            if(DB_BRAIN) printf("attack move: %s\n",mtoa(move));
        }
        ///defensive strategy
        void defend(){
            int totWins=eTotWins;
            int rChance=enemy.rWins*100/totWins;
            int pChance=rChance+enemy.pWins*100/totWins;
            int sChance=pChance+enemy.sWins*100/totWins;
            if(DB_BRAIN) printf("%s defends! (%d %d %d) ",player.name,rChance,pChance,sChance);
            int r=rand()%100;
            if(r<rChance) move=PAPER;
            else if(r<pChance) move=SCISSORS;
            else if(r<sChance) move=ROCK;
            if(DB_BRAIN) printf("contrasted move: %s\n",mtoa(move));
        }
        ///decide smart or random strategy
		//random strategy
		if(rand()%100<=player.random || (enemy.rWins==0 && enemy.pWins==0 && enemy.sWins==0)){
			if(DB_BRAIN) printf("%s uses a random strategy\n",player.name);
            return (enum move_t)((rand()%3)+1);
		}
        //smart strategy
        else{
            if(DB_BRAIN) printf("%s uses a smart strategy\n",player.name);
            ///decide smart strategy type (defensive or offensive)
            if(pTotWins!=0 && eTotWins!=0){
                if(rand()%100<player.offensive) attack();
                else defend();
            }
            else if(pTotWins==0 && eTotWins==0){
                ;
            }
            else{
                if(pTotWins==0) defend();
                else if(eTotWins==0) attack();
            }
            return move;
        }
    }
};
//processes and prints the result of a normal rps battle
int rps(){
    int pl1,pl2; //ids of the chosen players inside the players array
    enum move_t move1,move2; //moves that the chosen players have decided to make
    //pick players
    if(!pick_players(&pl1,&pl2)) return 1; //game is over
    //announce battle
    if(showInfo) printf("b%d: %s [%d,%d,%d,%d,%d] vs %s [%d,%d,%d,%d,%d]",battles+1,players[pl1].name,players[pl1].rWins,players[pl1].pWins,players[pl1].sWins,players[pl1].ties,players[pl1].lost,players[pl2].name,players[pl2].rWins,players[pl2].pWins,players[pl2].sWins,players[pl2].ties,players[pl2].lost);
    else printf("b%d: %s vs %s",battles+1,players[pl1].name,players[pl2].name);
    if(players[pl1].life>1 || players[pl2].life>1) printf(" (%d,%d)",players[pl1].life,players[pl2].life);
    if(strcmp(players[pl1].boss,"NONE")!=0 || strcmp(players[pl2].boss,"NONE")!=0) printf(" (%s,%s)",players[pl1].boss,players[pl2].boss);
    printf("\n");
    getch();
    //pick players moves
    move1=pick_move(players[pl1],players[pl2]);
    move2=pick_move(players[pl2],players[pl1]);
    //print the battle
    printf("%s throws %s, %s throws %s.\n",players[pl1].name,mtoa(move1),players[pl2].name,mtoa(move2));
    getch();
    //see who wins
    //tie
    if(move1==move2 && !players[pl1].god && !players[pl2].god){
        //set both players as winners, announce tie, and increment tie counter
        players[pl1].state=WINNER;
        players[pl2].state=WINNER;
        players[pl1].ties++;
        players[pl2].ties++;
        printf("it's a tie...\n");
        ties++;
        //roll tie monsters event
        boss_spawn(1,BOSS_TIE_MONSTER,pl1,pl2);
        printf("\n");
    }
    //someone wins
    else{
        int win;
        int lose;
        //pl1 wins
        if(players[pl1].god || (((move1==ROCK && move2==SCISSORS) || (move1==PAPER && move2==ROCK) || (move1==SCISSORS && move2==PAPER)) && !players[pl2].god)){
            win=pl1;
            lose=pl2;
            if(move1==ROCK) players[pl1].rWins++;
            else if(move1==PAPER) players[pl1].pWins++;
            else if(move1==SCISSORS) players[pl1].sWins++;
        }
        //pl2 wins
        else{
            win=pl2;
            lose=pl1;
            if(move2==ROCK) players[pl2].rWins++;
            else if(move2==PAPER) players[pl2].pWins++;
            else if(move2==SCISSORS) players[pl2].sWins++;
        }
        //decide winner
        players[win].state=WINNER;
        //decide loser (or deduct life)
        if(players[lose].life<=1) players[lose].state=LOSER;
        else{
            players[lose].life--;
            players[lose].state=WINNER;
        }
        players[lose].lost++;
        //add loser to the hall of shame (if it's the first battle)
        if(shame){
            FILE* f=fopen("hall of shame.txt","a");
            fprintf(f,"%s\n",players[lose].name);
            add_fans(lose,-20);
            fclose(f);
            shame=0;
        }
        //announce winner
        printf("%s wins!\n",players[win].name);
        //add and deduct fans
        int fans;
        if(rounds==0) fans=0;
        else fans=rand()%((rounds+1)*FAN_MULT);
        if(fans>0){
            char fanWord[4];
            //roll crowd roar
            if(rand()%CROWD_ROAR_CHANCE<=rounds){
                printf("the crowd roars: \"%s %s %s!\" ",players[win].name,players[win].name,players[win].name);
                fans*=CROWD_ROAR_MULT;
                players[win].honor++;
                players[lose].shit++;
            }
            //announce and exchange fans
            if(fans==1) strcpy(fanWord,"fan");
            else strcpy(fanWord,"fans");
            printf("%s steals %d %s from %s!\n",players[win].name,fans,fanWord,players[lose].name);
            add_fans(win,fans);
            add_fans(lose,-fans);
        }
        //roll Hitler event
        if((win==pl1 && move1==SCISSORS) || (win==pl2 && move2==SCISSORS)) boss_spawn(1,BOSS_SCISSORS,win,lose);
        printf("\n");
    }
    //increase battle counter
    battles++;
    return 0; // game not over
}

///MAIN
int main(){
    srand(time(NULL));
    get_players();
    intro();
    do{
        if(rps()) break;
    }while(check_end()==0);
    end();
}
