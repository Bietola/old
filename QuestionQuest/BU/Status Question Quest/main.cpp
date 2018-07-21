#include <cassert>
#include <cstring>
#include <curses.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include "menu.h"
#include "palette.h"

#define DEF_FCLR COLOR_WHITE
#define DEF_BCLR COLOR_BLACK

#define C_N '8'
#define C_S '2'
#define C_W '4'
#define C_E '6'
#define C_NW '7'
#define C_NE '9'
#define C_SW '1'
#define C_SE '3'

#define MC_UP '8'
#define MC_DOWN '2'
#define MC_SELECT '5'

using namespace std;

///macro functions
#define ERR(fstr,f) \
    clear(); \
    mvprintw(0,0,fstr,f); \
    getch();

///ENUMERATIONS
enum TILE{
    TL_GROUND,
    TL_GRASS,
    TL_WALL,
    TL_MAX,
};
enum ESSENCE{
    ESS_NOSPAWN,
    ESS_ADV,
    ESS_PAIN,
    ESS_FORCE,
    ESS_MIND,
    ESS_NORMAL,
    ESS_WATER,
    ESS_FIRE,
    ESS_WIND,
    ESS_EARTH,
    ESS_BAGGIO,
    ESS_MAX,
};
enum ATT_TYPE{
    ATT_QUICK,
    ATT_NORMAL,
    ATT_HEAVY,
};
enum QESS{
    QESS_ALL,
    QESS_OPT,
    QESS_MATH,
    QESS_SCIENCE,
    QESS_PHILOSOPHY,
};
enum MOVEACTION{
    MOVEACTION_DAM,
};

///CLASSES
///point class (group of coordinates)
struct Point{
    int x,y;
    Point():x(0),y(0){}
    Point(int x,int y):x(x),y(y){}
};
///status class
struct Status{
    //variables
    float att;
    float def;
    float luck;
    float agi;
    bool nomana;
    //constructors
    Status(float att,float def,float luck,float agi,bool nomana):
        att(att),
        def(def),
        luck(luck),
        agi(agi),
        nomana(nomana)
    {}
    Status():
        att(1),
        def(1),
        luck(1),
        agi(1),
        nomana(0)
    {}
    //functions
    void multiply(Status status){
        att *=status.att;
        def *=status.def;
        luck*=status.luck;
        agi *=status.agi;
        nomana=status.nomana;
    }
    void add(Status status){
        att +=status.att;
        def +=status.def;
        luck+=status.luck;
        agi +=status.agi;
        nomana |= status.nomana;
    }
    void print(int x,int y){
        if(att!=1) mvprintw(y++,x,"ATT: %f",att);
        if(def!=1) mvprintw(y++,x,"DEF: %f",def);
        if(luck!=1) mvprintw(y++,x,"LCK: %f",luck);
        if(agi!=1) mvprintw(y++,x,"AGI: %f",agi);
    }
};
///effect class
struct Effect{
    //variables
    string name;
    string description;
    int compressionLv;
    Status multStatus;
    enum DURATION{
        ATTACK,
        BATTLE,
        TIME,
        SICKNESS
    }durationType;
    union{
        int durationTime;
    };
    //initialization functions
    void start(string effString){
        stringstream ss;
        ss << effString;
        //default values in case nothing is modified
        durationType=ATTACK;
        name="NONE";
        description="NONE";
        multStatus=Status();
        //return if null string
        if(ss.peek()=='.') return;
        //start reading string
        do{
            //get com
            string com;
            while(ss.peek()!='(') com+=ss.get();
            //add name
            if(com=="NAM"){
                assert(ss.get()=='(');
                name.clear();
                while(ss.peek()!=')') name+=ss.get();
                ss.get();
            }
            //add description
            else if(com=="DES"){
                assert(ss.get()=='(');
                description.clear();
                while(ss.peek()!=')') description+=ss.get();
                ss.get();
            }
            //add duration type and info
            else if(com=="DUR"){
                string type;
                assert(ss.get()=='(');
                while(ss.peek()!=',' && ss.peek()!=')') type+=ss.get();
                //decide type
                if(type=="A") durationType=ATTACK;
                else if(type=="B") durationType=BATTLE;
                else if(type=="T") durationType=TIME;
                else if(type=="S") durationType=SICKNESS;
                //decide infos
                if(type=="A" || type=="B"){
                    //no info
                    assert(ss.get()==')');
                }
                else if(type=="T"){
                    //amount of turns
                    string s;
                    assert(ss.get()==',');
                    while(ss.peek()!=')') s+=ss.get();
                    durationTime=atoi(s.c_str());
                    ss.get();
                }
                else if(type=="S"){
                    //WIP (no info for now)
                    assert(ss.get()==')');
                }
                else{
                    //invalid decide type
                    assert(0);
                }
            }
            //add nomana "multiply"
            else if(com=="NMN"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.nomana=atoi(s.c_str());
                ss.get();
            }
            //add attack multiply
            else if(com=="ATT"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.att=atof(s.c_str());
                ss.get();
            }
            //add attack multiply
            else if(com=="DEF"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.def=atof(s.c_str());
                ss.get();
            }
            //add attack multiply
            else if(com=="LCK"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.luck=atof(s.c_str());
                ss.get();
            }
            //add attack multiply
            else if(com=="AGI"){
                string s;
                assert(ss.get()=='(');
                while(ss.peek()!=')') s+=ss.get();
                multStatus.agi=atof(s.c_str());
                ss.get();
            }
            //invalid command
            else{
                assert(0);
            }
            assert(ss.peek()==';' || ss.peek()=='.');
        }while(ss.get()!='.');
    }
    //constructors
    Effect(string name,string description,Status multStatus,DURATION durationType,int durationTime):
        name(name),
        description(description),
        compressionLv(1),
        multStatus(multStatus),
        durationType(durationType),
        durationTime(durationTime)
    {}
    Effect():
        name("NONE"),
        description("NONE"),
        compressionLv(1),
        multStatus(Status()),
        durationType(ATTACK)
    {}
    Effect(string effString):
        compressionLv(1)
    {
        start(effString);
    }
    //getters
    string getDurationTypeString(){
        switch(durationType){
            case ATTACK:
                return "attack";
                break;
            case BATTLE:
                return "battle";
                break;
            case TIME:
                return "time";
                break;
            case SICKNESS:
                return "sickness";
                break;
        }
    }
    string getDurationInfoString(){
        switch(durationType){
            case ATTACK:
                return "lasts until next attack";
                break;
            case BATTLE:
                return "lasts for the remainder of the battle";
                break;
            case TIME:
                return "lasts "+string(durationTime)+" game turns";
                break;
            case SICKNESS:
                return "WIP (works like attack type for now";
                break;
        }
    }
    short getFclr(){
        switch(durationType){
            case ATTACK:
                return COLOR_RED;
                break;
            case BATTLE:
                return COLOR_MAGENTA;
                break;
            case TIME:
                return COLOR_YELLOW;
                break;
            case SICKNESS:
                return COLOR_GREEN;
                break;
        }
    }
    short getBclr(){
        //WIP (always return black
        return COLOR_BLACK;
    }
    //functions
    void compress(Status compressStatus){
        multStatus.multiply(compressStatus);
        compressionLv++;
        if(compressionLv==2){
            name+=" x2";
        }
        else{
            name[name.size()-1]=(char)('0'+compressionLv);
        }
    }
};

///tile class
struct Tile{
    TILE type;
    char gra;
    short fclr,bclr;
    ESSENCE essence;
    int danger;
    bool passable;
    Tile(TILE type,ESSENCE essence,char gra,short fclr,short bclr,int danger,bool passable):type(type),essence(essence),gra(gra),fclr(fclr),bclr(bclr),danger(danger),passable(passable){}
};
///tile global variables
Tile* gTiles[TL_MAX]={
    new Tile(TL_GROUND,ESS_EARTH,'.',COLOR_YELLOW,COLOR_BLACK,0,true),
    new Tile(TL_GRASS,ESS_NORMAL,',',COLOR_GREEN,COLOR_BLACK,1,true),
    new Tile(TL_WALL,ESS_EARTH,'#',COLOR_RED,COLOR_BLACK,0,false)
};

///entity class
struct Entity{
    //variables
    string name,description;
    //constructors
    Entity();
    //functions
    ;
};
///entity functions
Entity::Entity(){
    name="NO NAME";
    description="NO DES";
}

///question class
struct Question{
    //variables
    string question;
    string answers[4];
    QESS essence;
    int dif;
    int rightAnswer;
    //constructors
    Question();
    Question(string q,string a1,string a2,string a3,string a4);
    //functions
    string ask();
    bool isRight(string choice);
};
///question functions
//constructors
Question::Question(){
    question.clear();
    for(int j=0;j<4;j++){
        answers[j].clear();
    }
}
//print the question menu on the screen and make it interactive
string Question::ask(){
    Menu qMenu;
    clear();
    glstart_menu(&qMenu,0,0,(char*)question.c_str(),(char*)answers[0].c_str(),(char*)answers[1].c_str(),(char*)answers[2].c_str(),(char*)answers[3].c_str(),"stop");
    string s(inter_menu(&qMenu));
    return s;
}
//check if specified choice is right
bool Question::isRight(string choice){
    if(choice==answers[rightAnswer]) return true;
    return false;
}
///question global variables
vector<Question> gQuestions;

///attack move class
struct AttMove{
    //variables
    string id,name,des;
    string sverb,pverb;
    int mana;
    int agility;
    int chance;
    int qstDif;
    vector<ESSENCE> essence;
    vector<MOVEACTION> action;
    vector<QESS> qEssence;
    Effect qstEff;
    Effect selfEff;
    Effect eneEff;
    //functions
    void giveNameAndDesToEff(Effect& eff){
        if(eff.name=="NONE") eff.name=name+" effect";
        if(eff.description=="NONE") eff.description="effect given by the move \""+name+"\".";
    }
};

///speak node class
struct SpeakNode{
    //enumerations
    enum TXTOWN{
        TXTOWN_USER,
        TXTOWN_CPU,
    };
    //variables
    string title;
    string ret;
    vector<TXTOWN> textOwners;
    vector<string> text;
    vector<SpeakNode*> links;
    SpeakNode *back;
    //constructors
    SpeakNode(SpeakNode *b=NULL):ret("NONE"),back(b){}
    SpeakNode(string title,SpeakNode *b=NULL):title(title),ret("NONE"),back(b){}
};

///life class
struct Life:public Entity{
    //enumerations
    enum MOVE{
        USER,
        DUMMY,
        IMMOBILE,
    }moveType;
    //variables
    string id,name,des;
    char gra;
    short fclr,bclr;
    ESSENCE essence;
    int x,y;
    int vx,vy;
    int rar,swnLv;
    int hp;
    int mana;
    Status status;
    vector<Effect> effects;
    vector<AttMove*> moves;
    SpeakNode *speakHead;
    vector<string> dialogMods;
    //constructors
    ;
    //get status modified by effects
    Status effSt(){
        Status retSt=status;
        for(int j=0;j<effects.size();j++){
            retSt.multiply(effects[j].multStatus);
        }
        return retSt;
    }
    //virtual functions
    void speak();
    //functions
    string showDialog();
    void setCoos(int x,int y){this->x=x;this->y=y;}
    int getAttMoveIndexByName(string name){
        for(int j=0;j<moves.size();j++){
            if(moves[j]->name==name) return j;
        }
        return -1;
    }
    //add an effect to effect vector of life and return a string announcing them
    string applyEffect(Effect eff){
        ///check if effect is already present
        bool doubleEffect=false;
        for(int j=0;j<effects.size();j++){
            if(effects[j].name==eff.name){
                //compress effect
                effects[j].name;
                effects[j].compress(eff.multStatus);
                doubleEffect=true;
            }
        }
        ///add effect to effects
        if(!doubleEffect) effects.push_back(eff);
        ///build effect announcement string
        stringstream ss;
        string effName=(eff.name=="NONE" ? "effect" : eff.name);
        ss << effName << " applied to " << name << ": ";
        Status st=eff.multStatus;
        //nomana
        if(st.nomana)  ss << "nomana mode activated! /";
        //multipliers
        if(st.att!=1)  ss << "attack boosted by " << st.att << " / ";
        if(st.def!=1)  ss << "defense boosted by " << st.def << " / ";
        if(st.luck!=1) ss << "luck boosted by " << st.luck << " / ";
        if(st.agi!=1)  ss << "agility boosted by " << st.agi << " / ";
        //null string case
        if(ss.str()==effName+" applied to "+name+": ") return "NONE";
        //return
        return ss.str();
    }
    //remove all attack duration type effects
    void resetAttackEffects(){
        for(int j=0;j<effects.size();j++){
            if(effects[j].durationType==Effect::ATTACK){
                effects.erase(effects.begin()+j);
                j--;
            }
        }
    }
    //remove all attack duration type effects
    void resetBattleEffects(){
        for(int j=0;j<effects.size();j++){
            if(effects[j].durationType==Effect::BATTLE){
                effects.erase(effects.begin()+j);
                j--;
            }
        }
    }
    //show all effects applied to life
    void showEffects(){
        clear();
        //print name box
        set_pair(bclr,fclr);
        for(int y=0;y<3;y++) mvaddch(y,0,'*');
        //print "graphics"
        set_pair(fclr,bclr);
        mvaddch(1,2,gra);
        //print name
        set_pair(COLOR_WHITE,COLOR_BLACK);
        mvaddstr(1,4,name.c_str());
        //print effect description divider
        for(int y=3;y<TERMINAL_MAX_Y+1;y++){
            set_pair(COLOR_YELLOW,COLOR_WHITE);
            mvaddch(y,TERMINAL_MAX_X/2,'|');
        }
        //create menu for effects
        Menu effm;
        start_menu(&effm,1,4,"effects",COLOR_WHITE,COLOR_BLACK,'5','8','2','e',COLOR_WHITE,COLOR_RED,"stop");
        for(int j=0;j<effects.size();j++) add_menuMember(&effm,effects[j].name.c_str(),effects[j].getFclr(),effects[j].getBclr());
        add_menuMember(&effm,"exit",COLOR_WHITE,COLOR_BLACK);
        //make menu interactive
        while(true){
            if(effects.size()==0){
                mvaddstr(4,1,"there are no effects...");
                getch();
                break;
            }
            //get choice as id
            int choice=idinter_menu(&effm);
            //exit if necessary
            if(choice==effects.size()) break;
            //clear
            clear_rectangle(TERMINAL_MAX_X/2+1,3,TERMINAL_MAX_X,TERMINAL_MAX_Y);
            //print description of effect with given id
            Effect eff=effects[choice];
            int X=TERMINAL_MAX_X/2+2;
            //name
            set_pair(eff.getBclr(),eff.getFclr());
            mvaddstr(3,X,eff.name.c_str());
            //description
            int y=4;
            set_pair(COLOR_WHITE,COLOR_BLACK);
            for(int j=0;j<eff.description.size();){
                for(int x=X;x<TERMINAL_MAX_X;x++,j++){
                    if(j>=eff.description.size()) break;
                    mvaddch(y,x,eff.description.c_str()[j]);
                }
                y++;
            }
            //duration
            set_pair(eff.getFclr(),eff.getBclr());
            mvprintw(y+1,X,"duration type: %s",eff.getDurationTypeString().c_str());
            //duration info
            set_pair(COLOR_WHITE,COLOR_BLACK);
            mvprintw(y+2,X,"%s",eff.getDurationInfoString().c_str());
            //print status
            set_pair(COLOR_WHITE,COLOR_BLACK);
            eff.multStatus.print(X,y+4);
        }
    }
};
///life global variables
vector<Life*> life;
int mesn=0;
vector<string> allMes;
vector<short> mesColors;
bool mesSub=false;
string speakBox;
Life *adv=NULL;
///life global functions
//search a life by id
Life *search_life(string id){
    for(vector<Life*>::iterator i=life.begin();i<life.end();i++){
        if((*i)->id==id) return *i;
    }
    return NULL;
}
//battle
#define BSMX 51 //!MUST BE ODD
#define BSMY 13
#define BSMM 10
//display battle message
void setMesSub(bool toggle){
    mesSub=toggle;
    if(!mesSub){
        for(int j=0;j<mesColors.size();j++){
            mesColors[j]=DEF_FCLR;
        }
    }
}
void printBatmes(){
    for(int j=0;j<allMes.size();j++){
        set_pair(mesColors[j],DEF_BCLR);
        mvprintw(BSMY+j+1,0,"%s",allMes[j].c_str());
    }
    set_pair(DEF_FCLR,DEF_BCLR);
    refresh();
}
void batmes(string mes){
    if(mes!="NONE"){
        mesn++;
        if(mesn>BSMM){
            allMes.erase(allMes.begin());
            mesColors.erase(mesColors.begin());
            clear_rectangle(0,BSMY+1,TERMINAL_MAX_X,TERMINAL_MAX_Y);
        }
        allMes.push_back(mes);
        if(mesSub) mesColors.push_back(COLOR_YELLOW);
        else mesColors.push_back(DEF_FCLR);
        printBatmes();
    }
}
//normal attack
void attack(int pl,ATT_TYPE type,Life &latt,Life &ldef){
    //calculate hit chance
    stringstream attVerb,missVerb;
    int hitChance,bHitChance=latt.effSt().agi*100/(latt.effSt().agi+ldef.effSt().agi);
    if(type==ATT_NORMAL){
        hitChance=bHitChance;
    }
    else if(type==ATT_QUICK){
        hitChance=bHitChance*(rand()%3+1);
        latt.applyEffect(Effect("Quickness","makes your attacks weaker, but more likely to hit",Status(0.5,1,1,1,1),Effect::ATTACK,0));
        attVerb << "quickly ";
    }
    else if(type==ATT_HEAVY){
        hitChance=bHitChance*0.25*(rand()%4+1);
        latt.applyEffect(Effect("Heaviness","makes your attacks stronger, but less likely to hit",Status(0.5,1,1,1,1),Effect::ATTACK,0));
        attVerb << "heavily ";
    }
    if(pl==0 || pl==2){
        attVerb << "attacks";
        missVerb << "misses";
    }
    else if(pl==1){
        attVerb << "attack";
        missVerb << "miss";
    }
    if(hitChance<40 && type!=ATT_HEAVY) hitChance=40;
    if(hitChance<5) hitChance=5;
    //check if attack is successful
    string defName;
    string attName;
    if(pl==1){
        attName="you";
        defName="the " + ldef.name;
    }
    if(pl==2){
        defName="you";
        attName="the " + latt.name;
    }
    if(rand()%100<hitChance){
        //extract needed values
        int att=(int)latt.effSt().att;
        int attMaxLuck=(int)latt.effSt().luck;
        int def=(int)ldef.effSt().def;
        int defMaxLuck=(int)ldef.effSt().luck;
        //randomly calculate luck boosts
        int attLuckBoost=rand()%attMaxLuck;
        int defLuckBoost=rand()%defMaxLuck;
        //apply luck boosts
        int luckAtt=att+(att*attLuckBoost)/100;
        int luckDef=def+(def*defLuckBoost)/100;
        //calculate damage dealt
        int dam=luckAtt-(luckAtt*luckDef)/100;
        //prevent damage from being negative
        if(dam<0) dam=0;
        //inflict damage
        else ldef.hp-=dam;
        /*
        ERR("%d ",att);
        ERR("%d ",attMaxLuck);
        ERR("%d ",def);
        ERR("%d ",defMaxLuck);
        ERR("%d ",attLuckBoost);
        ERR("%d ",defLuckBoost);
        ERR("%d ",luckAtt);
        ERR("%d ",luckDef);
        ERR("%d ",dam);
        */
        //print attack
        stringstream ss;
        ss << attName << " " << attVerb.str() << " " << defName << " inflicting " << dam << "hp.";
        setMesSub(true);
        batmes(ss.str());
    }
    else{
        //print miss
        stringstream ss;
        ss << attName << " " << missVerb.str() << " " << defName;
        setMesSub(true);
        batmes(ss.str());
    }
    //reset all attack duration multipliers
    latt.resetAttackEffects();
}
//move special attack
bool moveAttack(int pl,Life &latt,Life &ldef,int mvIndex){
    AttMove am=*latt.moves[mvIndex];
    //announce question
    if(pl==1){
        setMesSub(true);
        std::stringstream ss;
        ss << "you use " << latt.moves[mvIndex]->name << "!";
        batmes(ss.str());
    }
    else if(pl==2){
        setMesSub(true);
        std::stringstream ss;
        ss << "the " << latt.name << " uses " << latt.moves[mvIndex]->name << "!";
        batmes(ss.str());
    }
    //player - ask question
    if(pl==1){
        //check if question is needed or optional
        bool question=true;
        if(am.qEssence.empty()) question=false;
        else if(find(am.qEssence.begin(),am.qEssence.end(),QESS_OPT)!=am.qEssence.end()){
            Menu m;
            glstart_menu(&m,BSMX+2,0,"ANSWER A QUESTION?","yes","no","stop");
            string choice(inter_menu(&m));
            if(choice=="no") question=false;
            erase_menu(m);
        }
        //ask question (maybe)
        if(question){
            //check if qessence contains all
            bool all=false;
            if(find(am.qEssence.begin(),am.qEssence.end(),QESS_ALL)!=am.qEssence.end()){
                all=true;
            }
            //find question of the attmove qessence
            int qi;
            do{
                qi=rand()%gQuestions.size();
            }while((gQuestions[qi].essence!=am.qEssence[rand()%am.qEssence.size()] && !all) || gQuestions[qi].dif!=am.qstDif);
            string choice=gQuestions[qi].ask();
            //apply question effect
            if(gQuestions[qi].isRight(choice)){
                mvprintw(7,0,"that is correct!");
                setMesSub(true);
                batmes(latt.applyEffect(am.qstEff));
            }
            else mvprintw(7,0,"wrong answer...");
            refresh();
            getch();
            clear();
        }
    }
    //cpu - 50/50 chance of getting question effects
    else if(pl==2){
        if(rand()%100<50){
            setMesSub(true);
            batmes(latt.applyEffect(am.qstEff));
        }
    }
    //calculate if move happens
    if(rand()%100>=am.chance){
        setMesSub(true);
        if(pl==1) batmes("you fail at applying the move!");
        else{
            std::stringstream ss;
            ss << "the " << latt.name << " fails at applying the move!";
            batmes(ss.str());
        }
        return true;
    }
    //check if enough mana and subtract mana cost
    else if(latt.mana<am.mana){
        setMesSub(true);
        batmes("not enough mana");
        return false;
    }
    else latt.mana-=am.mana;
    //find out agility
    int agility;
    if(am.agility==-1) agility=latt.effSt().agi;
    else agility=(latt.effSt().agi+am.agility)/2;
    //apply self effects
    setMesSub(true);
    batmes(latt.applyEffect(am.selfEff));
    //apply enemy effect
    batmes(ldef.applyEffect(am.eneEff));
    //execute move
    for(vector<MOVEACTION>::iterator q=am.action.begin();q!=am.action.end();q++){
        //attack enemy
        if(*q==MOVEACTION_DAM){
            attack(pl,ATT_NORMAL,latt,ldef);
        }
    }
}
int printBattleArena(Life &you,Life ene){
    //print frame
    set_pair(COLOR_WHITE,COLOR_RED);
    for(int y=0;y<BSMY;y+=BSMY-1){
        for(int x=0;x<BSMX;x++){
            mvaddch(y,x,' ');
        }
    }
    for(int x=0;x<BSMX;x+=(BSMX-1)/2){
        for(int y=1;y<BSMY-1;y++){
            mvaddch(y,x,' ');
        }
    }
    //print you statistics
    set_pair(DEF_FCLR,DEF_BCLR);
    mvprintw(1,1,"%s   ",you.name.c_str());
    mvprintw(2,1,"hp: %d   ",you.hp);
    mvprintw(3,1,"mana: %d   ",you.mana);
    //print ene statistics
    mvprintw(1,(BSMX-1)/2+1,"%s   ",ene.name.c_str());
    mvprintw(2,(BSMX-1)/2+1,"hp: %d   ",ene.hp);
    mvprintw(3,(BSMX-1)/2+1,"mana: %d   ",ene.mana);
    //print graphics
    set_pair(DEF_FCLR,DEF_BCLR);
    mvaddch(BSMY/2,BSMX/4,you.name[0]);
    mvaddch(BSMY/2,BSMX/4*3,ene.name[0]);
}
int battle(Life &you,Life ene){
    clear();
    while(you.hp>0 && ene.hp>0){
        //print "arena"
        printBattleArena(you,ene);
        //print battle menu and proceed with the player's turn
        while(true){
            Menu bm;
            glstart_menu(&bm,BSMX+2,0,"CHOICES:","attack","quick attack","heavy attack","use an attack move","show status effects","show enemy's status effects","escape","stop");
            string choice(inter_menu(&bm));
            //normal attack
            if(choice=="attack"){
                attack(1,ATT_NORMAL,you,ene);
                break;
            }
            //quick attack
            if(choice=="quick attack"){
                attack(1,ATT_QUICK,you,ene);
                break;
            }
            //heavy attack
            if(choice=="heavy attack"){
                attack(1,ATT_HEAVY,you,ene);
                break;
            }
            //attack move
            else if(choice=="use an attack move"){
                erase_menu(bm);
                Menu amm;
                glstart_menu(&amm,BSMX+2,0,"ATT MOVES:","stop");
                for(int j=0;j<you.moves.size();j++){
                    gladd_menuMember(&amm,you.moves[j]->name.c_str());
                }
                gladd_menuMember(&amm,"back <-");
                choice=inter_menu(&amm);
                int attMoveIndex=you.getAttMoveIndexByName(choice);
                if(attMoveIndex!=-1){
                    erase_menu(amm);
                    if(moveAttack(1,you,ene,attMoveIndex)) break;
                    else continue;
                }
                else if(choice=="back <-"){
                    erase_menu(amm);
                    continue;
                }
                else{
                    batmes("ERR: no such move...");
                };
            }
            //show status effects
            else if(choice=="show status effects"){
                you.showEffects();
                clear();
                printBattleArena(you,ene);
                printBatmes();
                continue;
            }
            //show enemy's status effects
            else if(choice=="show enemy's status effects"){
                ene.showEffects();
                clear();
                printBattleArena(you,ene);
                printBatmes();
                continue;
            }
            //escape from battle
            else if(choice=="escape"){
                ///very WIP way of exiting
                ene.hp=-1;
                break;
            }
        }
        //proceed with the CPU's turn
        if(you.hp<0 || ene.hp<0) break;
        //try an attack move for a random amount of time
        int j;
        for(j=rand()%10;j>=0;j--){
            int k;
            if(ene.moves[k=rand()%ene.moves.size()]->mana<=ene.mana && rand()%100<50){
                moveAttack(2,ene,you,k);
                break;
            }
        }
        //choose a random attack based on agility
        if(j==-1){
            int r=rand()%100;
            if(r<15) attack(2,ATT_QUICK,ene,you);
            else if(r+=15<70) attack(2,ATT_NORMAL,ene,you);
            else if(r+=70<15) attack(2,ATT_HEAVY,ene,you);
        }
        setMesSub(false);
    }
    int ret;
    if(you.hp<=0){
        stringstream ss;
        ss << "you die...";
        batmes(ss.str());
        getch();
        ret=1;
    }
    else if(ene.hp<=0){
        stringstream ss;
        ss << ene.name << " dies!";
        batmes(ss.str());
        //reset battle duration types effects
        you.resetBattleEffects();
        getch();
        ret=2;
    }
    else ret=0;
    mesn=0;
    allMes.clear();
    mesColors.clear();
    return ret;
}
//speak box
#define SB_GRA_X 0
#define SB_GRA_Y 15
#define SB_GRA_W 6
#define SB_GRA_H 4
#define SB_BOX_X (SB_GRA_X+SB_GRA_W+1) + 0
#define SB_BOX_Y (SB_GRA_Y)
#define SB_BOX_W 72
#define SB_BOX_H (SB_GRA_H)
#define SM_X 0
#define SM_Y SB_BOX_Y
void interSpeakBox(string s,char gra){
    //print gra borders
    const char BGRA='+';
    set_pair(COLOR_YELLOW,COLOR_BLUE);
    for(int x=0;x<=SB_GRA_W;x++){
        mvaddch(SB_GRA_Y,SB_GRA_X+x,BGRA);
        mvaddch(SB_GRA_Y+SB_GRA_H,SB_GRA_X+x,BGRA);
    }
    for(int y=0;y<=SB_GRA_H;y++){
        mvaddch(SB_GRA_Y+y,SB_GRA_X,BGRA);
        mvaddch(SB_GRA_Y+y,SB_GRA_X+SB_GRA_W,BGRA);
    }
    //print gra
    mvaddch(SB_GRA_Y+SB_GRA_H/2,SB_GRA_X+SB_GRA_W/2,gra);
    //print box borders
    const char BBOX='\\';
    set_pair(COLOR_GREEN,COLOR_YELLOW);
    for(int x=0;x<=SB_BOX_W;x++){
        mvaddch(SB_BOX_Y,SB_BOX_X+x,BBOX);
        mvaddch(SB_BOX_Y+SB_BOX_H,SB_BOX_X+x,BBOX);
    }
    for(int y=0;y<=SB_BOX_H;y++){
        mvaddch(SB_BOX_Y+y,SB_BOX_X,BBOX);
        mvaddch(SB_BOX_Y+y,SB_BOX_X+SB_BOX_W,BBOX);
    }
    //print speak string
    set_pair(COLOR_WHITE,COLOR_BLACK);
    mvaddstr(SB_BOX_Y+SB_BOX_H/2,SB_BOX_X+2,s.c_str());
    //show everything
    refresh();
    getch();
    //erase everything
    draw_rectangle(SB_GRA_X,SB_GRA_Y,SB_GRA_X+SB_GRA_W,SB_GRA_Y+SB_GRA_H,COLOR_BLACK,COLOR_BLACK,' ');
    draw_rectangle(SB_BOX_X,SB_BOX_Y,SB_BOX_X+SB_BOX_W,SB_BOX_Y+SB_BOX_H,COLOR_BLACK,COLOR_BLACK,' ');
}
///life member functions
string Life::showDialog(){
    SpeakNode *sn=speakHead;
    while(true){
        int lastText;
        for(lastText=0;lastText<sn->text.size();lastText++){
            if(sn->text[lastText]!="RET"){
                if(sn->textOwners[lastText]==SpeakNode::TXTOWN_USER) interSpeakBox(sn->text[lastText],adv->gra);
                else interSpeakBox(sn->text[lastText],gra);
            }
        }
        if(sn->ret!="NONE") return sn->ret;
        if(!sn->links.empty()){
            Menu m;
            glstart_menu(&m,SM_X,SM_Y,sn->text[lastText-1].c_str(),"stop");
            for(int j=0;j<sn->links.size();j++){
                gladd_menuMember(&m,sn->links[j]->title.c_str());
            }
            sn=sn->links[idinter_menu(&m)];
            erase_menu(m);
        }
        else return "NONE";
    }
}
void Life::speak(){
    stringstream ss(showDialog());
    string s;
    ss >> s;
    bool err=false;
    //universal dialog returns
    //none (ignore)
    if(s=="NONE"){
        ;
    }
    //battle
    else if(s=="BATTLE"){
        battle(*adv,*this);
    }
    else if(!dialogMods.empty()){
        //specific dialog returns
        for(vector<string>::iterator q=dialogMods.begin();q!=dialogMods.end();q++){
            string dm=*q;
            /*
            //quest maker
            if(dm=="QUESTMAKER"){
                //give a random quest of a specified level
                if(s=="GIVE_RAND_QUEST"){
                    int lv;
                    ss >> lv;
                    adv->addItem(new QuestMap(lv));
                }
                //error
                else err=true;
            }
            */
        }
    }
    else err=true;
    if(err) ERR("%s not recognized as a REC value",s.c_str());
}

/*
///item class
struct Item:public Entity{
    //variables
    string name;
    //pure virtual functions
    virtual void equip()=0;
    virtual void unequip()=0;
};
*/

///board class
struct Board{
    //variables
    vector<Life*> blife;
    vector< vector<TILE> > tiles;
    short bgclr;
    int level;
    //constructors
    Board(int maxx,int maxy,short bgclr,int seed);
    Board(int maxx,int maxy,short bgclr);
    Board();
    //functions
    TILE at(int x,int y);
    Tile getTile(int x,int y);
    Life *getLife(int x,int y);
    void generate(int maxx,int maxy,int seed);
    bool isOcc(int x,int y);
    bool checkSpawn(int x,int y);
    bool spawn(int x,int y,Life* life);
    bool spawn(int x,int y,string id);
    void move();
    void print(int tx,int ty,int bx,int by,int w,int h);
    void print(int sx,int sy);
    void print(int sx,int sy,Life *life);
    void print(Life *life);
    void print();
    //static functions
    static bool isInField(int x,int y,int fx,int fy,int w,int h);
};

///board static functions
//returns true if the given x/y coordinates are within range of the given sx-sy;ex-ey field
bool Board::isInField(int x,int y,int fx,int fy,int w,int h){
    if(x<fx || x>fx+w || y<fy || y>fy+h) return false;
    return true;
}

///board functions
//constructor
Board::Board(int maxx,int maxy,short bgclr,int seed):bgclr(bgclr){generate(maxx,maxy,seed);}
Board::Board(int maxx,int maxy,short bgclr):bgclr(bgclr){generate(maxx,maxy,time(NULL));}
Board::Board():bgclr(COLOR_BLACK){}
//getters
TILE Board::at(int x,int y){
    return tiles[x][y];
}
Tile Board::getTile(int x,int y){
    return *gTiles[tiles[x][y]];
}
Life *Board::getLife(int x,int y){
    for(vector<Life*>::iterator q=blife.begin();q!=blife.end();q++){
        Life *l=*q;
        if(l->x==x && l->y==y) return l;
    }
    return NULL;
}
//other functions
void Board::generate(int maxx,int maxy,int seed){
    srand(seed);
    for(int y=0;y<maxy;y++){
        vector<TILE> v;
        for(int x=0;x<maxx;x++){
            v.push_back((TILE)(rand()%(int)(TL_MAX)));
        }
        tiles.push_back(v);
    }
}
bool Board::isOcc(int x,int y){
    for(int j=0;j<blife.size();j++){
        if(blife[j]->x==x && blife[j]->y==y) return true;
    }
    return false;
}
bool Board::checkSpawn(int x,int y){
    if(isOcc(x,y) || y>=tiles.size() || x>=tiles[0].size() || !getTile(x,y).passable) return false;
    return true;
}
bool Board::spawn(int x,int y,Life* life){
    if(!checkSpawn(x,y)) return false;
    life->setCoos(x,y);
    blife.push_back(life);
    return true;
}
bool Board::spawn(int x,int y,string id){
    return spawn(x,y,new Life(*(search_life(id))));
}
void Board::move(){
    for(int j=0;j<blife.size();j++){
        Life::MOVE mt=blife[j]->moveType;
        if(mt==Life::USER){
            char com=getch();
            int x=0,y=0;
            switch(com){
                case C_N:
                    y--;
                    break;
                case C_E:
                    x++;
                    break;
                case C_S:
                    y++;
                    break;
                case C_W:
                    x--;
                    break;
                case C_NW:
                    x--;
                    y--;
                    break;
                case C_NE:
                    x++;
                    y--;
                    break;
                case C_SE:
                    x++;
                    y++;
                    break;
                case C_SW:
                    x--;
                    y++;
                    break;
                default:
                    break;
            }
            int fx=blife[j]->x+x;
            int fy=blife[j]->y+y;
            if(checkSpawn(fx,fy)){
                blife[j]->x=fx;
                blife[j]->y=fy;
                //check for grass battles
                if(getTile(fx,fy).danger>0){
                    if(rand()%100<100){
                        while(true){
                            int ene=rand()%life.size();
                            //enemy's essence is different from the tile's
                            if(life[ene]->essence!=getTile(fx,fy).essence) continue;
                            //spawn level too high
                            if(life[ene]->swnLv>(float)(level*getTile(fx,fy).danger)/(float)100) continue;
                            //spawn level too low
                            //!if(life[ene]->swnLv<level/5) continue; WIP
                            //rarity test
                            if(rand()%100>(100-life[ene]->rar)) continue;
                            //!WIP upgrade life
                            ;
                            //start battle
                            battle(*(blife[j]),*life[ene]);
                            break;
                        }
                    }
                }
            }
            else if(isOcc(fx,fy)){
                //check if unit can speak
                Life *l=getLife(fx,fy);
                SpeakNode *DB_l_speakHead=l->speakHead;
                if(l->speakHead!=NULL){
                    l->speak();
                }
            }
        }
        else if(mt==Life::DUMMY){
            int x,y;
            do{
                x=rand()%3-1;
                y=rand()%3-1;
            }while(!checkSpawn(blife[j]->x+x,blife[j]->y+y));
            blife[j]->x+=x;
            blife[j]->y+=y;
        }
        else if(mt==Life::IMMOBILE){
            ;
        }
    }
}
void Board::print(int tx,int ty,int bx,int by,int w,int h){
    for(int y=0;y<h;y++){
        for(int x=0;x<w;x++){
            set_pair(getTile(bx+x,by+y).fclr,bgclr);
            mvaddch(ty+y,tx+x,gTiles[tiles[bx+x][by+y]]->gra);
        }
    }
    for(int j=0;j<blife.size();j++){
        Life *l=blife[j];
        if(isInField(l->x,l->y,bx,by,w,h)){
            short bclr=l->bclr;
            if(bclr==COLOR_NULL) bclr=bgclr;
            set_pair(l->fclr,bclr);
            mvaddch(ty+(l->y-by),tx+(l->x-bx),l->gra);
        }
    }
    set_pair(DEF_FCLR,DEF_BCLR);
}
void Board::print(int sx,int sy){
    print(sx,sy,0,0,tiles.size(),tiles[0].size());
}
void Board::print(){
    print(0,0);
}
void Board::print(int sx,int sy,Life* life){
    if(life==NULL) return;
    int vsx=life->x-life->vx;
    int vsy=life->y-life->vy;
    int vw=2*life->vx+1;
    int vh=2*life->vy+1;
    if(vsx<0) vsx=0;
    if(vsx+vw>tiles[0].size()) vsx=tiles[0].size()-vw;
    if(vsy<0) vsy=0;
    if(vsy+vh>tiles.size()) vsy=tiles.size()-vh;
    print(sx,sy,vsx,vsy,vw,vh);
}
void Board::print(Life *life){
    print(0,0,life);
}

///OTHER GLOBAL VARIABLES///
vector<AttMove*> attMoves;
Board board;


///GLOBAL FUNCTIONS///
//start game
short atocolor(string s){
    if(s=="WHITE") return COLOR_WHITE;
    if(s=="BLACK") return COLOR_BLACK;
    if(s=="YELLOW") return COLOR_YELLOW;
    if(s=="CYAN") return COLOR_CYAN;
    return COLOR_RED;
}
Life::MOVE atomovetype(string s){
    if(s=="USER") return Life::USER;
    if(s=="DUMMY") return Life::DUMMY;
    if(s=="IMMOBILE") return Life::IMMOBILE;
    return Life::IMMOBILE;
}
ESSENCE atoess(string s){
    if(s=="ADV") return ESS_ADV;
    if(s=="NORMAL") return ESS_NORMAL;
    if(s=="WIND") return ESS_WIND;
    if(s=="MIND") return ESS_MIND;
    if(s=="FORCE") return ESS_FORCE;
    if(s=="PAIN") return ESS_PAIN;
    return ESS_NOSPAWN;
}
QESS atoqess(string s){
    if(s=="ALL") return QESS_ALL;
    if(s=="OPT") return QESS_OPT;
    if(s=="MATH") return QESS_MATH;
    if(s=="SCIENCE") return QESS_SCIENCE;
    if(s=="PHILOSOPHY") return QESS_PHILOSOPHY;
    return QESS_ALL;
}
MOVEACTION atomoveaction(string s){
    if(s=="DAM") return MOVEACTION_DAM;
    return MOVEACTION_DAM;
}
void start_game(){
    menuEclr=0;
    menuSelKey='s';
    menuUpKey='w';
    menuDownKey='x';
    menuBackKey='e';
    menuTitleFclr=COLOR_RED;
    menuTitleBclr=COLOR_BLACK;
    menuMemberFclr=COLOR_WHITE;
    menuMemberBclr=COLOR_BLACK;
    menuSelFclr=COLOR_BLACK;
    menuSelBclr=COLOR_WHITE;
}
//search a move by id
AttMove *search_move(string id){
    for(vector<AttMove*>::iterator i=attMoves.begin();i<attMoves.end();i++){
        if((*i)->id==id) return *i;
    }
}
//load all questions
void load_questions(const char *fileName){
    ifstream f(fileName);
    string s;
    char c;
    Question q;
    while(f.get()!=':');
    while(true){
        while(f.get()!=':');
        f >> s;
        if(s=="END") break;
        q.essence=atoqess(s);
        f >> q.dif;
        do{
            q.question.push_back(f.get());
        }while(f.peek()!='[');
        for(int j=0;j<4;j++){
            if(j==0) f.get();
            if(f.get()=='1') q.rightAnswer=j;
            f.get();
            do{
                q.answers[j].push_back(f.get());
            }while(f.peek()!=']');
            if(j!=3) while(f.get()!='[');
        }
        gQuestions.push_back(q);
        q.question.clear();
        for(int j=0;j<4;j++) q.answers[j].clear();
    }
    f.close();
}
//load all moves
void load_moves(const char *fileName){
    ifstream f(fileName);
    string s;
    char c;
    while(f.get()!=':');
    while(true){
        AttMove m;
        //id
        f >> m.id;
        if(m.id=="END") break;
        //name
        f >> m.name;
        //verbs
        f >> m.sverb;
        f >> m.pverb;
        //essences
        while(f.get()!='[');
        while(true){
            f >> s;
            if(s=="]") break;
            m.essence.push_back(atoess(s));
        }
        //question types
        while(f.get()!='[');
        while(true){
            f >> s;
            if(s=="]") break;
            m.qEssence.push_back(atoqess(s));
        }
        //questions difficulty
        f >> m.qstDif;
        //mana required
        f >> m.mana;
        //move actions
        while(f.get()!='[');
        while(true){
            f >> s;
            if(s=="]") break;
            m.action.push_back(atomoveaction(s));
        }
        //agility
        f >> m.agility;
        //chance
        f >> m.chance;
        //questions effects
        while(f.get()!='[');
        s.clear();
        while(f.peek()!=']') s+=f.get();
        m.qstEff=Effect(s);
        m.giveNameAndDesToEff(m.qstEff);
        //move base effect on player
        while(f.get()!='[');
        s.clear();
        while(f.peek()!=']') s+=f.get();
        m.selfEff=Effect(s);
        m.giveNameAndDesToEff(m.selfEff);
        //move base effect on enemy
        while(f.get()!='[');
        s.clear();
        while(f.peek()!=']') s+=f.get();
        m.eneEff=Effect(s);
        m.giveNameAndDesToEff(m.eneEff);
        //description
        while(f.get()!='[');
        do{
            m.des.push_back(f.get());
        }while(f.peek()!=']');
        f.get();
        attMoves.push_back(new AttMove(m));
    }
    f.close();
}
//load all living entities
void load_life(const char *fileName){
    ifstream f(fileName);
    string s;
    char c;
    Life l;
    while(f.get()!=':');
    while(true){
        f >> l.id;
        if(l.id=="END") break;
        l.speakHead=NULL;
        l.status=Status();
        f >> c;
        s.clear();
        l.name.clear();
        while(true){
            f >> s;
            if(s=="]") break;
            l.name+=s;
        }
        f >> l.gra;
        f >> s;
        l.fclr=atocolor(s);
        f >> s;
        l.bclr=atocolor(s);
        f >> s;
        l.moveType=atomovetype(s);
        f >> s;
        l.essence=atoess(s);
        f >> l.rar;
        f >> l.swnLv;
        f >> l.hp;
        f >> l.mana;
        f >> l.status.att;
        f >> l.status.def;
        f >> l.status.luck;
        f >> l.status.agi;
        f >> l.vx;
        f >> l.vy;
        for(int j=0;j<4;j++){
            f >> s;
            if(s=="NO") break;
            l.moves.push_back(search_move(s));
        }
        while(f.get()!='[');
        do{
            l.des.push_back(f.get());
        }while(f.peek()!=']');
        f.get();
        life.push_back(new Life(l));
        l.des.clear();
        l.moves.clear();
    }
    f.close();
}
//load life dialog
void load_dialog(const char *fileName){
    ifstream f(fileName);
    //constants
    const char TEXT="TEXT";
    const char LINKS="LINKS";
    const char TITLE="TITLE";
    const char IF="IF";
    const char RET="RET";
    const char START="[";
    const char END="]";
    //variables
    string s;
    Life *l;
    //read mode enum
    enum RM{
        RM_NODE,
        RM_LINKS,
        RM_RET,
    }rm=RM_NODE;
    while(true){
        f >> s;
        if(s=="END") break;
        if((l=search_life(s))==NULL){ERR("%s is not a life id.",s.c_str());}
        else{
            f >> s;
            while(s==":"){
                f >> s;
                l->dialogMods.push_back(s);
                f >> s;
            }
            l->speakHead=new SpeakNode();
            SpeakNode *curSN=l->speakHead;
            s.clear();
            while(true){
                f >> s;
                if(s=="}END") break;
                if(rm==RM_NODE){
                    if(s=="TEXT" || s=="YOUTEXT"){
                        if(s=="YOUTEXT") curSN->textOwners.push_back(SpeakNode::TXTOWN_USER);
                        else curSN->textOwners.push_back(SpeakNode::TXTOWN_CPU);
                        s.clear();
                        f.get();f.get();
                        while(f.peek()!=']') s+=f.get();
                        f.get();
                        curSN->text.push_back(s);
                    }
                    else if(s=="RET"){
                        s.clear();
                        f.get();f.get();
                        while(f.peek()!=']') s+=f.get();
                        f.get();
                        curSN->text.push_back("RET");
                        curSN->ret=s;
                        rm=RM_RET;
                    }
                    else if(s=="LINKS"){
                        f.get();f.get();
                        rm=RM_LINKS;
                        continue;
                    }
                    else if(s=="}"){
                        if(curSN->back==NULL) break;
                        curSN=curSN->back;
                        rm=RM_LINKS;
                        continue;
                    }
                    else{
                        ERR("%s not a valid dialog instruction in NODE read mode",s.c_str());
                    }
                }
                else if(rm==RM_LINKS){
                    if(s=="TITLE"){
                        s.clear();
                        f.get();f.get();
                        while(f.peek()!=']') s+=f.get();
                        f.get();f.get();
                        SpeakNode *tmp=new SpeakNode(s,curSN);
                        curSN->links.push_back(tmp);
                        curSN=curSN->links.at(curSN->links.size()-1);
                        rm=RM_NODE;
                        continue;
                    }
                    else if(s=="}"){
                        rm=RM_NODE;
                        continue;
                    }
                    else{
                        ERR("%s not a valid dialog instruction in LINKS read mode",s.c_str());
                    }
                }
                else if(rm==RM_RET){
                    if(s=="}"){
                        if(curSN->back==NULL) break;
                        curSN=curSN->back;
                        rm=RM_LINKS;
                        continue;
                    }
                    else{
                        ERR("%s not a valid dialog instruction in RET read mode",s.c_str());
                    }
                }
            }
        }
    }
}

///MAIN///
int main()
{
    initscr();
    noecho();
    curs_set(0);
    start_color();
    /*Menu menu;
    start_menu(&menu,0,0,"main menu",COLOR_RED,COLOR_WHITE,'s','u','m',KEY_BACKSPACE,COLOR_BLACK,COLOR_WHITE,"play the game",COLOR_WHITE,COLOR_BLACK,"exit",COLOR_WHITE,COLOR_BLACK,"stop");
    if(!strcmp("play the gamen",inter_menu(menu))){
        printw("ok then :)");
    }*/
    //set menu's global setting
    menuSelKey='5';
    menuUpKey='8';
    menuDownKey='2';
    //load stuff
    load_questions("questions.txt");
    load_moves("moves.txt");
    load_life("life.txt");
    load_dialog("dialog.txt");
    //initialize board
    Board board(10,10,COLOR_BLACK,1);
    //initialize adventurer
    adv=new Life(*(search_life("adventurer")));
    board.spawn(4,4,adv);
    board.spawn(5,5,new Life(*(search_life("crazy_old_man"))));
    board.spawn(5,4,new Life(*(search_life("idiot"))));
    board.spawn(5,3,new Life(*(search_life("quest_giver"))));
    //start main loop
    board.print(1,1,adv);
    do{
        board.move();
        clear();
        board.print(1,1,adv);
    }while(board.blife[0]->hp>0);
    //exit program
    return 0;
}
