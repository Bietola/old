#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

///CONSTANTS
#define BUFLEN 100

///TYPES
//move type
enum move_t{
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
};
//role type (how a player plays the game)
enum role_t{
    USER,
    CPU,
};

///STRUCTURES
//player structure
struct Player{
    //strings
    char* name;
    //technical characteristics
    enum role_t role;
    enum state_t state;
};

///GLOBAL DATA
//players array
struct Player* players;
//first player that looses (he's a big merda)
int loserID=-1;
//number of players currently in the players array
int plNum=0;
//number of battles and rounds that have been played
int battles=0;
int rounds=0;

///FUNCTIONS
///utilities
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
        //get role
        fscanf(f,"%s",inStr);
        if(strcmp(inStr,"USER")==0) players[plNum].role=USER;
        else players[plNum].role=CPU;
        //set initial state
        players[plNum].state=FREE;
        //increment players number
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
        printf("%s\n",players[j].name);
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
//randomly picks two players
void pick_players(int* pl1,int* pl2){
    //1
    do{
        *pl1=rand()%plNum;
    }while(players[*pl1].state!=FREE);
    //2
    do{
        *pl2=rand()%plNum;
    }while(players[*pl2].state!=FREE || *pl2==*pl1);
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
            if(players[j].state==WINNER || players[j].state==FREE){
                players[j].state=FREE;
                k++;
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
//end the game
void end(){
    int j;//counter
    //find the winner
    for(j=0;j<plNum;j++){
        if(players[j].state==FREE) break;
    }
    //print the winner
    printf("the champion of this rps tournament is %s!\n",players[j].name);
    //add it to the hall of fame
    FILE* f=fopen("hall of fame.txt","a");
    fprintf(f,"%s\n",players[j].name);
    fclose(f);
    f=fopen("hall of shame.txt","a");
    fprintf(f,"%s\n",players[loserID].name);
    fclose(f);
    getch();
}
///battles
//calculates a move based on the role
enum move_t pick_move(struct Player player){
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
        return (enum move_t)(rand()%3);
    }
};
//processes and prints the result of a normal rps battle
void rps(){
    int pl1,pl2; //ids of the chosen players inside the players array
    enum move_t move1,move2; //moves that the chosen players have decided to make
    //pick players
    pick_players(&pl1,&pl2);
    //announce battle
    printf("b%d: %s vs %s\n",battles+1,players[pl1].name,players[pl2].name);
    getch();
    //pick players moves
    move1=pick_move(players[pl1]);
    move2=pick_move(players[pl2]);
    //print the battle
    printf("%s throws %s, %s throws %s.\n",players[pl1].name,mtoa(move1),players[pl2].name,mtoa(move2));
    getch();
    //see who wins
    //tie
    if(move1==move2){
        players[pl1].state=WINNER;
        players[pl2].state=WINNER;
        printf("it's a tie...\n\n");
    }
    //someone wins
    else{
        //pl1 wins
        if((move1==ROCK && move2==SCISSORS) || (move1==PAPER && move2==ROCK) || (move1==SCISSORS && move2==PAPER)){
            players[pl1].state=WINNER;
            players[pl2].state=LOSER;
            printf("%s wins!\n\n",players[pl1].name);
            if(loserID<0) loserID=pl2;
        }
        //pl2 wins
        else{
            players[pl2].state=WINNER;
            players[pl1].state=LOSER;
            printf("%s wins!\n\n",players[pl2].name);
            if(loserID<0) loserID=pl1;
        }
    }
    //increase battle counter
    battles++;
}

///MAIN
int main(){
    get_players();
    intro();
    do{
        rps();
    }while(check_end()==0);
    end();
}
