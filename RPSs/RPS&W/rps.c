#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

///CONSTANTS
#define BUFLEN 100

#define HEAD_HEALTH 100
#define BODY_HEALTH 100
#define ARM_HEALTH 100
#define LEG_HEALTH 100

#define STRENGTH_MIN 1
#define STRENGTH_MAX 10

#define ENDURANCE_MIN 1
#define ENDURANCE_MAX 10

#define AGILITY_MIN 1
#define AGILITY_MAX 10

#define INTELLECT_MIN 1
#define INTELLECT_MAX 10

#define CONSCIOUSNESS_MIN 10

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
//gender type
enum gender_t{
    MALE,
    FEMALE,
    NEUTRAL,
};

///STRUCTURES
//body part structure
struct BodyPart{
    char* name;
    int health;
    int canParry;
    int canGrab;
    int canStrike;
};
//player structure
struct Player{
    //strings
    char* name;
    //technical characteristics
    enum role_t role;
    enum state_t state;
    //gender
    enum gender_t gender;
    //health
    int consciousness;
    //main properties
    int intellect;
    int strength;
    int agility;
    int endurance;
    //body parts
    int numBodyParts;
    struct BodyPart* bodyParts;
};

///GLOBAL DATA
//players array
struct Player* players;
//number of players currently in the players array
int plNum=0;
//number of battles and rounds that have been played
int battles=0;
int rounds=0;

///FUNCTIONS
///body parts
//add a new body part to a player
void add_body_part(struct Player* player,int canParry,int canGrab,int canStrike,char* name,int health){
    //find part id
    int id=player->numBodyParts;
    //allocate memory
    printf("strlen: %d\n",strlen(name));
    printf("id: %d\n",id);
    if(id==0) player->bodyParts=(struct BodyPart*)malloc(sizeof(struct BodyPart));
    else player->bodyParts=(struct BodyPart*)realloc(players->bodyParts,sizeof(struct BodyPart)*(id+1));
    //add "can" variables
    player->bodyParts[id].canParry=canParry;
    player->bodyParts[id].canGrab=canGrab;
    player->bodyParts[id].canParry=canStrike;
    //add name
    player->bodyParts[id].name=(char*)malloc(sizeof(char)*(strlen(name)+1));
    strcpy(player->bodyParts[id].name,name);
    //add health
    player->bodyParts[id].health=health;
    //increase the number of body parts
    player->numBodyParts++;
}
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
        //get gender
        fscanf(f,"%s",inStr);
        if(strcmp(inStr,"MALE")==0) players[plNum].gender=MALE;
        else if(strcmp(inStr,"FEMALE")==0) players[plNum].gender=FEMALE;
        else players[plNum].gender=NEUTRAL;
        //get role
        fscanf(f,"%s",inStr);
        if(strcmp(inStr,"USER")==0) players[plNum].role=USER;
        else players[plNum].role=CPU;
        //set initial state
        players[plNum].state=FREE;
        //set random attributes
        players[plNum].intellect=INTELLECT_MIN+(rand()%(INTELLECT_MAX-INTELLECT_MIN));
        players[plNum].strength=STRENGTH_MIN+(rand()%(STRENGTH_MAX-STRENGTH_MIN));
        players[plNum].endurance=ENDURANCE_MIN+(rand()%(ENDURANCE_MAX-ENDURANCE_MIN));
        players[plNum].agility=AGILITY_MIN+(rand()%(AGILITY_MAX-AGILITY_MIN));
        //set hit points (or consciousness)
        players[plNum].consciousness=CONSCIOUSNESS_MIN+players[plNum].endurance+(players[plNum].intellect/2);
        //set default body parts
        /*players[plNum].numBodyParts=0;
        add_body_part(&players[plNum],0,0,1,"head",BODY_HEALTH);
        add_body_part(&players[plNum],0,0,0,"chest",BODY_HEALTH);
        add_body_part(&players[plNum],0,0,0,"abdomen",BODY_HEALTH);
        add_body_part(&players[plNum],0,0,0,"groin",BODY_HEALTH);
        add_body_part(&players[plNum],1,1,1,"right arm",BODY_HEALTH);
        add_body_part(&players[plNum],1,1,1,"left arm",BODY_HEALTH);
        add_body_part(&players[plNum],1,1,1,"right leg",BODY_HEALTH);
        add_body_part(&players[plNum],1,1,1,"left leg",BODY_HEALTH);*/
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
        }
        //pl2 wins
        else{
            players[pl2].state=WINNER;
            players[pl1].state=LOSER;
            printf("%s wins!\n\n",players[pl2].name);
        }
    }
    //increase battle counter
    battles++;
}
//get a wrestle string information
void wrestle_info(char* wrestleString,char* attack,char* attacked,char* attacker){
    int j=0;
    int k=0;
    //find attack
    do{
        attack[j]=wrestleString[j];
        j++;
    }while(wrestleString[j]!=' ' && wrestleString[j]!='\n' && wrestleString[j]!='\0');
    attack[j]='\0';
    //if the attack is "parry" then don't try to find the attacked body part
    if(strcmp(attack,"parry")==0 || strcmp(attack,"charge")==0) strcpy(attacked,"none");
    //find attacked body part
    else{
        j++;
        do{
            attacked[k]=wrestleString[j];
            j++;
            k++;
        }while(wrestleString[j]!=' ' && wrestleString[j]!='\n' && wrestleString[j]!='\0');
        attacked[k]='\0';
        //get the rest of the name if the body part is an arm of a leg
        if(strcmp(attacked,"left")==0 || strcmp(attacked,"right")==0){
            do{
                attacked[k]=wrestleString[j];
                j++;
                k++;
            }while(wrestleString[j]!=' ' && wrestleString[j]!='\n' && wrestleString[j]!='\0');
            attacked[k]='\0';
        }
    }
    //charge does not use any body part
    if(strcmp(attack,"charge")==0) strcpy(attacker,"none");
    //skip the "with"
    else{
        j++;
        while(wrestleString[j]!=' ') j++;
        j++;
        //find the body part with which the player attacked
        k=0;
        do{
            attacker[k]=wrestleString[j];
            j++;
            k++;
        }while(wrestleString[j]!=' ' && wrestleString[j]!='\n' && wrestleString[j]!='\0');
        attacker[k]='\0';
        //get the rest of the name if the body part is an arm of a leg
        if(strcmp(attacker,"left")==0 || strcmp(attacker,"right")==0){
            do{
                attacker[k]=wrestleString[j];
                j++;
                k++;
            }while(wrestleString[j]!=' ' && wrestleString[j]!='\n' && wrestleString[j]!='\0');
            attacker[k]='\0';
        }
    }
}
/*
//finds the move type inside of a wrestleString
char* find_move(char* wrestleString){
    int j=0;
    char move[BUFLEN];
    do{
        move[j]=wrestleString[j];
        j++;
    }while(wrestleString[j]!=' ');
    move[j]='\0';
    return move;
}
//finds the body part attacked inside of a wrestleString
char* find_attacked(char* wrestleString){
    int j=0;
    int k=0;
    char attacked[BUFLEN];
    //skip move
    while(wrestleString[j]!=' ') j++;
    j++;
    //get attacked body part
    do{
        attacked[k]=wrestleString[j];
        j++;
        k++;
    }while(wrestleString[j]!=' ');
    attacked[k]='\0';
    //get the rest of the name if the body part is an arm of a leg
    if(strcmp(attacked,"left")==0 || strcmp(attacked,"right")==0){
        do{
            attacked[k]=wrestleString[j];
            j++;
            k++;
        }while(wrestleString[j]!=' ');
        attacked[k]='\0';
    }
    return attacked;
}
//finds the body part with which the player attacked inside of a wrestleString
char* find_attacker(char* wrestleString){
    int j=0;
    int k=0;
    char attacker[BUFLEN];
    //skip move
    while(wrestleString[j]!=' ') j++;
    j++;
    //get attacked body part
    do{
        attacker[k]=wrestleString[j];
        j++;
        k++;
    }while(wrestleString[j]!=' ');
    //get the rest of the name if the body part is an arm of a leg
    if(strcmp(attacker,"left")==0 || strcmp(attacker,"right")==0){
        while(wrestleString[j]!=' ') j++;
        j++;
    }
    //skip the "with"
    while(wrestleString[j]!=' ') j++;
    j++;
    //get the string
    k=0;
    do{
        attacker[j]=wrestleString[j];
        j++;
        k++;
    }while(wrestleString[j]!=' ');
    //get the rest of the name if the body part is an arm of a leg
    if(strcmp(attacker,"left")==0 || strcmp(attacker,"right")==0){
        do{
            attacker[k]=wrestleString[j];
            j++;
            k++;
        }while(wrestleString[j]!=' ');
    }
    attacker[k-1]='\0';
    return attacker;
}
*/
//make a wrestle string based on the specified player's role
void make_wrestle_string(struct Player player,char* wrestleString){
    char analizer[BUFLEN];
    //user controlled (input)
    if(player.role==USER){
        //get wrestle string
        printf("%s, make your move: ",player.name);
        fgets(wrestleString,BUFLEN,stdin);
        //!check for an error (WIP)
    }
    //normal computer controlled player
    else if(player.role==CPU){
        int r;
        char* attack;
        char* attacked;
        char* attacker;
        //attack type
        r=rand()%4;
        if(r==0) attack="parry";
        else if(r==1) attack="grab";
        else if(r==2) attack="strike";
        else if(r==3){
            wrestleString="charge";
            //!test
            printf("WS: %s\n",wrestleString);
            return;
        }
        //attacked body part
        if(strcmp(attack,"parry")==0) attacked=" ";
        else{
            r=rand()%6;
            if(r==0) attacked=" head ";
            else if(r==1) attacked=" body ";
            else if(r==2) attacked=" right arm ";
            else if(r==3) attacked=" left arm ";
            else if(r==4) attacked=" right leg ";
            else if(r==5) attacked=" left leg ";
        }
        //attacker body part
        r=rand()%6;
        if(r==0) attacker="with head";
        else if(r==1) attacker="with body";
        else if(r==2) attacker="with right arm";
        else if(r==3) attacker="with left arm";
        else if(r==4) attacker="with right leg";
        else if(r==5) attacker="with left leg";
        //concatenate strings
        strcpy(wrestleString,attack);
        strcat(wrestleString,attacked);
        strcat(wrestleString,attacker);
        //!test
        printf("WS: %s\n",wrestleString);
    }
}
//attempts to make the "winner" win according to his attack type compared to the "loser". Returns 0 if it fails. Also calculates the outcome of ties
int whurt(int winner,char* winWrestleString,int loser,char* losWrestleString){
    char wAttack[BUFLEN],lAttack[BUFLEN];
    char wAttacker[BUFLEN],lAttacker[BUFLEN];
    char wAttacked[BUFLEN],lAttacked[BUFLEN];
    //get the wrestle string infos
    wrestle_info(winWrestleString,wAttack,wAttacked,wAttacker);
    wrestle_info(losWrestleString,lAttack,lAttacked,lAttacker);
    printf("wAttack: %s\n",wAttack);
    //tie
    if(strcmp(wAttack,lAttack)==0){
        //stare (both parry)
        if(strcmp(wAttack,"parry")==0){
            printf("%s stares at %s...\n%s stares back...\n",players[winner].name,players[loser].name,players[loser].name);
        }
        //hug (both grab)
        else if(strcmp(wAttack,"grab")==0){
            printf("%s gently hugs %s...\n",players[winner].name,players[loser].name);
        }
        //double strike (both strike)
        else if(strcmp(wAttack,"strike")==0){
            printf("%s strikes %s's %s with %s %s.\n",players[winner].name,players[loser].name,wAttacked,possessive(players[winner].gender),wAttacker);
            printf("%s strikes back. Hitting %s's %s with %s %s.\n",players[loser].name,players[winner].name,lAttacked,possessive(players[loser].gender),lAttacker);
        }
        return 1; //the winner has won or there has been a tie
    }
    //winner wins
    //parry and counterstrike (winner parries, loser strikes)
    if(strcmp(wAttack,"parry")==0 && strcmp(lAttack,"strike")==0){
        printf("%s strikes %s, but %s parries the blow with %s %s.\n",players[loser].name,players[winner].name,pronoun(players[winner].gender),possessive(players[winner].gender),wAttacker);
        printf("%s delivers a mighty hit on %s's %s!\n",players[winner].name,players[loser].name,lAttacker);
        return 1; //the winner has won or there has been a tie
    }
    //grab (winner grabs, loser parries)
    if(strcmp(wAttack,"grab")==0 && strcmp(lAttack,"parry")==0){
        printf("%s gets a hold of %s's %s with %s %s.\n",players[winner].name,players[loser].name,wAttacked,possessive(players[winner].gender),wAttacker);
        return 1; //the winner has won or there has been a tie
    }
    //strike (winner strikes, loser grabs)
    if(strcmp(wAttack,"strike")==0 && strcmp(lAttack,"grab")==0){
        printf("%s tries to grab %s.\n",players[loser].name,players[winner].name);
        printf("%s strikes %s's %s with %s %s.\n",players[winner].name,players[loser].name,lAttacked,possessive(players[winner].gender),wAttacker);
        return 1; //the winner has won or there has been a tie
    }
    //charge (winner charges, loser parries)
    if(strcmp(wAttack,"charge")==0 && strcmp(lAttack,"parry")==0){
        printf("%s charges at %s! %s stands %s ground...\n",players[winner].name,players[loser].name,players[loser].name,possessive(players[loser].gender));
        return 1; //the winner has won or there has been a tie
    }
    //counter-charge by grabbing and throwing (winner grabs, loser charges)
    if(strcmp(wAttack,"grab")==0 && strcmp(lAttack,"charge")==0){
        printf("%s charges at %s! But %s catches %s with %s %s and throws %s in the air!\n",players[loser].name,players[winner].name,pronoun(players[winner].gender),possessive(players[loser].gender),possessive(players[winner].gender),wAttacker,possessive(players[loser].gender));
        return 1; //the winner has won or there has been a tie
    }
    //counter-charge by striking in the guts (winner strikes, loser charges)
    if(strcmp(wAttack,"strike")==0 && strcmp(lAttack,"charge")==0){
        printf("%s charges at %s! But %s prepares for the impact and strikes %s in the guts using %s %s!\n",players[loser].name,players[winner].name,pronoun(players[winner].gender),players[loser].name,possessive(players[winner].gender),wAttacker);
        return 1; //the winner has won or there has been a tie
    }
    //winner looses
    return 0; //the winner has lost
}
void show_hurt(int pl){
    ;
}
int check_ko(int pl){
    ;
}
//wrestling match
void wrestle(){
    int ko;
    int pl1,pl2; //players that are wrestling
    char wString1[BUFLEN],wString2[BUFLEN]; //wrestle strings
    //pick the players
    pick_players(&pl1,&pl2);
    //announce battle
    printf("b%d: %s vs %s\n",battles+1,players[pl1].name,players[pl2].name);
    getch();
    //start the fight
    do{
        //make the wrestle strings
        make_wrestle_string(players[pl1],wString1);
        make_wrestle_string(players[pl2],wString2);
        //calculate the outcome
        if(whurt(pl1,wString1,pl2,wString2)!=1) whurt(pl2,wString2,pl1,wString1);
        //show if on of the players is hurt
        show_hurt(pl1);
        show_hurt(pl2);
        //check if one of the players is ko
        ko=check_ko(pl1);
    }while(ko==0);
}

///MAIN
int main(){
    get_players();
    intro();
    do{
        wrestle();
    }while(check_end()==0);
    end();
}
